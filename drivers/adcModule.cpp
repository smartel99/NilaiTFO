#include "adcModule.hpp"
#if defined(NILAI_USE_ADC) && defined(HAL_ADC_MODULE_ENABLED)

AdcModule::AdcModule(ADC_TypeDef* adc, const std::string& label) : m_adc(adc), m_label(label)
{
    CEP_ASSERT(adc != nullptr, "ADC is null!");

    // Set prescaler to PCLK/4:
    m_adc->CCR |= ADC_CCR_ADCPRE_0;

    // Enable Temperature sensor and VRefInt:
    m_adc->CCR |= ADC_CCR_TSVREFE;
}

#endif
