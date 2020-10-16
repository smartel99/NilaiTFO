#pragma once

#if defined(NILAI_USE_ADC)
#    include "stm32f4xx_hal.h"
#    if defined(HAL_ADC_MODULE_ENABLED)
#        include "Core/Inc/adc.h"
#        include "shared/defines/module.hpp"

class AdcModule : cep::Module
{
public:
    AdcModule(ADC_TypeDef* adc, const std::string& label);
    ~AdcModule( ) = default;

    virtual void               Run( ) override {}
    virtual const std::string& GetLabel( ) const override { return m_label; }

    uint16_t analogRead(ADC_HandleTypeDef* adcHandle, uint32_t channel);

private:
    ADC_TypeDef* m_adc   = nullptr;
    std::string  m_label = "";
};
#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO ADC module is enabled, but HAL_ADC_MODULE_ENABLED is undefined!
#        endif
#    endif
#endif
