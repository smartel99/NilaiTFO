/**
 * @file    sw_tas5760.cpp
 * @author  Samuel Martel
 * @date    2022-03-03
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#include "sw_tas5760.h"

#if defined(NILAI_USE_TAS5760)
#    include "../../defines/smart_pointers.h"
#    include "../../processes/application.h"
#    include "../../services/logger.h"

#    define TAS_INFO(msg, ...)                                                                     \
        LOG_INFO("[%s]: " msg, Device::m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define TAS_ERROR(msg, ...)                                                                    \
        LOG_ERROR("[%s]: " msg, Device::m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)

namespace Nilai::Interfaces
{
template<typename Device>
void SwTas5760<Device>::Build(const TAS5760::SwConfig& cfg)
{
    // If a fault pin is given, bind it to the event.
    if (!cfg.Fault.IsDefault())
    {
#    if defined(NILAI_USE_EVENTS)
        Events::EventTypes event = Events::PinToEvent(cfg.Fault);
        m_faultEvent             = event;
        Application::Get().RegisterEventCallback(event,
                                                 [this](Events::Event*) -> bool
                                                 {
                                                     m_faultTime = GetTime();
                                                     m_hasFault  = true;
                                                     return false;
                                                 });
        m_faultFunction = [this]() { HandleFault(); };
#    else
        // Events disabled, must be manually checked.
        m_faultFunction = [this]()
        {
            static bool lastState = true;
            bool        newState  = m_cfg.Fault.Get();
            if (lastState != newState)
            {
                lastState = newState;
                // Pin is low on error.
                m_hasFault  = !newState;
                m_faultTime = GetTime();
            }
            HandleFault();
        };
#    endif
    }

    // If a headphone detect pin is given, bind it to the event.
    if (!cfg.HpDetect.IsDefault())
    {
#    if defined(NILAI_USE_EVENTS)
        Events::EventTypes event = Events::PinToEvent(cfg.HpDetect);
        m_hpChangeEvent          = event;
        Application::Get().RegisterEventCallback(event,
                                                 [this](Events::Event*) -> bool
                                                 {
                                                     m_hpChanged = true;
                                                     return false;
                                                 });
        m_hpChangeFunction = [this]() { HandleHeadphoneChange(); };
#    else
        // Events disabled, must be manually checked.
        m_hpChangeFunction = [this]()
        {
            static bool lastState = true;
            bool        newState  = m_cfg.HpDetect.Get();
            if (lastState != newState)
            {
                lastState = newState;
                // Pin is low on error.
                m_hpChanged = !newState;
            }
            HandleHeadphoneChange();
        };
#    endif
    }
}

template<typename Device>
void SwTas5760<Device>::Run()
{
    m_faultFunction();
    m_hpChangeFunction();

    if (Nilai::GetTime() >= m_nextVerificationTime)
    {
        if (!ValidateRegisters())
        {
            SetRegisters();
        }
        m_nextVerificationTime = Nilai::GetTime() + s_verificationPeriod;
    }

    Device::Run();
}

template<typename Device>
bool SwTas5760<Device>::DoPost()
{
    bool hasPassed = Tas5760Module<Device>::DoPost();
    if (Init())
    {
        TAS_INFO("POST OK");
    }
    else
    {
        TAS_ERROR("Unable to initialize TAS5760");
        hasPassed = false;
    }
    return hasPassed;
}

template<typename Device>
void SwTas5760<Device>::ToggleMute(bool s)
{
    m_cfg.VolumeConfig.MuteL = s;
    m_cfg.VolumeConfig.MuteR = s;
    SetRegister(TAS5760::Registers::VolCtrlCfg, static_cast<uint8_t>(m_cfg.VolumeConfig));
}

template<typename Device>
void SwTas5760<Device>::ToggleSleep(bool s)
{
    m_cfg.PowerConfig.SpkSleep = s;
    SetRegister(TAS5760::Registers::PwrCtrl, static_cast<uint8_t>(m_cfg.PowerConfig));
}

template<typename Device>
bool SwTas5760<Device>::IsAsleep() const
{
    return m_cfg.PowerConfig.SpkSleep == 1;
}

template<typename Device>
void SwTas5760<Device>::ToggleShutdown(bool s)
{
    // Shutdown pin is active low.
    m_isShutdown = s;
    m_cfg.SpkShutdown.Set(!m_isShutdown);
}

template<typename Device>
bool SwTas5760<Device>::IsShutdown() const
{
    return m_isShutdown;
}

template<typename Device>
void SwTas5760<Device>::SetChannelVolume(TAS5760::Channels ch, uint8_t vol)
{
    struct ChInfo
    {
        TAS5760::Registers reg;
        uint8_t&           val;
    };
    std::array channels = {ChInfo {TAS5760::Registers::LeftChVolCtrl, m_lVol},
                           ChInfo {TAS5760::Registers::RightChVolCtrl, m_rVol}};
    auto       chId     = static_cast<size_t>(ch);
    NILAI_ASSERT(chId < channels.size(), "Invalid Channel");

    ChInfo& channel = channels[chId];
    channel.val     = vol;
    SetRegister(channel.reg, channel.val);
}

template<typename Device>
bool SwTas5760<Device>::SetDigitalClipLevel(uint32_t lvl)
{
    //! Bits 19..14 -> Power Control register.
    m_cfg.PowerConfig.DigClipLev = static_cast<uint8_t>(((lvl & 0x00FC0000) >> 14));
    //! Bits 13..6 -> Digital Clipper 2 register.
    m_cfg.DigClipLev13_6 = static_cast<uint8_t>(((lvl & 0x00003FC0) >> 6));
    //! Bits 5..0 -> Digital Clipper 1 register.
    m_cfg.DigClipLev5_0 = static_cast<uint8_t>(((lvl & 0x0000003F) << 2));

    bool isGood = true;
    isGood &= SetRegister(TAS5760::Registers::PwrCtrl, static_cast<uint8_t>(m_cfg.PowerConfig));
    isGood &= SetRegister(TAS5760::Registers::DigClip2, m_cfg.DigClipLev13_6);
    isGood &= SetRegister(TAS5760::Registers::DigClip1, m_cfg.DigClipLev5_0);

    return isGood;
}

template<typename Device>
bool SwTas5760<Device>::Stream(const void* samples, size_t cnt)
{
    return Device::Stream(samples, cnt);
}

template<typename Device>
bool SwTas5760<Device>::PauseStream()
{
    ToggleMute(true);
    ToggleSleep(true);
    return Device::PauseStream();
}

template<typename Device>
bool SwTas5760<Device>::ResumeStream()
{
    ToggleMute(false);
    ToggleSleep(false);
    return Device::ResumeStream();
}

template<typename Device>
bool SwTas5760<Device>::StopStream()
{
    //    Deinit();
    ToggleMute(true);
    ToggleSleep(true);
    return Device::StopStream();
}


//--------------------------------------------------------------------------------------------------
// Private Methods Definitions.

/**
 * @brief Execute the startup procedure for the TAS5760.
 *
 * The procedure:
 * <ol>
 *  <li>Configure all digital I/O pins as required by the application using PCB connections (should
 * be done by the hardware).</li>
 *  <li>Start with SPK_SD pin = LOW</li>
 *  <li>Bring up power supplies (the order does not matter, as long as the device is in
 * shutdown)</li>
 *  <li>Once power supplies are stable, start MCLK, SCLK and LRCK</li>
 *  <li>Configure the device via i2c, making sure to mute the device beforehand.</li>
 *  <li>Once power supplies and clocks are stable and the control port has been programmed, bring
 * SPK_SD HIGH</li>
 *  <li>Unmute the device via I2C</li>
 *  <li>The device is now in normal operation</li>
 * </ol>
 *
 * @tparam Device A class that can serve as the physical interface.
 * @returns True if the initialization was successful
 * @returns False if an error occurred during the initialization
 */
template<typename Device>
bool SwTas5760<Device>::Init()
{
    m_cfg.SpkShutdown.Set(false);

    Device::StartClock();

    bool isConfigGood = SetRegisters();

    Device::StopClock();

    if (isConfigGood)
    {
        // Bring the chip out of shutdown.
        //        m_cfg.SpkShutdown.Set(true);
        // Unmute the 2 channels.
        //        ToggleMute(false);
    }

    return isConfigGood;
}

/**
 * @brief Gracefully shuts the TAS5760 down.
 * @tparam Device A class that can serve as the physical interface with the TAS5760.
 * @return true
 */
template<typename Device>
bool SwTas5760<Device>::Deinit()
{
    ToggleMute(true);

    m_cfg.SpkShutdown.Set(false);    // Shut the chip down.

    Device::StopClock();

    return true;
}

template<typename Device>
void SwTas5760<Device>::HandleFault()
{
    if (m_hasFault && (GetTime() >= (m_faultTime + s_minErrorTime)))
    {
        if (!Device::IsStreaming())
        {
            return;
        }
        // Fault pin needs to be still LOW. If it isn't low anymore, the error is gone.
        //        if (!m_cfg.Fault.Get())
        {
            uint8_t s = m_cfg.I2c
                          .ReceiveFrameFromRegister(
                            static_cast<uint8_t>(m_cfg.Address),
                            static_cast<uint8_t>(TAS5760::Registers::FaultCfgAndErrStatus),
                            1)
                          .data[0];

            if ((s & s_latchingErrors) != 0)
            {
                // Error is latching, cleared by toggling the shutdown pin.
                m_cfg.SpkShutdown.Set(false);
                TAS_ERROR("A fault was detected: %s", StatusToStr(s).c_str());
            }
            else if ((s & s_clkeMask) != 0)
            {
                TAS_ERROR("A clock error was detected");
                m_onErrorCb();
            }
        }
        m_hasFault = false;
    }
}

template<typename Device>
void SwTas5760<Device>::HandleHeadphoneChange()
{
    if (m_hpChanged)
    {
        TAS_INFO("Headphone change detected: %i", static_cast<int>(m_cfg.HpDetect.Get()));
        m_hpChanged = false;
    }
}

template<typename Device>
bool SwTas5760<Device>::SetRegister(TAS5760::Registers r, uint8_t v)
{
    m_cfg.I2c.TransmitFrameToRegister(
      static_cast<uint8_t>(m_cfg.Address), static_cast<uint8_t>(r), &v, sizeof(uint8_t));

#    if defined(NILAI_TAS5760_VERIFY_WRITE)
    uint8_t readBack = GetRegister(r);
    if (readBack == v)
    {
        // Data read is data written, that's good.
        return true;
    }
    else if (r != TAS5760::Registers::FaultCfgAndErrStatus)    // that register has read-only bits
                                                               // that are always set.
    {
        // Mismatch between value read and value expected.
        TAS_ERROR("An error occurred while writing to 0x%02x, Exp: 0x%02x, got: 0x%02x",
                  static_cast<uint8_t>(r),
                  v,
                  readBack);
        return false;
    }
    return false;
#    else
    return true;
#    endif
}

template<typename Device>
uint8_t SwTas5760<Device>::GetRegister(TAS5760::Registers r)
{
    I2C::Frame f = m_cfg.I2c.ReceiveFrameFromRegister(
      static_cast<uint8_t>(m_cfg.Address), static_cast<uint8_t>(r), sizeof(uint8_t));
    return f.data[0];
}

template<typename Device>
const std::string& SwTas5760<Device>::StatusToStr(uint8_t s)
{
    using namespace std::string_literals;
    static std::string errStr = std::string(64, '\0');    // Pre-allocate 64 bytes.
    errStr.clear();

    if ((s & s_clkeMask) != 0)
    {
        errStr += "Clock error, "s;
    }
    if ((s & s_oceMask) != 0)
    {
        errStr += "Over current, "s;
    }
    if ((s & s_dceMask) != 0)
    {
        errStr += "DC error, "s;
    }
    if ((s & s_oteMask) != 0)
    {
        errStr += "Over temperature"s;
    }

    // Remove any trailing ',' at the end of the string.
    if (errStr.ends_with(','))
    {
        errStr.pop_back();
    }

    return errStr;
}

template<typename Device>
bool SwTas5760<Device>::ValidateRegisters()
{
    using TAS5760::Registers;

    auto check_register = [this](const char* name, Registers r, uint8_t expected) -> bool
    {
        if (auto v = GetRegister(r); v != expected)
        {
            TAS_ERROR("%s register (%#02x) is not as expected (%#02x)", name, v, expected);
            return false;
        }
        return true;
    };
#    define CHECK_REGISTER(reg, value)                                                             \
        if (!check_register(#reg, Registers::reg, static_cast<uint8_t>(value))) return false

    CHECK_REGISTER(PwrCtrl, m_cfg.PowerConfig);
    CHECK_REGISTER(DigCtrl, m_cfg.DigitalConfig);
    CHECK_REGISTER(VolCtrlCfg, m_cfg.VolumeConfig);
    CHECK_REGISTER(LeftChVolCtrl, m_lVol);
    CHECK_REGISTER(RightChVolCtrl, m_rVol);
    CHECK_REGISTER(AnalCtrl, m_cfg.AnalogConfig);
    CHECK_REGISTER(DigClip2, m_cfg.DigClipLev13_6);
    CHECK_REGISTER(DigClip1, m_cfg.DigClipLev5_0);

#    undef CHECK_REGISTER

    return true;
}

template<typename Device>
bool SwTas5760<Device>::SetRegisters()
{
    bool isConfigGood = true;
    isConfigGood &=
      SetRegister(TAS5760::Registers::PwrCtrl, static_cast<uint8_t>(m_cfg.PowerConfig));
    isConfigGood &=
      SetRegister(TAS5760::Registers::DigCtrl, static_cast<uint8_t>(m_cfg.DigitalConfig));

    // Mute both channels to avoid weird sounds during setup.
    isConfigGood &= SetRegister(TAS5760::Registers::VolCtrlCfg, s_bothChMuted);

    isConfigGood &= SetRegister(TAS5760::Registers::LeftChVolCtrl, m_lVol);
    isConfigGood &= SetRegister(TAS5760::Registers::RightChVolCtrl, m_rVol);
    isConfigGood &=
      SetRegister(TAS5760::Registers::AnalCtrl, static_cast<uint8_t>(m_cfg.AnalogConfig));
    // Ignore errors for now...
    /*isConfigGood &=*/
    SetRegister(TAS5760::Registers::FaultCfgAndErrStatus, static_cast<uint8_t>(m_cfg.FaultConfig));
    isConfigGood &= SetRegister(TAS5760::Registers::DigClip2, m_cfg.DigClipLev13_6);
    isConfigGood &= SetRegister(TAS5760::Registers::DigClip1, m_cfg.DigClipLev5_0);

    // (possibly) unmute the channels.
    isConfigGood &=
      SetRegister(TAS5760::Registers::VolCtrlCfg, static_cast<uint8_t>(m_cfg.VolumeConfig));
    return isConfigGood;
}
}    // namespace Nilai::Interfaces

#endif
