#pragma once

#include "stm32l4xx_hal_def.h"
#include <cstdint>

uint32_t HAL_GetTick(void);
void HAL_SetTick(uint32_t tick = 0);
void HAL_IncTick(uint32_t tick = 1); 