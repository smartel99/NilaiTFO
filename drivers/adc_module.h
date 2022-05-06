/**
******************************************************************************
* @file    adcModule.hpp
* @author  Samuel Martel
* @brief   Header for the on-board ADC module.
*
* @date 2021/10/27
*
******************************************************************************
*/
#ifndef NILAI_ADCMODULE_HPP
#define NILAI_ADCMODULE_HPP

#if defined(NILAI_USE_ADC)
#    include "../defines/internal_config.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_ADC_MODULE_ENABLED)
#        include "../defines/module.h"

#        include <functional>
#        include <map>
#        include <string>

namespace Nilai::Drivers
{
#        if USE_HAL_ADC_REGISTER_CALLBACKS == 1
#            define NILAI_ADC_REGISTER_CALLBACKS
#        endif

/**
 * @brief Controls a hardware ADC on the STM32.
 */
class AdcModule : public Nilai::Module
{
public:
    AdcModule(ADC_HandleTypeDef* adc, std::string label);
    ~AdcModule() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    /**
     * @brief Adds a callback that willed be invoked whenever the conversion of all channel is
     * completed.
     * @param name The name to assign to the callback. This name is used to keep track of the
     * callback.
     * @param cb The callback function. It must take an @c AdcModule* as input, and return nothing.
     */
    [[maybe_unused]] void AddConvCpltCallback(const std::string&                     name,
                                              const std::function<void(AdcModule*)>& cb);

    /**
     * @brief Adds a callback that willed be invoked whenever an error is detected by the ADC.
     * @param name The name to assign to the callback. This name is used to keep track of the
     * callback.
     * @param cb The callback function. It must take an @c AdcModule* as input, and return nothing.
     */
    [[maybe_unused]] void AddErrorCallback(const std::string&                     name,
                                           const std::function<void(AdcModule*)>& cb);

    /**
     * @brief Removes a callback called @c name from the module.
     * @param name The name of the callback to remove.
     *
     * @attention Do not remove a callback from within a callback!
     */
    [[maybe_unused]] void RemoveCallback(const std::string& name);

    /**
     * @brief Starts the continuous conversion on all channels.
     *
     * This is only stopped by calling @b AdcModule::Stop
     */
    void Start();

    /**
     * @brief Stops the ADC, terminating all updates of channels.
     */
    void Stop();

    /**
     * @brief Gets the tension, in volt, measured by the ADC at the desired channel.
     * @param channel The channel to read.
     * @return The value read on that channel.
     *
     * @attention The range of @b channel must be within 0 and the number of channels of the ADC.
     * Specifying a @b channel over the number of channels will result undefined behavior.
     */
    [[nodiscard]] float GetChannelReading(size_t channel) const;

    void ConvCpltCallback();
    void ErrorCallback();

private:
#        if defined(NILAI_ADC_REGISTER_CALLBACKS)
    static void AdcModuleConvCpltCallback(ADC_HandleTypeDef* adc);
    static void AdcModuleErrorCallback(ADC_HandleTypeDef* adc);
#        endif

private:
    ADC_HandleTypeDef* m_adc          = nullptr;
    uint32_t*          m_channelBuff  = nullptr;
    size_t             m_channelCount = 0;
    std::string        m_label;
    bool               m_isRunning = false;

    std::map<std::string, std::function<void(AdcModule*)>> m_convCpltCallbacks;
    std::map<std::string, std::function<void(AdcModule*)>> m_errorCallbacks;
};
}    // namespace Nilai::Drivers
#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO ADC module is enabled, but HAL_ADC_MODULE_ENABLED is undefined!
#        endif
#    endif
#elif defined(NILAI_TEST)
#    include "test/Mocks/AdcModule.h"
#endif
#endif
