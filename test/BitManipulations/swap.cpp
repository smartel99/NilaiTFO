/**
 ******************************************************************************
 * @file    swap.cpp
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

TEST(BitManipulation, Swap_integrals)
{
    EXPECT_EQ(0x5A, swap<uint8_t>(0xA5));
    EXPECT_EQ(0x1122, swap<uint16_t>(0x2211));
    EXPECT_EQ(0x11223344, swap<uint32_t>(0x33441122));
    EXPECT_EQ(0x1122334455667788, swap<uint64_t>(0x5566778811223344));
}
