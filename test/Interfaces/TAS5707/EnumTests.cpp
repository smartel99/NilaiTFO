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

#include "interfaces/TAS5707/Enums.h"
#include <gtest/gtest.h>

using namespace cep::Tas5707;

TEST(Tas5707_Enums, SampleRates)
{
    uint8_t valid[] = {SampleRates::Sr32kHz,
                       SampleRates::Sr44_1Or48kHz,
                       SampleRates::Sr16kHz,
                       SampleRates::Sr22_05Or24kHz,
                       SampleRates::Sr8kHz,
                       SampleRates::Sr11_025Or12kHz};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(SampleRates::IsSampleRate(i));
        }
        else
        {
            EXPECT_FALSE(SampleRates::IsSampleRate(i));
        }
    }
}

TEST(Tas5707_Enums, MCLKFrequencies)
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

TEST(Tas5707_Enums, PwmDcBlock)
{
    uint8_t valid[] = {PwmDcBlock::Disabled, PwmDcBlock::Enabled};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(PwmDcBlock::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(PwmDcBlock::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, ClkErrRecoveryMode)
{
    uint8_t valid[] = {ClkErrRecoveryMode::Soft, ClkErrRecoveryMode::Hard};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(ClkErrRecoveryMode::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(ClkErrRecoveryMode::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, DeEmphasisMode)
{
    uint8_t valid[] = {DeEmphasisMode::None, DeEmphasisMode::Fs32kHz, DeEmphasisMode::Fs48kHz};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(DeEmphasisMode::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(DeEmphasisMode::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, SerialDataMode)
{
    uint8_t valid[] = {SerialDataMode::RJ16Bits,
                       SerialDataMode::RJ20Bits,
                       SerialDataMode::RJ24Bits,
                       SerialDataMode::I2S16Bits,
                       SerialDataMode::I2S20Bits,
                       SerialDataMode::I2S24Bits,
                       SerialDataMode::LJ16Bits,
                       SerialDataMode::LJ20Bits,
                       SerialDataMode::LJ24Bits};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(SerialDataMode::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(SerialDataMode::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, ChannelEnable)
{
    uint8_t valid[] = {ChannelEnable::Disable, ChannelEnable::Enable};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(ChannelEnable::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(ChannelEnable::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, ChSoftMute)
{
    {
        uint8_t valid[] = {Ch1SoftMute::Mute, Ch1SoftMute::Unmute};

        for (uint8_t i = 0; i < 0xFF; i++)
        {
            if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
            {
                EXPECT_TRUE(Ch1SoftMute::IsValid(i));
            }
            else
            {
                EXPECT_FALSE(Ch1SoftMute::IsValid(i));
            }
        }
    }
    {
        uint8_t valid[] = {Ch2SoftMute::Mute, Ch2SoftMute::Unmute};

        for (uint8_t i = 0; i < 0xFF; i++)
        {
            if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
            {
                EXPECT_TRUE(Ch2SoftMute::IsValid(i));
            }
            else
            {
                EXPECT_FALSE(Ch2SoftMute::IsValid(i));
            }
        }
    }
}

TEST(Tas5707_Enums, ChannelVolume)
{
    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if ((i < 0xCF) || (i > 0xFE))
        {
            EXPECT_TRUE(ChannelVolume::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(ChannelVolume::IsValid(i));
        }
    }
}