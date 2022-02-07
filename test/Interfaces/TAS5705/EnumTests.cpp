/**
 * @file    EnumTests.cpp
 * @author  Samuel Martel
 * @date    2022-02-07
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */

#include "interfaces/TAS5705/Enums.h"
#include <gtest/gtest.h>

using namespace cep::Tas5705;

TEST(Tas5705_Enums, SampleRates)
{
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr32kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr38kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr44_1kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr48kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr88_2kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr96kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr176_4kHz));
    EXPECT_TRUE(SampleRates::IsSampleRate(SampleRates::Sr192kHz));

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        // Ignore all ints that don't have any of the first 5 bits set.
        if ((uint8_t)(i << 3) == 0)
        {
            EXPECT_TRUE(SampleRates::IsSampleRate(i));
        }
        else
        {
            EXPECT_FALSE(SampleRates::IsSampleRate(i)) << "i is " << std::hex << i;
        }
    }
}

TEST(Tas5705_Enums, MCLKFrequencies)
{
    uint8_t valid[] = {MclkFrequencies::Fs64,
                       MclkFrequencies::Fs128,
                       MclkFrequencies::Fs192,
                       MclkFrequencies::Fs256,
                       MclkFrequencies::Fs384,
                       MclkFrequencies::Fs512};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(MclkFrequencies::IsMclkFrequency(i));
        }
        else
        {
            EXPECT_FALSE(MclkFrequencies::IsMclkFrequency(i));
        }
    }
}

TEST(Tas5705_Enums, SCLKFrequencies)
{
    uint8_t valid[] = {SclkFrequencies::Fs64OrFs32, SclkFrequencies::Fs48};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(SclkFrequencies::IsSclkFrequency(i));
        }
        else
        {
            EXPECT_FALSE(SclkFrequencies::IsSclkFrequency(i));
        }
    }
}

TEST(Tas5705_Enums, ClockStatuses)
{
    uint8_t valid[] = {ClockStatuses::ClockNotValid, ClockStatuses::ClockValid};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(ClockStatuses::IsClockStatus(i));
        }
        else
        {
            EXPECT_FALSE(ClockStatuses::IsClockStatus(i));
        }
    }
}
