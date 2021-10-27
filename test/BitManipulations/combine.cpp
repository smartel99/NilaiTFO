/**
 ******************************************************************************
 * @file    combine.cpp
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-10-27
 *
 ******************************************************************************
 */
#include "defines/bitManipulations.hpp"
#include <gtest/gtest.h>

using namespace cep;

TEST(BitManipulation, Combine)
{
    uint8_t a[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    // Results that are expected by each iteration of the test.
    uint64_t exp[8] = {0x11,
                       0x1122,
                       0x112233,
                       0x11223344,
                       0x1122334455,
                       0x112233445566,
                       0x11223344556677,
                       0x1122334455667788};

    for (uint8_t i = 0; i < sizeof(a); i++)
    {
        EXPECT_EQ(exp[i], combine(a, i + 1))
          << "Didn't get expected result when combining " << (int)i << " values";
    }
}

TEST(BitManipulation, Combine_nullptr)
{
    EXPECT_EQ(0, combine(nullptr, 8));
}