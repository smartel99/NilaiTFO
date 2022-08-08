/**
 * @file    adc_module.cpp
 * @author  Samuel Martel
 * @date    2021/10/27
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
#include "adc_module.h"

#if defined(NILAI_USE_ADC) && defined(HAL_ADC_MODULE_ENABLED)
#    include "../defines/macros.h"
#    include "../defines/system.h"
#    include "../services/logger.h"
#    include "../services/time.h"

#    define ADC_DEBUG(msg, ...)   LOG_DEBUG("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADC_INFO(msg, ...)    LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADC_WARNING(msg, ...) LOG_WARNING("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADC_ERROR(msg, ...)   LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

#    if defined(NILAI_ADC_REGISTER_CALLBACKS)
#        define REGISTER_CALLBACK(cbId, cb)                                                        \
            do                                                                                     \
            {                                                                                      \
                HAL_StatusTypeDef s = HAL_ADC_RegisterCallback(m_adc, cbId, cb);                   \
                if (s != HAL_OK)                                                                   \
                {                                                                                  \
                    ADC_ERROR("Unable to register callback '%s': %i", #cb, s);                     \
                }                                                                                  \
            } while (0)
#    else
#        define REGISTER_CALLBACK(cbId, cb)
#    endif


namespace Nilai::Drivers
{
struct AdcPair
{
    ADC_TypeDef* Adc    = nullptr;
    AdcModule*   Module = nullptr;

    AdcPair(ADC_TypeDef* adc, AdcModule* module) : Adc(adc), Module(module) {}
};

static std::vector<AdcPair> s_modules = {};
static inline AdcModule*    FindModule(ADC_HandleTypeDef* adc)
{
    for (auto& [handle, module] : s_modules)
    {
        if (handle == adc->Instance)
        {
            return module;
        }
    }

    return nullptr;
}

static constexpr float ConvertToVolt(uint32_t val)
{
    //! Full scale = Resolution - 1 = 2^12 -1 = 4095.
    constexpr float FULL_SCALE = 4095.0f;
    //! VDDa = 3.3V (normally)
    constexpr float VDDA = 3.3f;
    return ((static_cast<float>(val) / FULL_SCALE) * VDDA);
}

// static std::map<ADC_HandleTypeDef*, AdcModule*> s_modules = {};

AdcModule::AdcModule(ADC_HandleTypeDef* adc, std::string label)
: m_adc(adc), m_label(std::move(label))
{
#    if !defined(NILAI_TEST)
    NILAI_ASSERT(adc != nullptr, "[%s]: ADC handle is null!", m_label.c_str());
    m_channelBuff.resize(adc->Init.NbrOfConversion);

    s_modules.emplace_back(m_adc->Instance, this);

#        if defined(NILAI_ADC_REGISTER_CALLBACKS)
    // Register conversion complete and error callbacks.
    REGISTER_CALLBACK(HAL_ADC_CONVERSION_COMPLETE_CB_ID, &AdcModule::AdcModuleConvCpltCallback);
    REGISTER_CALLBACK(HAL_ADC_ERROR_CB_ID, &AdcModule::AdcModuleErrorCallback);
#        endif

    Start();

    ADC_INFO("Initialized");
#    endif
}

AdcModule::~AdcModule()
{
    Stop();
}

/**
 * For the POST to pass, we must read a non-null value on every channels.
 * This assumes that enough time has elapsed since the initialization for conversions to have been
 * done on every channels.
 * @return true if the POST passed, false otherwise.
 */
bool AdcModule::DoPost()
{
    for (uint8_t attempt = 0; attempt < 5; attempt++)
    {
        bool isAllChannelsOk = true;
        Start();
        Delay(5);

        size_t ch = 1;
        for (const auto& value : m_channelBuff)
        {
            // A value of 0 (0 LSB counts) is assumed to be erroneous given its
            // unlikeliness.
            if (value == 0)
            {
                ADC_ERROR("Channel %i is reading 0 counts!", ch);
                isAllChannelsOk = false;
            }
            else
            {
                ADC_INFO("Channel %i is reading: %i", ch, value);
            }
            ++ch;
        }

        Stop();

        if (isAllChannelsOk)
        {
            ADC_INFO("POST OK");
            return true;
        }
        else
        {
            ADC_WARNING("Not all channels are ok, retrying... %i", attempt);
        }
    }
    return false;
}

void AdcModule::Run()
{
    if (m_lastError != 0)
    {
#    if defined(NILAI_ADC_STATUS_STRING)
        ADC_ERROR("An ADC error occurred (%#08x): %s", m_lastError, StatusToStr(m_lastError));
#    else
        ADC_ERROR("An error occurred: %#08x", m_lastError);
#    endif
        m_lastError = 0;
        Start();
    }
}

float AdcModule::GetChannelReading(size_t channel) const
{
    return ConvertToVolt(GetRawChannelReading(channel));
}

uint32_t AdcModule::GetRawChannelReading(size_t channel) const
{
    NILAI_ASSERT(channel < m_channelBuff.size(),
                 "[%s] Channel %i is not a valid channel!",
                 m_label.c_str(),
                 channel);
    return m_channelBuff[channel];
}

[[maybe_unused]] size_t AdcModule::AddConvCpltCallback(const std::function<void(AdcModule*)>& cb)
{
    NILAI_ASSERT(cb, "Callback function must be callable!");
    if (cb)
    {
        bool isRunning = m_isRunning;
        if (isRunning)
        {
            Stop();
        }
        m_convCpltCallbacks.push_back(cb);

        if (isRunning)
        {
            Start();
        }
#    if defined(NILAI_TEST)
        // For unit tests, instantly call it back.
        cb(this);
#    endif

        return m_convCpltCallbacks.size() - 1;
    }

    return -1;
}

[[maybe_unused]] size_t AdcModule::AddErrorCallback(const std::function<void(AdcModule*)>& cb)
{
    NILAI_ASSERT(cb, "Callback function must be callable!");

    if (cb)
    {
        bool isRunning = m_isRunning;
        if (isRunning)
        {
            Stop();
        }
        m_errorCallbacks.push_back(cb);
        if (isRunning)
        {
            Start();
        }

        return m_errorCallbacks.size() - 1;
    }
    return -1;
}

void AdcModule::Start()
{
    HAL_ADC_Start_DMA(m_adc, m_channelBuff.data(), m_channelBuff.size());
    m_isRunning = true;
}

void AdcModule::Stop()
{
    HAL_ADC_Stop_DMA(m_adc);
    m_isRunning = false;
}

void AdcModule::ConvCpltCallback()
{
    for (const auto& cb : m_convCpltCallbacks)
    {
        cb(this);
    }
}

void AdcModule::ErrorCallback()
{
    Stop();
    m_lastError = HAL_ADC_GetError(m_adc);
    for (const auto& cb : m_errorCallbacks)
    {
        cb(this);
    }
}

/**
 * @brief Callback invoked by the HAL once a full sequence of conversion is completed.
 * @param adc Handle to the hardware peripheral.
 */
#    if defined(NILAI_ADC_REGISTER_CALLBACKS)
void AdcModule::AdcModuleConvCpltCallback(ADC_HandleTypeDef* adc)
#    else
extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc)
#    endif
{
    AdcModule* module = FindModule(adc);
    NILAI_ASSERT(module != nullptr, "Module is null!");
    module->ConvCpltCallback();
}

/**
 * @brief Callback invoked by the HAL when an error is detected in the ADC peripheral.
 * @param adc Handle to the hardware peripheral.
 */
#    if defined(NILAI_ADC_REGISTER_CALLBACKS)
void AdcModule::AdcModuleErrorCallback(ADC_HandleTypeDef* adc)
#    else
extern "C" void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* adc)
#    endif
{
    AdcModule* module = FindModule(adc);
    if (module != nullptr)
    {
        module->ErrorCallback();
    }
}

#    if defined(NILAI_ADC_STATUS_STRING)
constexpr const char* AdcModule::StatusToStr(uint32_t code)
{
    switch (code)
    {
        case (HAL_ADC_ERROR_INTERNAL): return "Internal Error";
        case (HAL_ADC_ERROR_OVR): return "Overrun";
        case (HAL_ADC_ERROR_DMA): return "DMA Error";
#        if defined(NILAI_ADC_REGISTER_CALLBACKS)
        case (HAL_ADC_ERROR_INVALID_CALLBACK): return "Invalid callback";
#        endif
        default: return "Unknown";
    }
}

#    endif
}    // namespace Nilai::Drivers
#endif
