/**
******************************************************************************
* @file    gpio.h
* @author  Samuel Martel
* @brief   Header for the GPIO mock up module.
*
* @date 2021/10/26
*
******************************************************************************
*/
#ifndef GUARD_GPIO_H
#define GUARD_GPIO_H

#include <cstdint>

using GPIO_TypeDef = uint16_t;

extern GPIO_TypeDef* GPIOA;
extern GPIO_TypeDef* GPIOB;
extern GPIO_TypeDef* GPIOC;
extern GPIO_TypeDef* GPIOD;
extern GPIO_TypeDef* GPIOE;
extern GPIO_TypeDef* GPIOF;
extern GPIO_TypeDef* GPIOG;
extern GPIO_TypeDef* GPIOH;
extern GPIO_TypeDef* GPIOI;

enum GPIO_PinState
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET,
};


void          HAL_GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin);

#endif    // GUARD_GPIO_H
