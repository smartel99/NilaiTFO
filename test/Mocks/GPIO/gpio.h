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

struct GPIO_TypeDef
{
    uint32_t BSRR = 0;    // Output.
    uint32_t IDR  = 0;    // Input.
};

extern GPIO_TypeDef* GPIOA;
extern GPIO_TypeDef* GPIOB;
extern GPIO_TypeDef* GPIOC;
extern GPIO_TypeDef* GPIOD;
extern GPIO_TypeDef* GPIOE;
extern GPIO_TypeDef* GPIOF;
extern GPIO_TypeDef* GPIOG;
extern GPIO_TypeDef* GPIOH;
extern GPIO_TypeDef* GPIOI;

#endif    // GUARD_GPIO_H
