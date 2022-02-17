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
    if (m_isConfigured)
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
        if (m_hasTriggered)
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
    if ((config == m_config) && !force)
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

    ADS_INFO("Done! %i ms", HAL_GetTick() - startTime);
    m_isConfigured = true;
}

void AdsModule::Enable()
{
    if (!m_active)
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
    if (m_active)
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
    if (!m_active) return m_latestFrame;

    uint8_t data[16];
    std::copy_n(data, 16, m_dataRx.front().begin());
    m_dataRx.pop();

    // First 3 bytes are for the status.
    // TODO: Maybe check status?
    // Each consecutive trio of bytes represent the data of a channel.
    float chs[4] = {CalculateTension(&data[3]),
                    CalculateTension(&data[6]),
                    CalculateTension(&data[9]),
                    CalculateTension(&data[12])};

    // Check for the trigger, if we haven't triggered yet.
    if (m_hasTriggered)
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
        if (!m_repeat)
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
}

bool AdsModule::SendCommand(uint16_t cmd, uint16_t expectedResponse)
{
    return true;
}

bool AdsModule::SendConfig(uint8_t addr, uint8_t data)
{
    return true;
}

uint16_t AdsModule::Send(uint16_t data)
{
    return 0;
}

uint16_t AdsModule::ReadCommandResponse()
{
    return 0;
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
