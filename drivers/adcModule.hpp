#pragma once

#if defined(NILAI_USE_ADC)
#include "defines/internalConfig.h"
#include NILAI_HAL_HEADER
#if defined(HAL_ADC_MODULE_ENABLED)
#include "Core/Inc/adc.h"
#include "shared/defines/module.hpp"

class AdcModule : public cep::Module
{
public:
    AdcModule(ADC_HandleTypeDef* adc, const std::string& label);
    virtual ~AdcModule() override;

    virtual bool               DoPost() override;
    virtual void               Run() override {}
    virtual const std::string& GetLabel() const override { return m_label; }

    float GetChannelReading(size_t channel) const;

private:
    ADC_HandleTypeDef* m_adc          = nullptr;
    uint32_t*          m_channelBuff  = nullptr;
    size_t             m_channelCount = 0;
    std::string        m_label        = "";
};
#else
#if WARN_MISSING_STM_DRIVERS
#warning NilaiTFO ADC module is enabled, but HAL_ADC_MODULE_ENABLED is undefined!
#endif
#endif
#endif
