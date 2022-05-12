/**
 ******************************************************************************
 * @file    stm32f4xx_hal.h
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-12-01
 *
 ******************************************************************************
 */
#ifndef GUARD_STM32F4XX_HAL_H
#define GUARD_STM32F4XX_HAL_H
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_gpio.h"
#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif
    void     HAL_Delay(uint32_t ms);
    uint32_t HAL_GetTick(void);

#ifdef __cplusplus
};
#endif


#endif    // GUARD_STM32F4XX_HAL_H
