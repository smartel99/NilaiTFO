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

#    define TAS_INFO(msg, ...)  LOG_INFO("[%s]: " msg, Device::m_label.c_str(), ##__VA_ARGS__)
#    define TAS_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, Device::m_label.c_str(), ##__VA_ARGS__)

namespace Nilai::Interfaces
{
template<typename Device>
void SwTas5760<Device>::Build(const Tas5760::SwConfig& cfg)
{
    // If a fault pin is given, bind it to the event.
    if (!cfg.Fault.IsDefault())
    {
#    if defined(NILAI_USE_EVENTS)
        Events::EventTypes event = Events::PinToEvent(cfg.Fault);
        m_faultEvent             = event;
        Application::Get()->RegisterEventCallback(event,
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
        Application::Get()->RegisterEventCallback(event,
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
    SetRegister(Tas5760::Registers::VolCtrlCfg, m_cfg.VolumeConfig);
}

template<typename Device>
void SwTas5760<Device>::ToggleSleep(bool s)
{
    m_cfg.PowerConfig.SpkSleep = s;
    SetRegister(Tas5760::Registers::PwrCtrl, m_cfg.PowerConfig);
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
void SwTas5760<Device>::SetChannelVolume(Tas5760::Channels ch, uint8_t vol)
{
    struct ChInfo
    {
        Tas5760::Registers reg;
        uint8_t&           val;
    };
    static ChInfo channels[] = {{Tas5760::Registers::LeftChVolCtrl, m_lVol},
                                {Tas5760::Registers::RightChVolCtrl, m_rVol}};

    ChInfo& channel = channels[(size_t)ch];
    channel.val     = vol;
    SetRegister(channel.reg, channel.val);
}

template<typename Device>
bool SwTas5760<Device>::SetDigitalClipLevel(uint32_t lvl)
{
    //! Bits 19..14 -> Power Control register.
    m_cfg.PowerConfig.DigClipLev = (uint8_t)((lvl & 0x00FC0000) >> 14);
    //! Bits 13..6 -> Digital Clipper 2 register.
    m_cfg.DigClipLev13_6 = (uint8_t)((lvl & 0x00003FC0) >> 6);
    //! Bits 5..0 -> Digital Clipper 1 register.
    m_cfg.DigClipLev5_0 = (uint8_t)((lvl & 0x0000003F) << 2);

    bool isGood = true;
    isGood &= SetRegister(Tas5760::Registers::PwrCtrl, m_cfg.PowerConfig);
    isGood &= SetRegister(Tas5760::Registers::DigClip2, m_cfg.DigClipLev13_6);
    isGood &= SetRegister(Tas5760::Registers::DigClip1, m_cfg.DigClipLev5_0);

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
    Deinit();
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
 * @returns True if the initialization was successful
 * @returns False if an error occurred during the initialization
 */
template<typename Device>
bool SwTas5760<Device>::Init()
{
    m_cfg.SpkShutdown.Set(false);

    Device::StartClock();

    bool isConfigGood = true;
    isConfigGood &= SetRegister(Tas5760::Registers::PwrCtrl, m_cfg.PowerConfig);
    isConfigGood &= SetRegister(Tas5760::Registers::DigCtrl, m_cfg.DigitalConfig);

    // Mute both channels to avoid weird sounds during setup.
    isConfigGood &= SetRegister(Tas5760::Registers::VolCtrlCfg, s_bothChMuted);
    isConfigGood &= SetRegister(Tas5760::Registers::LeftChVolCtrl, m_lVol);
    isConfigGood &= SetRegister(Tas5760::Registers::RightChVolCtrl, m_rVol);
    isConfigGood &= SetRegister(Tas5760::Registers::AnalCtrl, m_cfg.AnalogConfig);
    // Ignore errors for now...
    /*isConfigGood &=*/SetRegister(Tas5760::Registers::FaultCfgAndErrStatus, m_cfg.FaultConfig);
    isConfigGood &= SetRegister(Tas5760::Registers::DigClip2, m_cfg.DigClipLev13_6);
    isConfigGood &= SetRegister(Tas5760::Registers::DigClip1, m_cfg.DigClipLev5_0);

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
        // Fault pin needs to be still LOW. If it isn't low anymore, the error is gone.
        if (!m_cfg.Fault.Get())
        {
            uint8_t s =
              m_cfg.I2c
                ->ReceiveFrameFromRegister(
                  (uint8_t)m_cfg.Address, (uint8_t)Tas5760::Registers::FaultCfgAndErrStatus, 1)
                .data[0];

            if ((s & s_latchingErrors) != 0)
            {
                // Error is latching, cleared by toggling the shutdown pin.
                m_cfg.SpkShutdown.Set(false);
                TAS_ERROR("A fault was detected: %s", StatusToStr(s).c_str());
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
        TAS_INFO("Headphone change detected: %i", (int)m_cfg.HpDetect.Get());
        m_hpChanged = false;
    }
}

template<typename Device>
bool SwTas5760<Device>::SetRegister(Tas5760::Registers r, uint8_t v)
{
    m_cfg.I2c->TransmitFrameToRegister((uint8_t)m_cfg.Address, (uint8_t)r, &v, sizeof(uint8_t));

#    if defined(NILAI_TAS5760_VERIFY_WRITE)
    I2C::Frame f =
      m_cfg.I2c->ReceiveFrameFromRegister((uint8_t)m_cfg.Address, (uint8_t)r, sizeof(uint8_t));
    if (f.data[0] == v)
    {
        // Data read is data written, that's good.
        return true;
    }
    else
    {
        // Mismatch between value read and value expected.
        TAS_ERROR("An error occurred while writing to 0x%02x, Exp: 0x%02x, got: 0x%02x",
                  (uint8_t)r,
                  v,
                  f.data[0]);
        return false;
    }
#    else
    return true;
#    endif
}

template<typename Device>
std::string SwTas5760<Device>::StatusToStr(uint8_t s)
{
    std::string errStr;

    if ((s & s_clkeMask) != 0)
    {
        errStr += "Clock error, ";
    }
    if ((s & s_oceMask) != 0)
    {
        errStr += "Over current, ";
    }
    if ((s & s_dceMask) != 0)
    {
        errStr += "DC error, ";
    }
    if ((s & s_oteMask) != 0)
    {
        errStr += "Over temperature";
    }

    return errStr;
}




}    // namespace Nilai::Interfaces

#endif
