/**
 * @file    SwTas5760.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#include "SwTas5760.h"

#if defined(NILAI_USE_TAS5760)

#    include "../../processes/application.hpp"
#    include "../../services/logger.hpp"

#    define TAS_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define TAS_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

using namespace cep;

SwTas5760::SwTas5760(const cep::Tas5760::SwConfig& cfg,
                     I2S_HandleTypeDef*            i2s,
                     const std::string&            label)
: Tas5760Module(i2s, label), m_cfg(cfg)
{
    Application* app = Application::Get();

    // If a fault pin is given, bind it to the event.
    if (!cfg.Fault.IsDefault())
    {
        Events::EventTypes event = Events::PinToEvent(cfg.Fault);
        app->RegisterEventCallback(event,
                                   [this](Events::Event*) -> bool
                                   {
                                       m_hasFault = true;
                                       m_cfg.SpkShutdown.Set(false);
                                       return false;
                                   });
        m_faultFunction = [this]() { HandleFault(); };
    }

    // If a headphone detect pin is given, bind it to the event.
    if (!cfg.HpDetect.IsDefault())
    {
        Events::EventTypes event = Events::PinToEvent(cfg.HpDetect);
        app->RegisterEventCallback(event,
                                   [this](Events::Event*) -> bool
                                   {
                                       m_hpChanged = true;
                                       return false;
                                   });
        m_hpChangeFunction = [this]() { HandleHeadphoneChange(); };
    }

    if (Init())
    {
        TAS_INFO("Initialized.");
    }
    else
    {
        TAS_ERROR("Unable to initialize TAS5760.");
    }
}

void SwTas5760::Run()
{
    m_faultFunction();
    m_hpChangeFunction();
}

bool SwTas5760::DoPost()
{
    return Tas5760Module::DoPost();
}

void SwTas5760::ToggleMute(bool s)
{
    m_cfg.VolumeConfig.MuteL = s;
    m_cfg.VolumeConfig.MuteR = s;
    SetRegister(cep::Tas5760::Registers::VolCtrlCfg, m_cfg.VolumeConfig);
}

void SwTas5760::ToggleSleep(bool s)
{
    // TODO
}

bool SwTas5760::IsAsleep() const
{
    return false;
}

void SwTas5760::ToggleShutdown(bool s)
{
    // Shutdown pin is active low.
    m_isShutdown = s;
    m_cfg.SpkShutdown.Set(!m_isShutdown);
}

bool SwTas5760::IsShutdown() const
{
    return m_isShutdown;
}

void SwTas5760::SetChannelVolume(cep::Tas5760::Channels ch, uint8_t vol)
{
    struct ChInfo
    {
        cep::Tas5760::Registers reg;
        uint8_t&                val;
    };
    ChInfo channels[] = {{cep::Tas5760::Registers::LeftChVolCtrl, m_lVol},
                         {cep::Tas5760::Registers::RightChVolCtrl, m_rVol}};

    ChInfo& channel = channels[(size_t)ch];
    channel.val     = vol;
    SetRegister(channel.reg, channel.val);
}

bool SwTas5760::SetDigitalClipLevel(uint32_t lvl)
{
    //! Bits 19..14 -> Power Control register.
    m_cfg.PowerConfig.DigClipLev = (uint8_t)((lvl & 0x00FC0000) >> 14);
    //! Bits 13..6 -> Digital Clipper 2 register.
    m_cfg.DigClipLev13_6 = (uint8_t)((lvl & 0x00003FC0) >> 6);
    //! Bits 5..0 -> Digital Clipper 1 register.
    m_cfg.DigClipLev5_0 = (uint8_t)((lvl & 0x0000003F) << 2);

    bool isGood = true;
    isGood &= SetRegister(cep::Tas5760::Registers::PwrCtrl, m_cfg.PowerConfig);
    isGood &= SetRegister(cep::Tas5760::Registers::DigClip2, m_cfg.DigClipLev13_6);
    isGood &= SetRegister(cep::Tas5760::Registers::DigClip1, m_cfg.DigClipLev5_0);

    return isGood;
}

bool SwTas5760::Stream(const uint16_t* samples, size_t cnt)
{
    ToggleMute(false);
    return I2sModule::Stream(samples, cnt);
}

bool SwTas5760::PauseStream()
{
    ToggleMute(true);
    return I2sModule::PauseStream();
}

bool SwTas5760::ResumeStream()
{
    ToggleMute(false);
    return I2sModule::ResumeStream();
}

bool SwTas5760::StopStream()
{
    ToggleMute(true);
    return I2sModule::StopStream();
}

//--------------------------------------------------------------------------------------------------
// Private Methods Definitions.

/**
 * @brief Execute the startup procedure for the TAS5760.
 *
 * The procedure:
 * <ol>
 *  <li>Configure all digital I/O pins as required by the application using PCB connections (should
 * be done by the hardware).</li> <li>Start with SPK_SD pin = LOW</li> <li>Bring up power supplies
 * (the order does not matter, as long as the device is in shutdown)</li> <li>Once power supplies
 * are stable, start MCLK, SCLK and LRCK</li> <li>Configure the device via i2c, making sure to mute
 * the device beforehand.</li> <li>Once power supplies and clocks are stable and the control port
 * has been programmed, bring SPK_SD HIGH</li> <li>Unmute the device via I2C</li> <li>The device is
 * now in normal operation</li>
 * </ol>
 * @returns True if the initialization was successful
 * @returns False if an error occurred during the initialization
 */
bool SwTas5760::Init()
{
    //    HAL_Delay(13);
    m_cfg.SpkShutdown.Set(false);

    StartClock();

    bool isConfigGood = true;
    isConfigGood &= SetRegister(cep::Tas5760::Registers::PwrCtrl, m_cfg.PowerConfig);
    isConfigGood &= SetRegister(cep::Tas5760::Registers::DigCtrl, m_cfg.DigitalConfig);

    // Mute both channels to avoid weird sounds during setup.
    isConfigGood &= SetRegister(cep::Tas5760::Registers::VolCtrlCfg, s_bothChMuted);
    isConfigGood &= SetRegister(cep::Tas5760::Registers::LeftChVolCtrl, m_lVol);
    isConfigGood &= SetRegister(cep::Tas5760::Registers::RightChVolCtrl, m_rVol);
    isConfigGood &= SetRegister(cep::Tas5760::Registers::AnalCtrl, m_cfg.AnalogConfig);
    // Ignore errors for now...
    /*isConfigGood &=*/SetRegister(cep::Tas5760::Registers::FaultCfgAndErrStatus,
                                   m_cfg.FaultConfig);
    isConfigGood &= SetRegister(cep::Tas5760::Registers::DigClip2, m_cfg.DigClipLev13_6);
    isConfigGood &= SetRegister(cep::Tas5760::Registers::DigClip1, m_cfg.DigClipLev5_0);

    if (isConfigGood)
    {
        // Bring the chip out of shutdown.
        m_cfg.SpkShutdown.Set(true);
        // Unmute the 2 channels.
        isConfigGood &= SetRegister(cep::Tas5760::Registers::VolCtrlCfg, m_cfg.VolumeConfig);
    }

    return isConfigGood;
}

void SwTas5760::HandleFault()
{
}

void SwTas5760::HandleHeadphoneChange()
{
}

bool SwTas5760::SetRegister(cep::Tas5760::Registers r, uint8_t v)
{
    m_cfg.I2c->TransmitFrameToRegister((uint8_t)m_cfg.Address, (uint8_t)r, &v, sizeof(uint8_t));

#    if defined(NILAI_TAS5760_VERIFY_WRITE)
    cep::I2C::Frame f =
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




#endif
