#pragma once

#include "Core/Inc/adc.h"
#include "shared/defines/module.hpp"

class adcModule
{
public:
  adcModule();
  ~adcModule();
  void Run();
  uint16_t analogRead(ADC_HandleTypeDef *adcHandle, uint32_t channel);


private:
};