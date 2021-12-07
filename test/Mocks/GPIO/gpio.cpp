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

#include "defines/macros.hpp"

GPIO_TypeDef GPIOA = 0x0000;
GPIO_TypeDef GPIOB = 0x0000;
GPIO_TypeDef GPIOC = 0x0000;
GPIO_TypeDef GPIOD = 0x0000;
GPIO_TypeDef GPIOE = 0x0000;
GPIO_TypeDef GPIOF = 0x0000;
GPIO_TypeDef GPIOG = 0x0000;
GPIO_TypeDef GPIOH = 0x0000;
GPIO_TypeDef GPIOI = 0x0000;

static bool IsPortValid(const uint16_t* port);


void HAL_GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state)
{
    CEP_ASSERT(IsPortValid(port), "GPIO port is invalid!");

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
    CEP_ASSERT(IsPortValid(port), "GPIO port is invalid!");

    return (*port & pin) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET;
}


bool IsPortValid(const uint16_t* port)
{
    if ((port == &GPIOA) || (port == &GPIOB) || (port == &GPIOC) || (port == &GPIOD) ||
        (port == &GPIOE) || (port == &GPIOF) || (port == &GPIOG) || (port == &GPIOH) ||
        (port == &GPIOI))
    {
        return true;
    }
    else
    {
        return false;
    }
}
