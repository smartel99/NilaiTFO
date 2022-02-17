/**
 ******************************************************************************
 * @file    gpio.cpp
 * @author  Samuel Martel
 * @brief   Sources for the mock GPIO module.
 *
 * @date 2021-10-26
 *
 ******************************************************************************
 */
#include "gpio.h"

#include <cassert>

GPIO_TypeDef _GPIOA = 0x0000;
GPIO_TypeDef _GPIOB = 0x0000;
GPIO_TypeDef _GPIOC = 0x0000;
GPIO_TypeDef _GPIOD = 0x0000;
GPIO_TypeDef _GPIOE = 0x0000;
GPIO_TypeDef _GPIOF = 0x0000;
GPIO_TypeDef _GPIOG = 0x0000;
GPIO_TypeDef _GPIOH = 0x0000;
GPIO_TypeDef _GPIOI = 0x0000;

GPIO_TypeDef* GPIOA = &_GPIOA;
GPIO_TypeDef* GPIOB = &_GPIOB;
GPIO_TypeDef* GPIOC = &_GPIOC;
GPIO_TypeDef* GPIOD = &_GPIOD;
GPIO_TypeDef* GPIOE = &_GPIOE;
GPIO_TypeDef* GPIOF = &_GPIOF;
GPIO_TypeDef* GPIOG = &_GPIOG;
GPIO_TypeDef* GPIOH = &_GPIOH;
GPIO_TypeDef* GPIOI = &_GPIOI;

static bool IsPortValid(const GPIO_TypeDef* port);


void HAL_GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state)
{
    assert(IsPortValid(port));

    if (state == GPIO_PIN_SET)
    {
        *port |= pin;
    }
    else
    {
        *port &= static_cast<uint16_t>(~pin);
    }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin)
{
    assert(IsPortValid(port));

    return (*port & pin) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET;
}


bool IsPortValid(const GPIO_TypeDef* port)
{
    return ((port == GPIOA) || (port == GPIOB) || (port == GPIOC) || (port == GPIOD) ||
            (port == GPIOE) || (port == GPIOF) || (port == GPIOG) || (port == GPIOH) ||
            (port == GPIOI));
}
