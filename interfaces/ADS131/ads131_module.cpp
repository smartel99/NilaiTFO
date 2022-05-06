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
#include "ads131_module.h"
#include <cmath>
#if defined(NILAI_USE_ADS) && defined(NILAI_USE_SPI)
#    include "defines/bitManipulations.hpp"
#    include "defines/globaldef.h"
#    include "defines/macros.hpp"
#    include "services/logger.hpp"
#    include APPLICATION_HEADER

#    include <limits>

#    define ADS_INFO(msg, ...)  LOG_INFO("[%s] " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADS_ERROR(msg, ...) LOG_ERROR("[%s] " msg, m_label.c_str(), ##__VA_ARGS__)

AdsModule::AdsModule(SpiModule* spi, const std::string& label)
: m_spi(spi), m_label(label), m_config(ADS::Config())
{
    ADS_INFO("Initialized.");
}

bool AdsModule::DoPost()
{
    if (m_isConfigured == true)
    {
        ADS_INFO("POST OK");
        return true;
    }
    else
    {
        ADS_ERROR("ADS has not been configured!");
        return false;
    }
}

void AdsModule::Run()
{
    static bool wasTriggered = false;

    if (m_hasTriggered != wasTriggered)
    {
        wasTriggered = m_hasTriggered;
        if (m_hasTriggered == true)
        {
            ADS_INFO(
              "Sampling triggered: %0.6fV %s %0.6fV",
              m_channels[m_trigChannel][0],
              (m_trigEdge == TrigEdge::Alter ? "<>" : (m_trigEdge == TrigEdge::Rising ? ">" : "<")),
              m_trigLevel);
        }
        ADS_INFO("%s", (m_hasTriggered == true ? "Trigger level detected!" : "Trigger reset."));
    }
}

void AdsModule::Configure(const ADS::Config& config, bool force)
{
    // If the config we received is the one already in the ADS
    // and we don't want to forcefully reset that configuration:
    if ((config == m_config) && force == false)
    {
        // No need to do anything.
        return;
    }

    uint32_t startTime = HAL_GetTick();

    m_config       = config;
    m_active       = false;
    m_isConfigured = false;

    ADS_INFO("Configuring ADS...");
    Reset();

    // CMD -> 0x0011    Resp -> 0xFF04
    if (SendCommand(ADS::SysCommands::Reset, ADS::Acknowledges::Ready) == false)
    {
        ADS_ERROR("Unable to reset.");
        return;
    }

    // CMD -> 0x0655    Resp -> 0x0655
    if (SendCommand(ADS::SysCommands::Unlock, ADS::Acknowledges::Unlock) == false)
    {
        ADS_ERROR("Unable to unlock.");
        return;
    }

    ADS::Registers::ClockConfig1 clk1(m_config);
    if (SendConfig(ADS::Registers::ClockConfig1::Address, clk1.value) == false)
    {
        ADS_ERROR("Unable to setup Clock Config 1 Register.");
        return;
    }

    ADS::Registers::ClockConfig2 clk2(m_config);
    if (SendConfig(ADS::Registers::ClockConfig2::Address, clk2.value) == false)
    {
        ADS_ERROR("Unable to setup Clock Config 2 Register.");
        return;
    }

    ADS::Registers::DigitalSysConfig dsc(m_config);
    if (SendConfig(ADS::Registers::DigitalSysConfig::Address, dsc.value) == false)
    {
        ADS_ERROR("Unable to setup Digital System Configuration Register.");
        return;
    }

    ADS::Registers::AnalogSysConfig asc(m_config);
    if (SendConfig(ADS::Registers::AnalogSysConfig::Address, asc.value) == false)
    {
        ADS_ERROR("Unable to setup Analog System Configuration Register");
        return;
    }

    ADS::Registers::Ch1DigitalGain ch1(m_config);
    if (SendConfig(ADS::Registers::Ch1DigitalGain::Address, ch1.value) == false)
    {
        ADS_ERROR("Unable to setup Channel 1 Digital Gain.");
        return;
    }

    ADS::Registers::Ch2DigitalGain ch2(m_config);
    if (SendConfig(ADS::Registers::Ch2DigitalGain::Address, ch2.value) == false)
    {
        ADS_ERROR("Unable to setup Channel 2 Digital Gain.");
        return;
    }

    ADS::Registers::Ch3DigitalGain ch3(m_config);
    if (SendConfig(ADS::Registers::Ch3DigitalGain::Address, ch3.value) == false)
    {
        ADS_ERROR("Unable to setup Channel 3 Digital Gain.");
        return;
    }

    ADS::Registers::Ch4DigitalGain ch4(m_config);
    if (SendConfig(ADS::Registers::Ch4DigitalGain::Address, ch4.value) == false)
    {
        ADS_ERROR("Unable to setup Channel 4 Digital Gain.");
        return;
    }

    ADS::Registers::ChannelEnable en(m_config);
    if (SendConfig(ADS::Registers::ChannelEnable::Address, en.value) == false)
    {
        ADS_ERROR("Unable to setup Channel Enable register.");
        return;
    }

    if (SendCommand(ADS::SysCommands::Wakeup, ADS::Acknowledges::Wakeup) == false)
    {
        ADS_ERROR("Unable to wake ADS.");
        return;
    }

    if (SendCommand(ADS::SysCommands::Lock, ADS::Acknowledges::Lock) == false)
    {
        ADS_ERROR("Unable to lock ADS.");
        return;
    }

    ADS_INFO("Done! %i ms", HAL_GetTick() - startTime);
    m_isConfigured = true;
}

void AdsModule::Enable()
{
    if (m_active == false)
    {
        ADS_INFO("Starting scope");
        m_config.enable = ADS::Enable::Enable;
        Configure(m_config, true);
        m_active        = true;
        m_lastStartTime = HAL_GetTick();
    }
}

void AdsModule::Disable()
{
    if (m_active == true)
    {
        ADS_INFO("Stopping scope");
        const auto& cs  = m_config.pins.chipSelect;
        const auto& rst = m_config.pins.reset;

        HAL_GPIO_WritePin(cs.port, cs.pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(rst.port, rst.pin, GPIO_PIN_RESET);
        m_active = false;
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
        while (HAL_GPIO_ReadPin(m_config.pins.dataReady.port, m_config.pins.dataReady.pin) !=
               GPIO_PIN_RESET)
        {
            // If we time out:
            if (HAL_GetTick() >= timeoutTime)
            {
                // Don't update the cached values and bail out.
                return m_latestFrame;
            }
        }
    }

    uint8_t data[16];

    // Activate CS to start transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port, m_config.pins.chipSelect.pin, GPIO_PIN_RESET);
    m_spi->Receive(&data[0], SIZEOF_ARRAY(data));
    // Release CS to complete transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port, m_config.pins.chipSelect.pin, GPIO_PIN_SET);

    // First 3 bytes are for the status.
    // TODO: Maybe check status?
    // Each consecutive trio of bytes represent the data of a channel.
    float chs[4] = {CalculateTension(&data[3]),
                    CalculateTension(&data[6]),
                    CalculateTension(&data[9]),
                    CalculateTension(&data[12])};

    // Check for the trigger, if we haven't triggered yet.
    if (m_hasTriggered == true)
    {
        m_channels.channel1[m_samplesTaken] = chs[0];
        m_channels.channel2[m_samplesTaken] = chs[1];
        m_channels.channel3[m_samplesTaken] = chs[2];
        m_channels.channel4[m_samplesTaken] = chs[3];
        ++m_samplesTaken;
    }
    else
    {
        static float lastRead = std::numeric_limits<float>::lowest();
        if (lastRead == std::numeric_limits<float>::lowest())
        {
            lastRead = chs[m_trigChannel];
        }
        if (m_trigEdge == TrigEdge::Alter)
        {
            // If the tension we're measuring now is different from the last reading, triggered.
            if ((chs[m_trigChannel] > lastRead) || (chs[m_trigChannel] < lastRead))
            {
                m_hasTriggered = true;
            }
        }
        else if (m_trigEdge == TrigEdge::Rising)
        {
            // If the tension we're measuring now is higher than the last reading, triggered.
            if (chs[m_trigChannel] > lastRead)
            {
                m_hasTriggered = true;
            }
        }
        else if (m_trigEdge == TrigEdge::Falling)
        {
            // If the tension we're measuring now is lower than the last reading, triggered.
            if (chs[m_trigChannel] < lastRead)
            {
                m_hasTriggered = true;
            }
        }

        m_channels.channel1[0] = chs[0];
        m_channels.channel2[0] = chs[1];
        m_channels.channel3[0] = chs[2];
        m_channels.channel4[0] = chs[3];
    }

    // If we're at the end of our buffers:
    if (m_samplesTaken >= m_samplesToTake)
    {
        m_hasTriggered = false;
        UpdateLatestFrame();
        if (m_repeat == false)
        {
            Disable();
        }
        if (m_callback)
        {
            m_callback(m_latestFrame);
        }
    }

    return m_latestFrame;
}

/*****************************************************************************/
/* Private method definitions                                                */
/*****************************************************************************/

void AdsModule::Reset()
{
    CEP_ASSERT(m_config.pins.chipSelect.port != nullptr, "ADS's Chip Select GPIO Port is NULL!");
    CEP_ASSERT(m_config.pins.dataReady.port != nullptr, "ADS's Data Ready GPIO Port is NULL!");
    CEP_ASSERT(m_config.pins.done.port != nullptr, "ADS's Done GPIO Port is NULL!");
    CEP_ASSERT(m_config.pins.reset.port != nullptr, "ADS's Reset GPIO Port is NULL!");

    const auto& cs  = m_config.pins.chipSelect;
    const auto& rst = m_config.pins.reset;

    HAL_GPIO_WritePin(cs.port, cs.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(rst.port, rst.pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(rst.port, rst.pin, GPIO_PIN_SET);
    HAL_Delay(5);
}

bool AdsModule::SendCommand(uint16_t cmd, uint16_t expectedResponse)
{
    uint8_t  attempts = 0;
    uint16_t resp     = 0;
    do
    {
        if (attempts > AdsModule::MaxInitAttempts)
        {
            return false;
        }
        attempts++;

        Send(Nilai::swap(cmd));
        resp = ReadCommandResponse();
        if (resp == expectedResponse)
        {
            break;
        }
        else
        {
            LOG_DEBUG("[ADS] Expected: 0x%04X, Resp: 0x%04X", expectedResponse, resp);
        }
    } while (resp != expectedResponse);

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
    uint16_t cmd =
      (ADS::Commands::WriteSingleRegisterMask | (uint16_t)addr << 8) | ((uint16_t)(data)&0x00FF);

    return SendCommand(cmd, response);
}

uint16_t AdsModule::Send(uint16_t data)
{
    std::vector<uint8_t> pkt = std::vector<uint8_t>(6);
    pkt[0]                   = ((uint8_t*)&data)[0];
    pkt[1]                   = ((uint8_t*)&data)[1];

    // Create a vector containing 6 uint8_t set to 0.
    std::vector<uint8_t> resp = std::vector<uint8_t>(6);

    // Activate CS to start transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port, m_config.pins.chipSelect.pin, GPIO_PIN_RESET);

    // Send a single 24-bits word (as 3 8-bits words).
    m_spi->Transaction(pkt, resp);

    // Release CS to end transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port, m_config.pins.chipSelect.pin, GPIO_PIN_SET);

    return (Nilai::combine(&resp[0], 2));
}

uint16_t AdsModule::ReadCommandResponse()
{
    uint32_t response = 0;

    // Activate CS to start transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port, m_config.pins.chipSelect.pin, GPIO_PIN_RESET);

    m_spi->Receive(reinterpret_cast<uint8_t*>(&response), 3);

    // Release CS to end transaction.
    HAL_GPIO_WritePin(m_config.pins.chipSelect.port, m_config.pins.chipSelect.pin, GPIO_PIN_SET);

    return (Nilai::swap((uint16_t)response));
}

float AdsModule::CalculateTension(uint8_t* data)
{
    int32_t up  = ((int32_t)data[0] << 24);
    int32_t mid = ((int32_t)data[1] << 16);
    int32_t bot = ((int32_t)data[2] << 8);

    // NOTE: This right-shift operation on signed data maintains the signed bit,
    // and provides for the sign-extension from 24 to 32 bits.
    return ConvertToVolt(((int32_t)(up | mid | bot)) >> 8);
}

float AdsModule::ConvertToVolt(int32_t val)
{
    // 1 LSB = (2 * Vref / Gain) / 2^24
    // #TODO Modify this to get the value depending on the ADS's config.
    constexpr float LSB = (2.0f * (2.442f / 1.0f)) / 16777216.0f;

    //    // The 0.02656 value is a negative offset added by the ADS131, which we don't want.
    return (((float)val * LSB) + 0.02656f);
}

uint32_t AdsModule::ConvertToHex(float val)
{
    // 1 LSB = (2 * Vref / Gain) / 2^24
    // #TODO Modify this to get the value depending on the ADS's config.
    constexpr float LSB = (2.0f * (2.442f / 1.0f)) / 16777216.0f;

    uint32_t h = 0;
    if (val < 0.0f)
    {
        val *= -1;
        h = (uint32_t)(val / LSB) & 0x007FFFFFF;
        h |= 0x00800000;
    }
    else
    {
        h = (uint32_t)(val / LSB) & 0x007FFFFFF;
    }
    return h;
}

const std::vector<float>& AdsModule::GetChannel(size_t channel) const
{
    CEP_ASSERT((channel < 4), "Invalid channel requested");
    return m_channels[channel];
}

void AdsModule::UpdateLatestFrame()
{
    float tot1 = 0, tot2 = 0, tot3 = 0, tot4 = 0;
    float min1 = std::numeric_limits<float>::max(), min2 = min1, min3 = min1, min4 = min1;
    float max1 = std::numeric_limits<float>::lowest(), max2 = max1, max3 = max1, max4 = max1;
    // Ignore the first half of the samples taken.
    int cnt = 0;
    for (size_t i = m_samplesToIgnore; i < m_channels.size(); i++)
    {
        min1 = std::min(min1, m_channels.channel1[i]);
        min2 = std::min(min2, m_channels.channel2[i]);
        min3 = std::min(min3, m_channels.channel3[i]);
        min4 = std::min(min4, m_channels.channel4[i]);

        max1 = std::max(max1, m_channels.channel1[i]);
        max2 = std::max(max2, m_channels.channel2[i]);
        max3 = std::max(max3, m_channels.channel3[i]);
        max4 = std::max(max4, m_channels.channel4[i]);

        tot1 += m_channels.channel1[i];
        tot2 += m_channels.channel2[i];
        tot3 += m_channels.channel3[i];
        tot4 += m_channels.channel4[i];

        cnt++;
    }

    tot1 /= cnt;
    tot2 /= cnt;
    tot3 /= cnt;
    tot4 /= cnt;

    m_latestFrame.avgChannel1 = (tot1);
    m_latestFrame.avgChannel2 = (tot2);
    m_latestFrame.avgChannel3 = (tot3);
    m_latestFrame.avgChannel4 = (tot4);

    m_latestFrame.minChannel1 = (min1);
    m_latestFrame.minChannel2 = (min2);
    m_latestFrame.minChannel3 = (min3);
    m_latestFrame.minChannel4 = (min4);

    m_latestFrame.maxChannel1 = (max1);
    m_latestFrame.maxChannel2 = (max2);
    m_latestFrame.maxChannel3 = (max3);
    m_latestFrame.maxChannel4 = (max4);
    m_latestFrame.timestamp   = HAL_GetTick();

    // Clear all of the buffers for the next run.
    m_samplesTaken = 0;

// Debug stuff.
#    if 0
    LOG_INFO("[ADS] (%i milliseconds)"
             "\n\r\tCH1: 0x%06X (%0.3f) min: %0.3f\tmax: %0.3f"
             "\n\r\tCH2: 0x%06X (%0.3f) min: %0.3f\tmax: %0.3f"
             "\n\r\tCH3: 0x%06X (%0.3f) min: %0.3f\tmax: %0.3f"
             "\n\r\tCH4: 0x%06X (%0.3f) min: %0.3f\tmax: %0.3f",
             HAL_GetTick( ) - m_lastStartTime,
             m_latestFrame.avgChannel1,
             tot1,
             min1,
             max1,
             m_latestFrame.avgChannel2,
             tot2,
             min2,
             max2,
             m_latestFrame.avgChannel3,
             tot3,
             min3,
             max3,
             m_latestFrame.avgChannel4,
             tot4,
             min4,
             max4);
#    endif
    m_lastStartTime = HAL_GetTick();
}
#endif
/* Have a wonderful day :) */
/**
 * @}
 */
/****** END OF FILE ******/
