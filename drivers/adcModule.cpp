#include "adcModule.hpp"
#if defined(NILAI_USE_ADC) && defined(HAL_ADC_MODULE_ENABLED)

#include "adc.h"
#include "main.h"


adcModule::adcModule()
{
  HAL_ADC_Stop(&hadc1);
  ADC_ChannelConfTypeDef yo = {ADC_CHANNEL_2, 1, ADC_SAMPLETIME_480CYCLES, 0};
  HAL_ADC_ConfigChannel(&hadc1, &yo);

  HAL_ADC_Start(&hadc1);

}

adcModule::~adcModule()
{
}

uint16_t adcModule::analogRead(ADC_HandleTypeDef* adcHandle, uint32_t channel)
{
  static uint32_t adcVal;


HAL_ADC_Start(&hadc1);


  // Poll ADC1 Perihperal & TimeOut = 1mSec
  HAL_ADC_PollForConversion(&hadc1, 1000);


// Read The ADC Conversion Result & Map It To PWM DutyCycle
 adcVal = HAL_ADC_GetValue(&hadc1);

  return (uint16_t)adcVal;
}

void adcModule::Run()
{
}
#endif
