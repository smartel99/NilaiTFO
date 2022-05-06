/**
 * @file    RegisterStructs.h
 * @author  Samuel Martel
 * @date    2022-03-03
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
#ifndef NILAI_TAS5760_REGISTERSTRUCTS_H
#define NILAI_TAS5760_REGISTERSTRUCTS_H

#if defined(NILAI_USE_SW_TAS5760)
#    include <cstdint>

namespace Nilai::Tas5760
{
union PwrCtrlReg
{
    struct
    {
        /**
         * @brief Speaker Shutdown
         *
         * <br>
         * @b 0: Speaker amplifier is shut down. This is the lowest power mode available when the
         * device is connected to power supplies. In this mode, circuitry in both the DVDD and PVDD
         * domain are powered down to minimize power consumption.
         * <br>
         * @b 1: Speaker amplifier is not shut down.
         */
        uint8_t SpkShutdown : 1;
        /**
         * @brief Sleep Mode
         *
         * <br>
         * @b 0: Device is not in sleep mode.
         * <br>
         * @b 1: Device is placed in sleep mode. In this mode, the power stage is disabled to reduce
         * quiescent power consumption over a 50/50 duty cycle mute, while low-voltage blocks remain
         * on standby. This reduces the time required to resume playback when compared with entering
         * and exiting full shut down.
         */
        uint8_t SpkSleep : 1;
        /**
         * @brief The digital clipper is decoded from 3 registers-
         *  DigClipLev[19:14], DigClipLev[13:6] and DigClipLev[5:0].
         *
         * DigClipLev[19:14], shown here, represents the upper 6 bits of the total of 20 bits that
         * are used to set the digital clipping threshold.
         */
        uint8_t DigClipLev : 6;
    };

private:
    uint8_t Register = 0xFD;

public:
    PwrCtrlReg() = default;
    PwrCtrlReg(uint8_t v) : Register(v | RESERVED_VALUE) {}
    PwrCtrlReg(const PwrCtrlReg&) = default;
    PwrCtrlReg& operator=(const PwrCtrlReg& o)
    {
        Register = o.Register;
        return *this;
    }
    PwrCtrlReg& operator=(uint8_t v)
    {
        Register = v & RESERVED_MASK;
        return *this;
    }

    operator int() const { return Register; }

    static constexpr uint8_t RESERVED_MASK  = 0xFF;
    static constexpr uint8_t RESERVED_VALUE = 0x00;
};

union DigCtrlReg
{
    struct
    {
        /**
         * @brief Serial Audio Input Format
         *
         * <br>
         * @b 000: Serial audio input format is 24 bits, right justified.
         * <br>
         * @b 001: Serial audio input format is 20 bits, right justified.
         * <br>
         * @b 010: Serial audio input format is 18 bits, right justified.
         * <br>
         * @b 011: Serial audio input format is 16 bits, right justified.
         * <br>
         * @b 100: Serial audio input format is I2S (default).
         * <br>
         * @attention Values above 0b100 shouldn't be used.
         */
        uint8_t InFmt : 3;
        /**
         * @brief Single Speed/Double Speed Mode Select
         *
         * <br>
         * @b 0: Serial audio port will accept single speed sample rates (that is: 32kHz, 44.1kHz
         * and 48kHz). <br>
         * @b 1: Serial audio port will accept double speed sample rates (that is: 88.2kHz and
         * 96kHz).
         */
        uint8_t SsDs : 1;
        /**
         * @brief Digital Boost
         *
         * <br>
         * @b 00: +0dB is added to the signal in the digital path.
         * <br>
         * @b 01: +6dB is added to the signal in the digital path (default).
         * <br>
         * @b 10: +12dB is added to the signal in the digital path.
         * <br>
         * @b 11: +18dB is added to the signal in the digital path.
         */
        uint8_t DigitalBoost : 2;
        /**
         * @attention This control is reserved and must not be changed from it's default setting.
         */
        uint8_t Reserved : 1;
        /**
         * @brief High-Pass Filter Bypass.
         * <br>
         * @b 0: The internal high-pass filter in the digital path is not bypassed.
         * <br>
         * @b 1: The internal high-pass filter in the digital path is bypassed.
         */
        uint8_t HpfBypass : 1;
    };

private:
    uint8_t Register = 0x14;

public:
    DigCtrlReg() = default;
    DigCtrlReg(uint8_t v) : Register(v | RESERVED_VALUE) {}
    DigCtrlReg(const DigCtrlReg&) = default;
    DigCtrlReg& operator=(const DigCtrlReg& o)
    {
        Register = o.Register;
        return *this;
    }
    DigCtrlReg& operator=(uint8_t v)
    {
        Register = v & RESERVED_MASK;
        return *this;
    }

    operator int() const { return Register; }

    static constexpr uint8_t RESERVED_MASK  = 0xBF;
    static constexpr uint8_t RESERVED_VALUE = 0x00;
};

union VolCtrConfigReg
{
    struct
    { /**
       * @brief Mute Left Channel.
       *
       * <br>
       * @b 0: The left channel is not muted.
       * <br>
       * @b 1: The left channel is muted. In software mute, most analog and digital blocks remain
       * active and the speaker amplifier outputs transition to a 50/50 duty cycle.
       */
        uint8_t MuteL : 1;
        /**
         * @brief Mute Right Channel.
         *
         * <br>
         * @b 0: The right channel is not muted.
         * <br>
         * @b 1: The right channel is muted. In software mute, most analog and digital blocks remain
         * active and the speaker amplifier outputs transition to a 50/50 duty cycle.
         */
        uint8_t MuteR : 1;
        /**
         * @attention This control is reserved and must not be changed from its default setting.
         */
        uint8_t Reserved : 5;
        /**
         * @brief Volume Fade Enable
         *
         * <br>
         * @b 0: Volume fading is disabled.
         * <br>
         * @b 1: Volume fading is enabled.
         */
        uint8_t Fade : 1;
    };

private:
    uint8_t Register = 0x80;

public:
    VolCtrConfigReg() = default;
    VolCtrConfigReg(uint8_t v) : Register(v | RESERVED_VALUE) {}
    VolCtrConfigReg(const VolCtrConfigReg&) = default;
    VolCtrConfigReg& operator=(const VolCtrConfigReg& o)
    {
        Register = o.Register;
        return *this;
    }
    VolCtrConfigReg& operator=(uint8_t v)
    {
        Register = v & RESERVED_MASK;
        return *this;
    }

    operator int() const { return Register; }

    static constexpr uint8_t RESERVED_MASK  = 0x83;
    static constexpr uint8_t RESERVED_VALUE = 0x00;
};

union AnalCtrlReg
{
    struct
    {
        //! @attention This control is reserved and must not be changed from its default setting.
        uint8_t Reserved : 1;
        /**
         * @brief Channel Selection for PBTL Mode
         *
         * <br>
         * @b 0: When placed in PBTL mode, the audio information from the right channel of the
         * serial audio input stream is used by the speaker amplifier. <br>
         * @b 1: When placed in PBTL mode, the audio information from the left channel of the serial
         * audio input stream is used by the speaker amplifier.
         */
        uint8_t PbtlChSel : 1;
        /**
         * @brief Analog Gain Setting
         *
         * <br>
         * @b 00: Analog gain setting is 19.2dBV. (Default)
         * <br>
         * @b 01: Analog gain setting is 22.6 dBV.
         * <br>
         * @b 10: Analog gain setting is 25dBV
         * <br>
         * @b 11: This setting is reserved and must not be used.
         */
        uint8_t AGain : 2;
        /**
         * @brief PWM Rate Select
         *
         * <table style="border:1px solid">
         *  <tr style="border-bottom:1px solid">
         *     <th>Value</th>
         *     <th style="border-left:1px solid">Output switching rate of the speaker amplifier</th>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">000</td><td>6 * LRCK</td>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">001</td><td>8 * LRCK</td>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">010</td><td>10 * LRCK</td>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">011</td><td>12 * LRCK</td>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">100</td><td>14 * LRCK</td>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">101</td><td>16 * LRCK</td>
         *  </tr>
         *  <tr style="border-bottom:1px solid">
         *     <td style="border-right:1px solid">110</td><td>20 * LRCK</td>
         *  </tr>
         *  <tr                                >
         *     <td style="border-right:1px solid">111</td><td>24 * LRCK</td>
         *  </tr>
         * </table>
         *
         * @note All rates listed above are valid for single speed mode. For double speed mode,
         * switching frequency is half of that represented above.
         */
        uint8_t PwmRateSel : 3;
        /**
         * @brief PBTL Enable
         *
         * <br>
         * @b 0: Device is placed in BTL (Bridge-Tied Load) mode.
         * <br>
         * @b 1: Device is placed in PBTL (Parallel Bridge-Tied Load) mode.
         */
        uint8_t PbtlEnable : 1;
    };

private:
    uint8_t Register = 0x51;

public:
    AnalCtrlReg() = default;
    AnalCtrlReg(uint8_t v) : Register(v | RESERVED_VALUE) {}
    AnalCtrlReg(const AnalCtrlReg&) = default;
    AnalCtrlReg& operator=(const AnalCtrlReg& o)
    {
        Register = o.Register;
        return *this;
    }
    AnalCtrlReg& operator=(uint8_t v)
    {
        Register = (v & RESERVED_MASK) | RESERVED_VALUE;
        return *this;
    }

    operator int() const { return Register; }

    static constexpr uint8_t RESERVED_MASK  = 0xFE;
    static constexpr uint8_t RESERVED_VALUE = 0x01;
};

union FaultCfgAndErrStatusReg
{
    struct
    {
        /**
         * @brief Over-temperature Error Status
         *
         * <br>
         * @b 0: The temperature of the die is below the OTE threshold.
         * <br>
         * @b 1: The temperature of the die has exceeded the level specified in the electrical
         * characteristics table. This is a latching error and SPK_SD must be toggled for the device
         * to resume normal operation. This bit will remain HIGH until SPK_SD is toggled.
         */
        uint8_t OverTemp : 1;
        /**
         * @brief Output DC Error Status
         *
         * <br>
         * @b 0: The DC offset level of the speaker amplifier outputs are below the DCE threshold.
         * <br>
         * @b 1: The DC offset level of the speaker amplifier outputs has exceeded the DCE
         * threshold, causing an error. This is a latching error and SPK_SD must be toggled after a
         * DCE event for the device to resume normal operation. This bit will remain HIGH until
         * SPK_SD is toggled.
         */
        uint8_t DCErr : 1;
        /**
         * @brief Over Current Error Status
         *
         * <br>
         * @b 0: The output current levels of the speaker amplifier outputs are below the OCE
         * threshold. <br>
         * @b 1: The DC offset level of the outputs has exceeded the OCE threshold, causing an
         * error. This is a latching error and SPK_SD must be toggled after an OCE event for the
         * device to resume normal operation. This bit will remain HIGH until SPK_SD is toggle.
         */
        uint8_t OverCurr : 1;
        /**
         * @brief Clock Error Status
         *
         * <br>
         * @b 0: Clocks are valid and no error is currently detected.
         * <br>
         * @b 1: A clock error is occurring. This error is non-latching, so intermittent clock
         * errors will be cleared when clocks re-enter valid state and the device will resume normal
         * operation automatically. This bit will likewise be cleared once normal operation resumes.
         */
        uint8_t ClkErr : 1;
        /**
         * @brief Over Current Threshold
         *
         * <br>
         * @b 00: Threshold is set to the default level specified in the electrical characteristics
         * table (Default). <br>
         * @b 01: Threshold is reduced to 75% of the level specified in the electrical
         * characteristics table. <br>
         * @b 10: Threshold is reduced to 50% of the level specified in the electrical
         * characteristics table. <br>
         * @b 11: Threshold is reduced to 25% of the level specified in the electrical
         * characteristics table.
         */
        uint8_t OverCurrThresh : 2;
        //! @attention This control is reserved and must not be changed from its default setting.
        uint8_t Reserved : 2;
    };

private:
    uint8_t Register = 0x00;

public:
    FaultCfgAndErrStatusReg() = default;
    FaultCfgAndErrStatusReg(uint8_t v) : Register((v & RESERVED_MASK) | RESERVED_VALUE) {}
    FaultCfgAndErrStatusReg(const FaultCfgAndErrStatusReg&) = default;
    FaultCfgAndErrStatusReg& operator=(const FaultCfgAndErrStatusReg& o)
    {
        Register = o.Register;
        return *this;
    }
    FaultCfgAndErrStatusReg& operator=(uint8_t v)
    {
        Register = (v & RESERVED_MASK) | RESERVED_VALUE;
        return *this;
    }

    operator int() const { return Register; }

    static constexpr uint8_t RESERVED_MASK  = 0x30;
    static constexpr uint8_t RESERVED_VALUE = 0x00;
};
}    // namespace Nilai::Tas5760
#endif

#endif    // NILAI_TAS5760_REGISTERSTRUCTS_H
