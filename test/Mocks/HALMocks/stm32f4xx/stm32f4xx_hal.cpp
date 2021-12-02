/**
 ******************************************************************************
 * @file    stm32f4xx.c
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-12-01
 *
 ******************************************************************************
 */

#include "stm32f4xx_hal.h"

#include <chrono>
#include <thread>

extern "C" void HAL_Delay(uint32_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
