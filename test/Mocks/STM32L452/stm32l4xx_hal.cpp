#include "stm32l452xx_hal.h"
static uint32_t m_tick;

uint32_t HAL_GetTick(void) {
    return m_tick;
}

void HAL_SetTick(uint32_t tick) {
    m_tick = tick;
}

void HAL_IncTick(uint32_t tick) {
    m_tick += tick;
} 