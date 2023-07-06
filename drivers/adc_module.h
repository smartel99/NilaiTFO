/**
 * @file    adc_module.h
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
#ifndef NILAI_ADCMODULE_HPP
#define NILAI_ADCMODULE_HPP

#if defined(NILAI_USE_ADC)

#    include "../defines/internal_config.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_ADC_MODULE_ENABLED) || defined(NILAI_TEST)
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
     * @param cb The callback function. It must take an @c AdcModule* as input, and return nothing.
     */
    [[maybe_unused]] size_t AddConvCpltCallback(const std::function<void(AdcModule*)>& cb);

    /**
     * @brief Adds a callback that willed be invoked whenever an error is detected by the ADC.
     * @param cb The callback function. It must take an @c AdcModule* as input, and return nothing.
     */
    [[maybe_unused]] size_t AddErrorCallback(const std::function<void(AdcModule*)>& cb);

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
    [[nodiscard]] float    GetChannelReading(size_t channel) const;
    [[nodiscard]] uint32_t GetRawChannelReading(size_t channel) const;

    virtual void ConvCpltCallback();
    virtual void ErrorCallback();

private:
#        if defined(NILAI_ADC_REGISTER_CALLBACKS)
    static void AdcModuleConvCpltCallback(ADC_HandleTypeDef* adc);
    static void AdcModuleErrorCallback(ADC_HandleTypeDef* adc);
#        endif

#        if defined(NILAI_ADC_STATUS_STRING)
    static constexpr const char* StatusToStr(uint32_t code);
#        endif

protected:
    ADC_HandleTypeDef*    m_adc         = nullptr;
    std::vector<uint32_t> m_channelBuff = {};
    std::string           m_label;
    bool                  m_isRunning = false;

    uint32_t m_lastError = 0;

    std::vector<std::function<void(AdcModule*)>> m_convCpltCallbacks;
    std::vector<std::function<void(AdcModule*)>> m_errorCallbacks;
};
}    // namespace Nilai::Drivers
#    else
#        if WARN_MISSING_STM_DRIVERS
static_assert(false, "NilaiTFO ADC module is enabled, but HAL_ADC_MODULE_ENABLED is undefined !");
#        endif
#    endif

#endif
#endif
