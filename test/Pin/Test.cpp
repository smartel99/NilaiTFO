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

TEST(Pin, Set_Single)
{
    Pin p = {&GPIOA, 1};
    p.Set(true);
    EXPECT_EQ(0x0001, GPIOA);
    p.Set(false);
    EXPECT_EQ(0x0000, GPIOA);
}

TEST(Pin, Set_Multiple)
{
    Pin p = {&GPIOA, 0xAAAA};
    p.Set(true);
    EXPECT_EQ(0xAAAA, GPIOA);
    p.Set(false);
    EXPECT_EQ(0x0000, GPIOA);
}

TEST(Pin, Set_DontAffectOthers)
{
    Pin p1 = {&GPIOA, 1};
    Pin p2 = {&GPIOA, 0x8000};
    p1.Set(true);
    p2.Set(true);
    EXPECT_EQ(0x8001, GPIOA);

    p1.Set(false);
    EXPECT_EQ(0x8000, GPIOA);

    EXPECT_EQ(true, p2.Get());
}

TEST(Pin, Get_Single)
{
    Pin p = {&GPIOA, 1};
    EXPECT_EQ(false, p.Get());
    p.Set(true);
    EXPECT_EQ(true, p.Get());
}

TEST(Pin, Compare_eq)
{
    Pin a = {&GPIOA, 1};
    Pin a2 = {&GPIOA, 1};

    EXPECT_TRUE(a == a2) << "Expected pin a to be identical to pin a2";
}

TEST(Pin, Compare_neq)
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
}
