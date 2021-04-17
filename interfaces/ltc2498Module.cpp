/**
 * @addtogroup ltc2498Module.cpp
 * @{
 *******************************************************************************
 * @file	ltc2498Module.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#include "ltc2498Module.h"

#if defined(NILAI_USE_LTC2498) && defined(NILAI_USE_SPI)
#include "services/logger.hpp"

#define LTC_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#define LTC_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

namespace LTC2498
{
std::array<uint8_t, 4> ConversionSettings::ToRegValues() const
{
    std::array<uint8_t, 4> data = {0};

    data[0] |= 0xA0;    // PRE_EN1: 1, PRE_EN2: 0, EN: 1
    data[0] |= (uint8_t)(type == AcquisitionTypes::SingleEnded ? 0x10 : 0x00);    // SGL.
    data[0] |= (uint8_t)(polarity == Polarities::Negative ? 0x08 : 0x00);         // ODD
    data[0] |= (uint8_t)channel;                                                // ODD, A2, A1, A0.
    data[1] |= 0x80;                                                            // EN2: 1.
    data[1] |= (uint8_t)(inputType == InputTypes::TempSensor ? 0x40 : 0x00);    // IM.
    data[1] |=
      (uint8_t)(((filters == Filters::Hz50) || (filters == Filters::All)) ? 0x00 : 0x20);    // FA.
    data[1] |=
      (uint8_t)(((filters == Filters::Hz60) || (filters == Filters::All)) ? 0x00 : 0x10);    // FB.
    data[1] |= (uint8_t)(speed == Speeds::x2 ? 0x08 : 0x00);                                 // SPD.

    return data;
}
}    // namespace LTC2498

Ltc2498Module::Ltc2498Module(const std::string& label,
                             SpiModule*         spi,
                             const Pin&         inPin,
                             const Pin&         csPin)
: m_label(label), m_spi(spi), m_misoPin(inPin), m_csPin(csPin)
{
    // Enable the LTC2498 chip select signal to monitor its status.
    m_csPin.Set(false);

    // Configure the MISO pin as a GPIO input to monitor the LTC's status.
    SetMisoAsGpio();

    LTC_INFO("Initialized");
}

/**
 * To pass the POST, we must be able to read the internal temperature sensor of the LTC2498 ADC.
 * This temperature must be within a normal temperature range (-30C, 243K to +60C, 333k)
 * @return
 */
bool Ltc2498Module::DoPost()
{
    LTC2498::ConversionSettings config;
    config.channel   = LTC2498::Channels::CH0;
    config.type      = LTC2498::AcquisitionTypes::SingleEnded;
    config.polarity  = LTC2498::Polarities::Positive;
    config.inputType = LTC2498::InputTypes::TempSensor;
    config.filters   = LTC2498::Filters::All;
    config.speed     = LTC2498::Speeds::x1;
    config.callback  = {};

    if (StartConversion(config) == false)
    {
        LTC_ERROR("Error in POST: Unable to start conversion!");
        return false;
    }

    size_t start = HAL_GetTick();
    while (IsConversionInProgress() == true)
    {
        if (HAL_GetTick() >= start + 250)
        {
            // Give the ADC the time of 2 samples (~266.67ms) to complete the conversion.
            LTC_ERROR("Error in POST: Timed out while waiting for ADC!");
            return false;
        }
    }

    const LTC2498::Reading& reading = GetLastReading();
    // Calculate the temperature (in kelvins) from the reading.
    float temp = (float)(reading.raw) / 314.0f;

    if ((temp >= 243.15f) && (temp <= 333.15f))
    {
        LTC_INFO("POST OK");
        return true;
    }
    else
    {
        LTC_ERROR("Error in POST: Invalid temperature read: %0.3f", temp);
        return false;
    }
}

void Ltc2498Module::Run()
{
    if (m_isConverting == false)
    {
        // No conversions currently on the way, no need to do anything.
        return;
    }

    if (m_misoPin.Get() == false)
    {
        // MISO pin == 0 -> Conversion is complete!
        SetMisoAsMiso();

        std::array<uint8_t, 4> nextConversion = {0};
        // Check if we have a conversion queued up.
        int pos = GetNextConversion();
        if (pos != -1)
        {
            nextConversion = m_conversions[pos].ToRegValues();

            // Pre-saves the conversion settings.
            m_lastReading.config = m_conversions[pos];
        }

        std::array<uint8_t, 4> resp = SetNextConvAndReadResults(nextConversion);

        ParseConversionResult(resp);
        SetMisoAsGpio();
    }
}

bool Ltc2498Module::QueueConversions(const std::vector<LTC2498::ConversionSettings>& conversions,
                                     bool                                            repeat)
{
    m_conversions = conversions;
    m_repeat      = repeat;

    m_currentConversion = 0;

    // If there are no conversions in the vector:
    if (conversions.empty() == true)
    {
        // Don't start a conversion.
        return false;
    }
    else
    {
        SetNextConvAndReadResults(conversions[0].ToRegValues());
        return true;
    }
}

/**
 * If MISO is 1: Conversion in progress.
 * If MISO is 0: No conversion in progress.
 * @return
 */
bool Ltc2498Module::IsConversionInProgress() const
{
    return m_misoPin.Get();
}

bool Ltc2498Module::StartConversion(const LTC2498::ConversionSettings& config)
{
    return QueueConversions({config}, false);
}

void Ltc2498Module::SetMisoAsGpio()
{
    HAL_GPIO_DeInit(m_misoPin.port, m_misoPin.pin);

    GPIO_InitTypeDef GPIO_InitStruct = {};

    GPIO_InitStruct.Pin  = m_misoPin.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(m_misoPin.port, &GPIO_InitStruct);
}

void Ltc2498Module::SetMisoAsMiso()
{
    HAL_GPIO_DeInit(m_misoPin.port, m_misoPin.pin);

    GPIO_InitTypeDef GPIO_InitStruct = {};

    GPIO_InitStruct.Pin   = m_misoPin.pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // TODO Change GPIO_AF5_SPI1 for something generic that works with other SPI peripherals.
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(m_misoPin.port, &GPIO_InitStruct);
}

/**
 *
 * @return -1 if there's no queued conversion.
 */
int Ltc2498Module::GetNextConversion()
{
    if (m_conversions.empty())
    {
        // Conversion queue is empty.
        return -1;
    }
    else
    {
        if (m_currentConversion < m_conversions.size())
        {
            // Not at the end of the queue.
            int pos = (int)m_currentConversion;
            m_currentConversion++;
            return pos;
        }
        else if (m_repeat == true)
        {
            // At the end of the queue but we need to repeat it.
            m_currentConversion = 0;
            return m_currentConversion;
        }
        else
        {
            // At the end of the queue, no repeat.
            // Clear the queue.
            m_conversions.clear();
            return -1;
        }
    }
}

std::array<uint8_t, 4>
Ltc2498Module::SetNextConvAndReadResults(const std::array<uint8_t, 4>& config)
{
    std::array<uint8_t, 4> resp = {0};

    // Read the conversion result and (maybe) start the next one.
    if (m_spi->Transaction(config.data(), config.size(), resp.data(), resp.size()) !=
        CEP_SPI::Status::NONE)
    {
        LTC_ERROR("Unable to communicate with the ADC!");
    }

    // If all bytes of the configuration are 0, there's no conversion taking place.
    if (config[0] == 0 && config[1] == 0 && config[2] == 0 && config[3] == 0)
    {
        m_isConverting = false;
    }
    else
    {
        m_isConverting = true;
    }

    return resp;
}

void Ltc2498Module::ParseConversionResult(const std::array<uint8_t, 4>& resp)
{
    // Convert into a single uint32_t for QoL.
    uint32_t raw = ((uint32_t)resp[0] << 24) | ((uint32_t)resp[1] << 16) |
                   ((uint32_t)resp[2] << 8) | ((uint32_t)resp[3]);

    // Bit 31 is End of Conversion flag, should obviously be set to 0.
    if ((raw & 0x8000) == 1)
    {
        LTC_ERROR("End of conversion flag is not 0!\t0x%08X", raw);
        return;
    }

    // Bit 30 is a dummy bit, should always be 0.
    if ((raw & 0x4000) == 1)
    {
        LTC_ERROR("Dummy bit is not 0!\t0x%08X", raw);
        return;
    }

    // Bit 29 is the sign of the result.
    int sign = ((raw & 0x2000) ? 1 : -1);

    // Bit 28 to 5 is the conversion result.
    raw = (raw & 0x1FFFFFE0) >> 5;

    // Apply the sign.
    raw *= sign;

    m_lastReading.raw = raw;

    // Convert the raw reading into volts.
    m_lastReading.reading = (float)raw * (5.0f / 16777216.0f);
}

#endif
