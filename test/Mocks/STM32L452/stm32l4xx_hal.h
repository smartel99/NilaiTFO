#ifndef NILAI_TEST_MOCK_STM32L4XX_HAL_H
#define NILAI_TEST_MOCK_STM32L4XX_HAL_H

#include "stm32l4xx_hal_def.h"
#include <cstdint>

#include "../ADC/adc.h"
#include "../CAN/can.h"
#include "../GPIO/gpio.h"
#include "../I2C/i2c.h"
#include "../UART/uart.h"

uint32_t HAL_GetTick(void);
void     HAL_SetTick(uint32_t tick = 0);
void     HAL_IncTick(uint32_t tick = 1);
#endif
