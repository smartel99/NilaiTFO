#ifndef NILAI_TEST_MOCK_STM32F4XX_HAL_H
#define NILAI_TEST_MOCK_STM32F4XX_HAL_H

#include "stm32f4xx_hal_def.h"

#include "../ADC/adc.h"
#include "../CAN/can.h"
#include "../GPIO/gpio.h"
#include "../I2C/i2c.h"
#include "../UART/uart.h"

#include <cstdint>

uint32_t HAL_GetTick(void);
void     HAL_SetTick(uint32_t tick = 0);
void     HAL_IncTick(uint32_t tick = 1);

#endif
