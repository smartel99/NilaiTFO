/**
 * @file    Tas5707Module.cpp
 * @author  Samuel Martel
 * @date    2022-02-07
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
#include "Tas5707Module.h"

#include "Enums.h"

#include "../../services/logger.hpp"

#define TAS_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#define TAS_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

Tas5707Module::Tas5707Module(const cep::Tas5707::HardwareConfig& hw,
                             const cep::Tas5707::SoftwareConfig& sw,
                             std::string                         label)
: m_hw(hw), m_sw(sw), m_label(std::move(label))
{
    CEP_ASSERT(m_hw.IsValid(), "Hardware configuration is not valid");
    CEP_ASSERT(m_sw.Validate(), "Software configuration is not valid");

    // Set all digital inputs to low first.
    m_hw.PVddEn.Set(false);
    m_hw.PwrDwn.Set(false);
    m_hw.Reset.Set(false);
    m_hw.Mute.Set(false);
    m_hw.HeadphoneSelect.Set(false);

    // Drive PwrDwn to high and keep Reset low, mute low (no mute), headphone select low (speaker
    // mode).
    m_hw.PwrDwn.Set(true);

    // We normally have to first provide stable and valid I2S clocks for at least 100us, but that
    // should already have been taken care of.
    // We now release the reset and wait a minimum of 13.5ms.
    m_hw.Reset.Set(true);
    HAL_Delay(14);

    // Enable PVDD and leave it some time to settle. The chip needs at least 10us after PVDD
    // stablizes.
    m_hw.PVddEn.Set(true);
    HAL_Delay(4);    // TODO Tune the PVDD delay.

    // Identify the I2C address of the chip, which varies by models.
    if (!FindTas5707I2CAddr())
    {
        TAS_ERROR("Unable to find TAS5707 on the I2C bus!");
        // Shut it down, to be sure nothing explodes.
        m_hw.PwrDwn.Set(false);
        m_hw.Reset.Set(false);
        m_hw.PVddEn.Set(false);
        CEP_ASSERT(false, "Unable to configure TAS5707");
        return;
    }

    // Tell the chip to trim its internal oscillator.
    TrimOscillator();
}

Tas5707Module::~Tas5707Module()
{
}

bool Tas5707Module::DoPost()
{
    return false;
}

void Tas5707Module::Run()
{
}


bool Tas5707Module::FindTas5707I2CAddr()
{
    using namespace cep::Tas5707;
    if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707Default))
    {
        TAS_INFO("Device is default TAS5707");
        m_i2cAddr = I2CAddresses::TAS5707Default;
    }
    else if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707ADefault))
    {
        TAS_INFO("Device is default TAS5707A");
        m_i2cAddr = I2CAddresses::TAS5707ADefault;
    }
    else if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707Alt))
    {
        TAS_INFO("Device is alternative TAS5707");
        m_i2cAddr = I2CAddresses::TAS5707Alt;
    }
    else if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707AAlt))
    {
        TAS_INFO("Device is alternative TAS5707A");
        m_i2cAddr = I2CAddresses::TAS5707AAlt;
    }

    // Returns true if we've found a TAS5707, false otherwise.
    return m_i2cAddr != 0;
}

void Tas5707Module::TrimOscillator()
{
    uint8_t v =
      m_hw.I2cHandle
        ->ReceiveFrameFromRegister(m_i2cAddr, (uint8_t)cep::Tas5707::Registers::OscTrim, 1)
        .data[0];

    // If OscTrim is set to 0xC0, the oscillator is already trimmed.
    if (v != 0xC0)
    {
        m_hw.I2cHandle->TransmitFrameToRegister(
          m_i2cAddr, (uint8_t)cep::Tas5707::Registers::OscTrim, {0});
        // This can take up to 50ms.
        uint32_t start = HAL_GetTick();
        while ((HAL_GetTick() <= start + 50) && v != 0xC0)
        {
            v =
              m_hw.I2cHandle
                ->ReceiveFrameFromRegister(m_i2cAddr, (uint8_t)cep::Tas5707::Registers::OscTrim, 1)
                .data[0];
            TAS_INFO("Oscillator status: %#02X", v);
            HAL_Delay(1);
        }
        if (v != 0xC0)
        {
            TAS_ERROR("Oscillator is not ready after 50ms!");
            return;
        }
    }

    TAS_INFO("Oscillator ready");
}
