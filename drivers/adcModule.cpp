#include "adcModule.hpp"
#if defined(NILAI_USE_ADC) && defined(HAL_ADC_MODULE_ENABLED)

AdcModule::AdcModule(ADC_TypeDef* adc, const std::string& label) : m_adc(adc), m_label(label) {}

#endif
