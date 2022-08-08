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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */

#include "interfaces/TAS5707/enums.h"
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

TEST(Tas5707_Enums, ClkErrRecoveryModes)
{
    uint8_t valid[] = {ClkErrRecoveryModes::Soft, ClkErrRecoveryModes::Hard};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(ClkErrRecoveryModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(ClkErrRecoveryModes::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, DeEmphasisModes)
{
    uint8_t valid[] = {DeEmphasisModes::None, DeEmphasisModes::Fs32kHz, DeEmphasisModes::Fs48kHz};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(DeEmphasisModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(DeEmphasisModes::IsValid(i));
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

TEST(Tas5707_Enums, FineVolume)
{
    uint8_t valid[] = {
      FineVolume::DB0, FineVolume::DB0_125, FineVolume::DB0_25, FineVolume::DB0_375};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(FineVolume::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(FineVolume::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, VolumeSlewRates)
{
    uint8_t valid[] = {VolumeSlewRates::Steps512,
                       VolumeSlewRates::Steps1024,
                       VolumeSlewRates::Steps2048,
                       VolumeSlewRates::Steps256};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(VolumeSlewRates::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(VolumeSlewRates::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, ModulationLimit)
{
    uint8_t valid[] = {ModulationLimit::P99_2,
                       ModulationLimit::P98_4,
                       ModulationLimit::P97_7,
                       ModulationLimit::P96_9,
                       ModulationLimit::P96_1,
                       ModulationLimit::P95_3,
                       ModulationLimit::P94_5,
                       ModulationLimit::P93_8};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(ModulationLimit::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(ModulationLimit::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, InterchannelDelays)
{
    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if ((uint8_t)(i << 6) == 0)
        {
            // Bit 0 and 1 are 0b00.
            EXPECT_TRUE(InterchannelDelays::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(InterchannelDelays::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, StartStopPeriods)
{
    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if ((i == 0) || ((i >= 0x08) && (i <= 0x1F)))
        {
            // Bit 0 and 1 are 0b00.
            EXPECT_TRUE(StartStopPeriods::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(StartStopPeriods::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, OscillatorTrimSelect)
{
    uint8_t valid[] = {OscillatorTrimSelect::Factory, OscillatorTrimSelect::Disabled};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(OscillatorTrimSelect::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(OscillatorTrimSelect::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, BackEndRestartDelays)
{
    using namespace BackEndRestartDelays;
    uint8_t valid[] = {Ms299, Ms449, Ms598, Ms748, Ms898, Ms1047, Ms1197, Ms1346, Ms1496};

    for (uint8_t i = 0; i < 0xFF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(BackEndRestartDelays::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(BackEndRestartDelays::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, Channel1ModulationModes)
{
    using namespace Channel1ModulationModes;
    uint32_t valid[] = {AD, BD};

    // Test from 0x00000000 to 0x00FF0000, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x00FF0000; i += 0x00010000)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(Channel1ModulationModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(Channel1ModulationModes::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, Channel2ModulationModes)
{
    using namespace Channel2ModulationModes;
    uint32_t valid[] = {AD, BD};

    // Test from 0x00000000 to 0x00FF0000, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x00FF0000; i += 0x00010000)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(Channel2ModulationModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(Channel2ModulationModes::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, Channel1InputSources)
{
    using namespace Channel1InputSources;
    uint32_t valid[] = {SDIN_L, SDIN_R, GND};

    // Test from 0x00000000 to 0x00FF0000, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x00FF0000; i += 0x00010000)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(Channel1InputSources::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(Channel1InputSources::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, Channel2InputSources)
{
    using namespace Channel2InputSources;
    uint32_t valid[] = {SDIN_L, SDIN_R, GND};

    // Test from 0x00000000 to 0x00FF0000, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x00FF0000; i += 0x00010000)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(Channel2InputSources::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(Channel2InputSources::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, OutASources)
{
    using namespace OutASources;
    uint32_t valid[] = {Ch1, Ch2, Ch1P, Ch2P};

    // Test from 0x00000000 to 0x00FF0000, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x00FF0000; i += 0x00010000)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(OutASources::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(OutASources::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, OutBSources)
{
    using namespace OutBSources;
    uint32_t valid[] = {Ch1, Ch2, Ch1P, Ch2P};

    // Test from 0x00000000 to 0x00FF0000, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x00FF0000; i += 0x00010000)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(OutBSources::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(OutBSources::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, OutCSources)
{
    using namespace OutCSources;
    uint32_t valid[] = {Ch1, Ch2, Ch1P, Ch2P};

    // Test from 0x00000000 to 0x0000FF00, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x0000FF00; i += 0x00000100)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(OutCSources::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(OutCSources::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, OutDSources)
{
    using namespace OutDSources;
    uint32_t valid[] = {Ch1, Ch2, Ch1P, Ch2P};

    // Test from 0x00000000 to 0x0000FF00, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x0000FF00; i += 0x00000100)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(OutDSources::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(OutDSources::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, DrcModes)
{
    using namespace DrcModes;
    uint32_t valid[] = {Disabled, Enabled};

    // Test from 0x00000000 to 0x000000FF, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x000000FF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(DrcModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(DrcModes::IsValid(i));
        }
    }
}

TEST(Tas5707_Enums, BankSwitchingModes)
{
    using namespace BankSwitchingModes;
    uint32_t valid[] = {NoSwitch, Bank1, Bank2, Bank3, Automatic};

    // Test from 0x00000000 to 0x000000FF, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x000000FF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(BankSwitchingModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(BankSwitchingModes::IsValid(i));
        }
    }

    EXPECT_FALSE(BankSwitchingModes::IsValid(0xFFFFFFFF));
}

TEST(Tas5707_Enums, BankMappingOverrides)
{
    using namespace BankMappingOverrides;
    uint32_t valid[] = {Default, Override};

    // Test from 0x00000000 to 0x000000FF, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x000000FF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(BankMappingOverrides::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(BankMappingOverrides::IsValid(i));
        }
    }

    EXPECT_FALSE(BankMappingOverrides::IsValid(0xFFFFFFFF));
}

TEST(Tas5707_Enums, EqModes)
{
    using namespace EqModes;
    uint32_t valid[] = {Enabled, Disabled};

    // Test from 0x00000000 to 0x000000FF, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x000000FF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(EqModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(EqModes::IsValid(i));
        }
    }

    EXPECT_FALSE(EqModes::IsValid(0xFFFFFFFF));
}

TEST(Tas5707_Enums, BiquadModes)
{
    using namespace BiquadModes;
    uint32_t valid[] = {Independent, Ganged};

    // Test from 0x00000000 to 0x000000FF, to avoid spending a *lot* of time here.
    for (uint32_t i = 0; i < 0x000000FF; i++)
    {
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid))
        {
            EXPECT_TRUE(BiquadModes::IsValid(i));
        }
        else
        {
            EXPECT_FALSE(BiquadModes::IsValid(i));
        }
    }

    EXPECT_FALSE(BiquadModes::IsValid(0xFFFFFFFF));
}

TEST(Tas5707_Enums, BankSampleRates)
{
    using namespace BankSampleRates;
    uint8_t valid[] = {
      Use32kHz, Use44_1Or48kHz, Use16kHz, Use22_025Or24kHz, Use8kHz, Use11_025Or12kHz};

    for (const auto& v : valid)
    {
        EXPECT_TRUE(IsValid(v));
    }

    EXPECT_FALSE(IsValid(0x60));
    EXPECT_FALSE(IsValid(0xFF));

    EXPECT_EQ(ToBank1(Bank1Default), 0x00008000);
    EXPECT_EQ(ToBank2(Bank2Default), 0x00700000);
    EXPECT_EQ(ToBank3(Bank3Default), 0x0F000000);
}
