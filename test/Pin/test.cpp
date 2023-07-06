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
#include "defines/internal_config.h"
#include "defines/pin.h"
#include <gtest/gtest.h>

using namespace Nilai;


TEST(NilaiPin, Set_Single)
{
    Pin p = {&GPIOA, 1};
    p.Set(true);
    GPIOA.Refresh();
    EXPECT_EQ(0x00000001, GPIOA.NILAI_GPIO_BSRR_REG);
    p.Set(false);
    GPIOA.Refresh();
    EXPECT_EQ(0x00010000, GPIOA.NILAI_GPIO_BSRR_REG);
}

TEST(NilaiPin, Set_Multiple)
{
    Pin p = {&GPIOA, 0xAAAA};
    p.Set(true);
    EXPECT_EQ(0x0000AAAA, GPIOA.NILAI_GPIO_BSRR_REG);
    p.Set(false);
    EXPECT_EQ(0xAAAA0000, GPIOA.NILAI_GPIO_BSRR_REG);
}

TEST(NilaiPin, Get_Single)
{
    Pin p = {&GPIOA, 1};
    GPIOA.Refresh();
    EXPECT_FALSE(p.Get());

    p.Set(true);
    GPIOA.Refresh();
    EXPECT_TRUE(p.Get());
}

TEST(NilaiPin, Compare_eq)
{
    Pin a  = {&GPIOA, 1};
    Pin a2 = {&GPIOA, 1};

    EXPECT_TRUE(a == a2) << "Expected pin a to be identical to pin a2";
}

TEST(NilaiPin, Compare_neq)
{
    // Different port, same pin.
    Pin a = {&GPIOA, 1};
    Pin b = {&GPIOB, 1};

    EXPECT_FALSE(a == b) << "Expected pin a to be different from pin b";

    // Same port, different pin.
    b = {&GPIOA, 2};
    EXPECT_FALSE(a == b) << "Expected pin a to be different from pin b";

    // Different port, different pin.
    b = {&GPIOB, 2};
    EXPECT_FALSE(a == b);
}
