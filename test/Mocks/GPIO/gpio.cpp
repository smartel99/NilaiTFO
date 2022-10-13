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

GPIO_TypeDef GPIOA = {};
GPIO_TypeDef GPIOB = {};
GPIO_TypeDef GPIOC = {};
GPIO_TypeDef GPIOD = {};
GPIO_TypeDef GPIOE = {};
GPIO_TypeDef GPIOF = {};
GPIO_TypeDef GPIOG = {};
GPIO_TypeDef GPIOH = {};
GPIO_TypeDef GPIOI = {};

static bool IsPortValid(const GPIO_TypeDef* port);


void HAL_GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state)
{
    assert(IsPortValid(port));

    if (state == GPIO_PIN_SET)
    {
        port->NILAI_GPIO_BSRR_REG |= pin;
    }
    else
    {
        port->NILAI_GPIO_BSRR_REG &= static_cast<uint16_t>(~pin);
    }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin)
{
    assert(IsPortValid(port));

    return (port->NILAI_GPIO_IDR_REG & pin) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET;
}

GPIO_TypeDef* NILAI_HAL_GPIO_Refresh(GPIO_TypeDef* port)
{
    port->Refresh();
    return port;
}


bool IsPortValid(const GPIO_TypeDef* port)
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
