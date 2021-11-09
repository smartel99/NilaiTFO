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
#ifndef GUARD_ADCMODULE_HPP
#define GUARD_ADCMODULE_HPP

#if defined(NILAI_USE_ADC)
#    include "defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_ADC_MODULE_ENABLED)
#        if !defined(NILAI_TEST)
#            include "Core/Inc/adc.h"
#        else
#            include "test/Mocks/adc.h"
#        endif
#        include "shared/defines/module.hpp"

#        include <functional>
#        include <map>

class AdcModule : public cep::Module
{
public:
    AdcModule(ADC_HandleTypeDef* adc, std::string label);
    ~AdcModule() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    [[maybe_unused]] void AddConvCpltCallback(const std::string&                     name,
                                              const std::function<void(AdcModule*)>& cb);
    [[maybe_unused]] void AddErrorCallback(const std::string&                     name,
                                           const std::function<void(AdcModule*)>& cb);

    [[maybe_unused]] void RemoveCallback(const std::string& name);

    void Start();
    void Stop();

    [[nodiscard]] float GetChannelReading(size_t channel) const;

private:
    void ConvCpltCallback();
    void ErrorCallback();


    static void AdcModuleConvCpltCallback(ADC_HandleTypeDef* adc);
    static void AdcModuleErrorCallback(ADC_HandleTypeDef* adc);

private:
    ADC_HandleTypeDef* m_adc          = nullptr;
    uint32_t*          m_channelBuff  = nullptr;
    size_t             m_channelCount = 0;
    std::string        m_label;
    bool               m_isRunning = false;

    std::map<std::string, std::function<void(AdcModule*)>> m_convCpltCallbacks;
    std::map<std::string, std::function<void(AdcModule*)>> m_errorCallbacks;

private:
    static std::map<ADC_HandleTypeDef*, AdcModule*> s_modules;
};
#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO ADC module is enabled, but HAL_ADC_MODULE_ENABLED is undefined!
#        endif
#    endif
#endif
#endif
