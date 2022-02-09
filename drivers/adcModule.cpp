#include "adcModule.hpp"

#if defined(NILAI_USE_ADC) && defined(HAL_ADC_MODULE_ENABLED)
#    include "defines/macros.hpp"
#    include "services/logger.hpp"

#    define ADC_DEBUG(msg, ...)   LOG_DEBUG("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADC_INFO(msg, ...)    LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADC_WARNING(msg, ...) LOG_WARNING("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ADC_ERROR(msg, ...)   LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

#    define REGISTER_CALLBACK(cbId, cb)                                                            \
        do                                                                                         \
        {                                                                                          \
            HAL_StatusTypeDef s = HAL_ADC_RegisterCallback(m_adc, cbId, cb);                       \
            if (s != HAL_OK)                                                                       \
            {                                                                                      \
                ADC_ERROR("Unable to register callback '%s': %i", #cb, s);                         \
            }                                                                                      \
        } while (0)


static constexpr float ConvertToVolt(uint32_t val)
{
    return ((static_cast<float>(val) / 4095.0f) * 3.3f);
}

std::map<ADC_HandleTypeDef*, AdcModule*> AdcModule::s_modules = {};

AdcModule::AdcModule(ADC_HandleTypeDef* adc, std::string label)
: m_adc(adc), m_label(std::move(label))
{
#    if !defined(NILAI_TEST)
    CEP_ASSERT(adc != nullptr, "[%s]: ADC handle is null!", m_label.c_str());
    m_channelCount = adc->Init.NbrOfConversion;
    m_channelBuff  = new uint32_t[m_channelCount];

    CEP_ASSERT(m_channelBuff != nullptr,
               "[%s]: Unable to allocate memory for channel data buffer!",
               m_label.c_str());

    s_modules[adc] = this;
#        if USE_HAL_ADC_REGISTER_CALLBACKS == 1
    // Register conversion complete and error callbacks.
    REGISTER_CALLBACK(HAL_ADC_CONVERSION_COMPLETE_CB_ID, &AdcModule::AdcModuleConvCpltCallback);
    REGISTER_CALLBACK(HAL_ADC_ERROR_CB_ID, &AdcModule::AdcModuleErrorCallback);
#        else
#            warning HAL is not configured to use ADC callbacks, using default handler.
#        endif

    Start();

    ADC_INFO("Initialized");
#    endif
}

AdcModule::~AdcModule()
{
    Stop();
    delete[] m_channelBuff;
    m_channelBuff = nullptr;
}

/**
 * For the POST to pass, we must read a non-null value on every channels.
 * This assumes that enough time has elapsed since the initialization for conversions to have been
 * done on every channels.
 * @return true if the POST passed, false otherwise.
 */
bool AdcModule::DoPost()
{
    if (m_channelBuff == nullptr)
    {
        ADC_ERROR("Error in POST: m_channelBuff is null!");
        return false;
    }

    for (uint8_t attempt = 0; attempt < 5; attempt++)
    {
        bool isAllChannelsOk = true;
        Start();
        HAL_Delay(5);
        for (size_t i = 0; i < m_channelCount; i++)
        {
            // A value of 0 (0 LSB counts) is assumed to be erroneous given its
            // unlikeliness.
            if (m_channelBuff[i] == 0)
            {
                ADC_ERROR("Channel %i is reading 0 counts!", i);
                isAllChannelsOk = false;
            }
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
}

float AdcModule::GetChannelReading(size_t channel) const
{
    CEP_ASSERT(channel < m_channelCount,
               "[%s] Channel %i is not a valid channel!",
               m_label.c_str(),
               channel);

    return ConvertToVolt(m_channelBuff[channel]);
}

/**
 * @brief Adds a callback that will be invoked when the sequence of conversion is completed.
 * @param name The name of the callback. This is used to identify it and to be able to remove it at
 * a later point.
 * @param cb The callback function.
 */
[[maybe_unused]] void AdcModule::AddConvCpltCallback(const std::string&                     name,
                                                     const std::function<void(AdcModule*)>& cb)
{
    CEP_ASSERT(cb, "Callback function must be callable!");
    //    ADC_DEBUG("Adding conversion complete callback '%s'", name.c_str());

    bool isRunning = m_isRunning;
    if (isRunning)
    {
        Stop();
    }
    m_convCpltCallbacks[name] = cb;

    if (isRunning)
    {
        Start();
    }
#    if defined(NILAI_TEST)
    // For unit tests, instantly call it back.
    cb(this);
#    endif
}

/**
 * @brief Adds a callback that will be invoked when an error is detected by the hardware.
 * @param name The name of the callback. This is used to identify it and to be able to remove it at
 * a later point.
 * @param cb The callback function.
 */
[[maybe_unused]] void AdcModule::AddErrorCallback(const std::string&                     name,
                                                  const std::function<void(AdcModule*)>& cb)
{
    CEP_ASSERT(cb, "Callback function must be callable!");
    //    ADC_DEBUG("Adding error callback '%s'", name.c_str());

    bool isRunning = m_isRunning;
    if (isRunning)
    {
        Stop();
    }
    m_errorCallbacks[name] = cb;
    if (isRunning)
    {
        Start();
    }
}

/**
 * @brief Removes a callback that was previously assigned to the module.
 * @param name The name of the callback to remove.
 *
 * @note If the callback @ref name isn't found in the module, nothing happens.
 */
[[maybe_unused]] void AdcModule::RemoveCallback(const std::string& name)
{
    bool isRunning = m_isRunning;
    if (isRunning)
    {
        Stop();
    }

    //    ADC_DEBUG("Removing callback '%s'", name.c_str());

    // Search in the list of callbacks for the desired one.
    auto it = m_convCpltCallbacks.find(name);
    if (it != m_convCpltCallbacks.end())
    {
        // Found it in conversion complete!
        m_convCpltCallbacks.erase(it);
    }
    else
    {
        it = m_errorCallbacks.find(name);
        if (it != m_errorCallbacks.end())
        {
            // Found it in error callbacks!
            m_errorCallbacks.erase(it);
        }
        else
        {
            ADC_WARNING("Unable to find a callback called '%s'!", name.c_str());
        }
    }

    if (isRunning)
    {
        Start();
    }
}

void AdcModule::Start()
{
    HAL_ADC_Start_DMA(m_adc, &m_channelBuff[0], m_channelCount);
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
        cb.second(this);
    }
}

void AdcModule::ErrorCallback()
{
    // TODO Print more information than that.
    for (const auto& cb : m_errorCallbacks)
    {
        cb.second(this);
    }
}

#    if USE_HAL_ADC_REGISTER_CALLBACKS == 1
/**
 * @brief Callback invoked by the HAL once a full sequence of conversion is completed.
 * @param adc Handle to the hardware peripheral.
 */
void AdcModule::AdcModuleConvCpltCallback(ADC_HandleTypeDef* adc)
{
    auto module = s_modules.find(adc);
    if (module != s_modules.end())
    {
        module->second->ConvCpltCallback();
    }
}

/**
 * @brief Callback invoked by the HAL when an error is detected in the ADC peripheral.
 * @param adc Handle to the hardware peripheral.
 */
void AdcModule::AdcModuleErrorCallback(ADC_HandleTypeDef* adc)
{
    auto module = s_modules.find(adc);
    if (module != s_modules.end())
    {
        module->second->ErrorCallback();
    }
}
#    else
void AdcModule::AdcModuleConvCpltCallback(ADC_HandleTypeDef*)
{
}
void AdcModule::AdcModuleErrorCallback(ADC_HandleTypeDef*)
{
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc)
{
    auto module = s_modules.find(adc);
    if (module != s_modules.end())
    {
        module->second->ErrorCallback();
    }
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* adc)
{
    auto module = s_modules.find(adc);
    if (module != s_modules.end())
    {
        module->second->ErrorCallback();
    }
}
#    endif


#endif
