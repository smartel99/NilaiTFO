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

#include "../../defines/internal_config.h"
#include <cstdint>

struct GPIO_TypeDef
{
    uint32_t NILAI_GPIO_BSRR_REG = 0;
    uint32_t NILAI_GPIO_IDR_REG  = 0;

    void Refresh()
    {
        // Upper 16 bits of BSRR are the "clear" request bits, used to force the bit in a low state.
        // Lower 16 bits of BSRR are the "set" request bits, used to force the bit in a high state.
        uint16_t clear     = ~static_cast<uint16_t>((NILAI_GPIO_BSRR_REG >> 16) & 0x0000FFFF);
        uint16_t set       = static_cast<uint16_t>((NILAI_GPIO_BSRR_REG & 0x0000FFFF));
        NILAI_GPIO_IDR_REG = clear & set;
    }
};

extern GPIO_TypeDef GPIOA;
extern GPIO_TypeDef GPIOB;
extern GPIO_TypeDef GPIOC;
extern GPIO_TypeDef GPIOD;
extern GPIO_TypeDef GPIOE;
extern GPIO_TypeDef GPIOF;
extern GPIO_TypeDef GPIOG;
extern GPIO_TypeDef GPIOH;
extern GPIO_TypeDef GPIOI;

enum GPIO_PinState
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET,
};


void          HAL_GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin);

GPIO_TypeDef* NILAI_HAL_GPIO_Refresh(GPIO_TypeDef* port);

#endif    // GUARD_GPIO_H
