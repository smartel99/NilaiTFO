/**
 ******************************************************************************
 * @addtogroup adsModule
 * @{
 * @file    adsModule
 * @author  Samuel Martel
 * @brief   Source for the adsModule module.
 *
 * @date 9/2/2020 11:54:30 AM
 *
 ******************************************************************************
 */
#include "adsModule.h"
#if defined(NILAI_USE_ADS) && defined(NILAI_USE_SPI)
#include "defines/macros.hpp"
#include "defines/bitManipulations.hpp"
#include "defines/globaldef.h"
#include "services/logger.hpp"

void AdsModule::Run()
{
}

void AdsModule::Configure(const ADS::Config &config, bool force)
{
    // If the config we received is the one already in the ADS
    // and we don't want to forcefully reset that configuration:
    if ((config == m_config) && force == false)
    {
        // No need to do anything.
        return;
    }

    m_config = config;
    m_active = false;

    Reset();

    if (SendCommand(ADS::SysCommands::Reset, ADS::Acknowledges::Ready) == false)
    {
        LOG_DEBUG("Unable to reset.");
        return;
    }

    if (SendCommand(ADS::SysCommands::Unlock, ADS::Acknowledges::Unlock)
            == false)
    {
        LOG_DEBUG("Unable to unlock.");
        return;
    }

    ADS::Registers::ClockConfig1 clk1(m_config);
    if (SendConfig(ADS::Registers::ClockConfig1::Address, clk1.value) == false)
    {
        LOG_DEBUG("Unable to setup Clock Config 1 Register.");
        return;
    }

    ADS::Registers::ClockConfig2 clk2(m_config);
    if (SendConfig(ADS::Registers::ClockConfig2::Address, clk2.value) == false)
    {
        LOG_DEBUG("Unable to setup Clock Config 2 Register.");
        return;
    }

    ADS::Registers::DigitalSysConfig dsc(m_config);
    if (SendConfig(ADS::Registers::DigitalSysConfig::Address, dsc.value)
            == false)
    {
        LOG_DEBUG("Unable to setup Digital System Configuration Register.");
        return;
    }

    ADS::Registers::AnalogSysConfig asc(m_config);
    if (SendConfig(ADS::Registers::AnalogSysConfig::Address, asc.value)
            == false)
    {
        LOG_DEBUG("Unable to setup Analog System Configuration Register");
        return;
    }

    ADS::Registers::Ch1DigitalGain ch1(m_config);
    if (SendConfig(ADS::Registers::Ch1DigitalGain::Address, ch1.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel 1 Digital Gain.");
        return;
    }

    ADS::Registers::Ch2DigitalGain ch2(m_config);
    if (SendConfig(ADS::Registers::Ch2DigitalGain::Address, ch2.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel 2 Digital Gain.");
        return;
    }

    ADS::Registers::Ch3DigitalGain ch3(m_config);
    if (SendConfig(ADS::Registers::Ch3DigitalGain::Address, ch3.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel 3 Digital Gain.");
        return;
    }

    ADS::Registers::Ch4DigitalGain ch4(m_config);
    if (SendConfig(ADS::Registers::Ch4DigitalGain::Address, ch4.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel 4 Digital Gain.");
        return;
    }

    ADS::Registers::ChannelEnable en(m_config);
    if (SendConfig(ADS::Registers::ChannelEnable::Address, en.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel Enable register.");
        return;
    }

    if (SendCommand(ADS::SysCommands::Wakeup, ADS::Acknowledges::Wakeup)
            == false)
    {
        LOG_DEBUG("Unable to wake ADS.");
        return;
    }

    if (SendCommand(ADS::SysCommands::Lock, ADS::Acknowledges::Lock) == false)
    {
        LOG_DEBUG("Unable to lock ADS.");
        return;
    }

    m_active = true;
    LOG_DEBUG("ADS Configuration Completed!");
}

void AdsModule::Enable()
{
    if (SendCommand(ADS::SysCommands::Unlock, ADS::Acknowledges::Unlock)
            == false)
    {
        LOG_DEBUG("Unable to unlock.");
        return;
    }
    m_config.enable = ADS::Enable::Enable;
    ADS::Registers::ChannelEnable en(m_config);
    if (SendConfig(ADS::Registers::ChannelEnable::Address, en.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel Enable register.");
        return;
    }
    if (SendCommand(ADS::SysCommands::Lock, ADS::Acknowledges::Lock) == false)
    {
        LOG_DEBUG("Unable to lock ADS.");
        return;
    }
}

void AdsModule::Disable()
{
    if (SendCommand(ADS::SysCommands::Unlock, ADS::Acknowledges::Unlock)
            == false)
    {
        LOG_DEBUG("Unable to unlock.");
        return;
    }
    m_config.enable = ADS::Enable::Enable;
    ADS::Registers::ChannelEnable en(m_config);
    if (SendConfig(ADS::Registers::ChannelEnable::Address, en.value) == false)
    {
        LOG_DEBUG("Unable to setup Channel Enable register.");
        return;
    }
    if (SendCommand(ADS::SysCommands::Lock, ADS::Acknowledges::Lock) == false)
    {
        LOG_DEBUG("Unable to lock ADS.");
        return;
    }
}

const AdsPacket& AdsModule::RefreshValues(uint32_t timeout)
{
    if (!m_active)
    {
        return m_latestFrame;
    }

    // If a timeout was specified:
    if (timeout > 0)
    {
        uint32_t timeoutTime = HAL_GetTick() + timeout;

        // Wait for DRDY to be active (LOW).
        while (HAL_GPIO_ReadPin(m_config.pins.dataReady.port,
                                m_config.pins.dataReady.pin) != GPIO_PIN_RESET)
        {
            // If we time out:
            if (HAL_GetTick() >= timeoutTime)
            {
                // Don't update the cached values and bail out.
                return m_latestFrame;
            }
        }
    }

    std::vector<uint8_t> data = std::vector<uint8_t>(15);

    // Activate CS to start transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port,
                      m_config.pins.chipSelect.pin, GPIO_PIN_RESET);
    m_spi->Receive(data.data(), data.size());
    // Release CS to complete transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port,
                      m_config.pins.chipSelect.pin, GPIO_PIN_SET);

    // First 3 bytes are for the status.
    // Each consecutive trios of bytes represent the data of a channel.
    int32_t ch1 = CalculateTension(&data.data()[3]);
    int32_t ch2 = CalculateTension(&data.data()[6]);
    int32_t ch3 = CalculateTension(&data.data()[9]);
    int32_t ch4 = CalculateTension(&data.data()[12]);

    m_latestFrame.channel1 = ConvertToVolt(ch1);
    m_latestFrame.channel2 = ConvertToVolt(ch2);
    m_latestFrame.channel3 = ConvertToVolt(ch3);
    m_latestFrame.channel4 = ConvertToVolt(ch4);
    m_latestFrame.timestamp = HAL_GetTick();

    return m_latestFrame;
}

/*****************************************************************************/
/* Private method definitions                                                */
/*****************************************************************************/

void AdsModule::Reset()
{
    CEP_ASSERT(m_config.pins.chipSelect.port != nullptr,
               "ADS's Chip Select GPIO Port is NULL!");
    CEP_ASSERT(m_config.pins.dataReady.port != nullptr,
               "ADS's Data Ready GPIO Port is NULL!");
    CEP_ASSERT(m_config.pins.done.port != nullptr,
               "ADS's Done GPIO Port is NULL!");
    CEP_ASSERT(m_config.pins.reset.port != nullptr,
               "ADS's Reset GPIO Port is NULL!");

    const auto &cs = m_config.pins.chipSelect;
    const auto &rst = m_config.pins.reset;

    HAL_GPIO_WritePin(cs.port, cs.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(rst.port, rst.pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(rst.port, rst.pin, GPIO_PIN_SET);
    HAL_Delay(5);
}

bool AdsModule::SendCommand(uint16_t cmd, uint16_t expectedResponse)
{
    uint8_t attempts = 0;
    do
    {
        if (attempts > AdsModule::MaxInitAttempts)
        {
            return false;
        }
        attempts++;

        Send(cep::swap(cmd));
    } while (ReadCommandResponse() != expectedResponse);

    return true;
}

/**
 * @brief   Configure an internal ADS131 register at a specified
 *          address with some data.
 *
 * @param   addr: Register address
 * @param   data:    Data to write in the register
 *
 * @return  true:    If data was written successfully
 * @return  false:   If the data couldn't be written in the register
 */
bool AdsModule::SendConfig(uint8_t addr, uint8_t data)
{
    /* Command : 0xAABB
     * A = address | 0x40
     * B = data
     */
    /* Response : 0xAABB
     * A = address | 0x20
     * B = data
     */
    /* Example : ADDRESS_ADC4 = 0x14
     *           Data = 0x01
     * Command  = 0x5401
     * Response = 0x3401
     */
    uint16_t response = ((addr | 0x20) << 8) | data;

    // Data is MSB in command because SPI sends LSB-first.
    uint16_t cmd = (ADS::Commands::WriteSingleRegisterMask
            | (uint16_t) addr << 8) | ((uint16_t) (data) & 0x00FF);

    return SendCommand(cmd, response);
}

uint16_t AdsModule::Send(uint16_t data)
{
    std::vector<uint8_t> pkt = std::vector<uint8_t>(6);
    pkt[0] = ((uint8_t*) &data)[0];
    pkt[1] = ((uint8_t*) &data)[1];

    // Create a vector containing 6 uint8_t set to 0.
    std::vector<uint8_t> resp = std::vector<uint8_t>(6);

    // Activate CS to start transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port,
                      m_config.pins.chipSelect.pin, GPIO_PIN_RESET);

    // Send a single 24-bits word (as 3 8-bits words).
    m_spi->Transaction(pkt, resp);

    // Release CS to end transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port,
                      m_config.pins.chipSelect.pin, GPIO_PIN_SET);

    return (cep::combine(&resp[0], 2));
}

uint16_t AdsModule::ReadCommandResponse()
{
    uint32_t response = 0;

    // Activate CS to start transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port,
                      m_config.pins.chipSelect.pin, GPIO_PIN_RESET);

    m_spi->Receive(reinterpret_cast<uint8_t*>(&response), 3);

    // Release CS to end transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port,
                      m_config.pins.chipSelect.pin, GPIO_PIN_SET);

    return (cep::swap((uint16_t) response));
}

int32_t AdsModule::CalculateTension(uint8_t *data)
{
    int32_t up = ((int32_t) data[0] << 24);
    int32_t mid = ((int32_t) data[1] << 16);
    int32_t bot = ((int32_t) data[2] << 8);

    // NOTE: This right-shift operation on signed data maintains the signed bit,
    // and provides for the sign-extension from 24 to 32 bits.
    return (((int32_t) (up | mid | bot)) >> 8);
}

float AdsModule::ConvertToVolt(int16_t val)
{
    // 1 LSB = (2 * Vref / Gain) / 2^24
    // #TODO Modify this to get the value depending on the ADS's config.
    constexpr float LSB = (2.0f * (2.442f / 1.0f)) / 16777216.0f;

    return ((float) val * LSB);
}
#endif
/* Have a wonderful day :) */
/**
 * @}
 */
/****** END OF FILE ******/
