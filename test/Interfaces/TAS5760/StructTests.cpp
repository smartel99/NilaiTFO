/**
 * @file    StructTests.cpp
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

#include "interfaces/TAS5760/RegisterStructs.h"
#include <gtest/gtest.h>

using namespace cep::Tas5760;

TEST(Tas5760_Structs, PwrCtrlReg)
{
    PwrCtrlReg r = {0};
    EXPECT_EQ(r, PwrCtrlReg::RESERVED_VALUE);

    r.DigClipLev = 0x3F;
    EXPECT_EQ(r, 0xFC);

    r          = 0;
    r.SpkSleep = 1;
    EXPECT_EQ(r, 0x02);

    r             = 0;
    r.SpkShutdown = 1;
    EXPECT_EQ(r, 0x01);

    // Reserved bit shouldn't be affected.
    r = 0xFF;
    EXPECT_EQ(r, PwrCtrlReg::RESERVED_MASK);
}

TEST(Tas5760_Structs, DigCtrlReg)
{
    DigCtrlReg r = {0};
    EXPECT_EQ(r, DigCtrlReg::RESERVED_VALUE);

    r.HpfBypass = 1;
    EXPECT_EQ(r, 0x80);

    r              = 0;
    r.DigitalBoost = 0x3;
    EXPECT_EQ(r, 0x30);

    r      = 0;
    r.SsDs = 1;
    EXPECT_EQ(r, 0x08);

    r       = 0;
    r.InFmt = 0x07;
    EXPECT_EQ(r, 0x07);

    // Reserved bit shouldn't be affected.
    r = 0xFF;
    EXPECT_EQ(r, DigCtrlReg::RESERVED_MASK);
}

TEST(Tas5760_Structs, VolCtrConfigReg)
{
    VolCtrConfigReg r = {0};
    EXPECT_EQ(r, VolCtrConfigReg::RESERVED_VALUE);

    r.Fade = 1;
    EXPECT_EQ(r, 0x80);

    r       = 0;
    r.MuteR = 1;
    EXPECT_EQ(r, 0x02);

    r       = 0;
    r.MuteL = 1;
    EXPECT_EQ(r, 0x01);

    // Reserved bit shouldn't be affected.
    r = 0xFF;
    EXPECT_EQ(r, VolCtrConfigReg::RESERVED_MASK);
}

TEST(Tas5760_Structs, AnalCtrlReg)
{
    AnalCtrlReg r {0};
    EXPECT_EQ(r, AnalCtrlReg::RESERVED_VALUE);

    r.PbtlEnable = 1;
    EXPECT_EQ(r, 0x80 | AnalCtrlReg::RESERVED_VALUE);

    r            = 0;
    r.PwmRateSel = 0x07;
    EXPECT_EQ(r, 0x70 | AnalCtrlReg::RESERVED_VALUE);

    r       = 0;
    r.AGain = 3;
    EXPECT_EQ(r, 0x0C | AnalCtrlReg::RESERVED_VALUE);

    r           = 0;
    r.PbtlChSel = 1;
    EXPECT_EQ(r, 0x02 | AnalCtrlReg::RESERVED_VALUE);

    // Reserved value shouldn't change.
    r = 0;
    EXPECT_EQ(r, AnalCtrlReg::RESERVED_VALUE);
}

TEST(Tas5760_Structs, FaultCfgAndErrStatusReg)
{
    FaultCfgAndErrStatusReg r {0};
    EXPECT_EQ(r, FaultCfgAndErrStatusReg::RESERVED_VALUE);

    r.OverCurrThresh = 0x03;
    EXPECT_EQ(r, 0x30);

    // Reserved value shouldn't change.
    r = 0xFF;
    EXPECT_EQ(r, 0x30);
}