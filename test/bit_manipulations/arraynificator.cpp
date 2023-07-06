/**
 ******************************************************************************
 * @file    arraynificator.cpp
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-10-27
 *
 ******************************************************************************
 */
#include "defines/bit_manipulations.h"
#include <gtest/gtest.h>

using namespace Nilai;

TEST(NilaiBitManipulation, Arraynificator)
{
    uint64_t v  = 0x0102030405060708;
    uint64_t v2 = 0;
    arraynificator(reinterpret_cast<void*>(&v), sizeof(v), reinterpret_cast<void*>(&v2));

    EXPECT_EQ(v, v2);
}

TEST(NilaiBitManipulation, Arraynificator_nullptr)
{
    // Nullptr with size > 0
    EXPECT_NO_THROW(arraynificator(nullptr, 8, nullptr));

    // Nullptr with size = 0
    EXPECT_NO_THROW(arraynificator(nullptr, 0, nullptr));

    // Nullptr for a source.
    uint64_t v = 0;
    EXPECT_NO_THROW(arraynificator(nullptr, sizeof(v), reinterpret_cast<void*>(&v)));

    // Nullptr for a destination.
    EXPECT_NO_THROW(
      arraynificator(reinterpret_cast<void*>(&v), sizeof(v), reinterpret_cast<void*>(&v)));
}
