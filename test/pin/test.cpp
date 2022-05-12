/**
 ******************************************************************************
 * @file    Test.cpp
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-10-26
 *
 ******************************************************************************
 */
#include "defines/pin.h"
#include <gtest/gtest.h>

using namespace cep;

static void SetGpioIdrFromBsrr(GPIO_TypeDef* gpio);

TEST(Pin, Set_Single)
{
    Pin p = {GPIOA, 1};
    p.Set(true);
    EXPECT_EQ(0x00000001, GPIOA->BSRR);
    p.Set(false);
    EXPECT_EQ(0x00010000, GPIOA->BSRR);
}

TEST(Pin, Set_Multiple)
{
    Pin p = {GPIOA, 0xAAAA};
    p.Set(true);
    EXPECT_EQ(0x0000AAAA, GPIOA->BSRR);
    p.Set(false);
    EXPECT_EQ(0xAAAA0000, GPIOA->BSRR);
}

TEST(Pin, Get_Single)
{
    Pin p = {GPIOA, 1};
    SetGpioIdrFromBsrr(GPIOA);
    EXPECT_FALSE(p.Get());

    p.Set(true);
    SetGpioIdrFromBsrr(GPIOA);
    EXPECT_TRUE(p.Get());
}

TEST(Pin, Compare_eq)
{
    Pin a  = {GPIOA, 1};
    Pin a2 = {GPIOA, 1};

    EXPECT_TRUE(a == a2) << "Expected pin a to be identical to pin a2";
}

TEST(Pin, Compare_neq)
{
    // Different port, same pin.
    Pin a = {GPIOA, 1};
    Pin b = {GPIOB, 1};

    EXPECT_FALSE(a == b) << "Expected pin a to be different from pin b";

    // Same port, different pin.
    b = {GPIOA, 2};
    EXPECT_FALSE(a == b) << "Expected pin a to be different from pin b";

    // Different port, different pin.
    b = {GPIOB, 2};
    EXPECT_FALSE(a == b);
}

void SetGpioIdrFromBsrr(GPIO_TypeDef* gpio)
{
    // Lower 16 bits acts as a set, upper 16 bits acts as a clear.
    uint16_t state = (uint16_t)gpio->BSRR;
    state &= (uint16_t)(~(gpio->BSRR >> 16));

    gpio->IDR = state;
}
