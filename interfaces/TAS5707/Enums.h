/**
 * @file    Enums.h
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
#ifndef NILAI_TAS5707_ENUMS_H
#define NILAI_TAS5707_ENUMS_H

#if defined(NILAI_USE_TAS5707)

#    include <cstdint>

namespace cep::Tas5707
{
enum class Registers : uint8_t
{
    //! Clock control register. 1 byte, default value: 0x6C
    ClkCtrl = 0x00,
    //! Device ID register. 1 byte, default value: 0x70
    DevId,
    //! Error status register. 1 byte, default: 0x00
    ErrStatus,
    //! System control register. 1 byte, default 0xA0
    SysCtrl1,
    //! Serial data interface register. 1 byte, default: 0x05
    SerDataInter,
    //! System control register 2. 1 byte, default: 0x40
    SysCtrl2,
    //! Soft mute register. 1 byte, default: 0x00
    SoftMute,
    //! Master volume register. 1 byte, default: 0xFF (Muted)
    MasterVol,
    //! Channel 1 volume. 1 byte, default: 0x30 (0dB)
    Ch1Vol,
    //! Channel 2 volume. 1 byte, default: 0x30 (0dB)
    Ch2Vol,
    //! Fine master volume register. 1 byte, default: 0x00
    MasterFineVol,
    // 0x0B-0x0D: Reserved.
    //! Volume configuration register. 1 byte, default: 0x91
    VolCtrl = 0x0E,
    // 0x0F: Reserved.
    //! Modulation limit register. 1 byte, default: 0x02
    ModLimit = 0x10,
    //! Inter-channel delay channel 1. 1 byte, default: 0xAC
    IcDelayCh1,
    //! Inter-channel delay channel 2. 1 byte, default: 0x54
    IcDelayCh2,
    //! Inter-channel delay channel 3. 1 byte, default: 0xAC
    IcDelayCh3,
    //! Inter-channel delay channel 4. 1 byte, default: 0x54
    IcDelayCh4,
    // 0x15-0x19: Reserved.
    //! Start/Stop period register. 1 byte, default: 0x0F
    StartStopPeriod = 0x1A,
    //! Oscillator trim register. 1 byte, default: 0x82
    OscTrim,
    //! Back end error register. 1 byte, default: 0x02
    BkndErr,
    // 0x1D-0x1F: Reserved.
    //! Input Mux register. 4 bytes, default: 0x00017772
    InMux = 0x20,
    // 0x21-0x24: Reserved.
    //! PWM Mux register. 4 bytes, default: 0x01021345
    PwmMux = 0x25,
    // 0x26-0x28: Reserved.

    /**
     * Biquad filters are all composed of 20 bytes, separated as:
     * <ul>
     *  <li>Bytes 0 to 3: b0 coefficient, default: 0x00800000</li>
     *  <li>Bytes 4 to 7: b1 coefficient, default: 0x00000000</li>
     *  <li>Bytes 8 to 11: b2 coefficient, default: 0x00000000</li>
     *  <li>Bytes 12 to 15: a1 coefficient, default: 0x00000000</li>
     *  <li>Bytes 16 to 19: a2 coefficient, default: 0x00000000</li>
     * </ul>
     */
    Ch1Bq_0 = 29,
    Ch1Bq_1,
    Ch1Bq_2,
    Ch1Bq_3,
    Ch1Bq_4,
    Ch1Bq_5,
    Ch1Bq_6,
    Ch2Bq_0,
    Ch2Bq_1,
    Ch2Bq_2,
    Ch2Bq_3,
    Ch2Bq_4,
    Ch2Bq_5,
    Ch2Bq_6,
    // 0x37-0x39: Reserved.
    /**
     * @brief DRC alpha of energy (ae) filter and 1 - ae.
     * 8 bytes, split as:
     * <ul>
     *  <li>Bytes 0 to 3: ae, default: 0x00800000</li>
     *  <li>Bytes 4 to 7: 1-ae, default: 0x00000000</li>
     * </ul>
     */
    DrcAe = 0x3A,
    /**
     * @brief DRC alpha of attack filter (aa) and 1 - aa.
     * 8 bytes, split as:
     * <ul>
     *  <li>Bytes 0 to 3: aa, default: 0x00800000</li>
     *  <li>Bytes 4 to 7: 1-aa, default: 0x00000000</li>
     * </ul>
     */
    DrcAa,
    /**
     * @brief DRC alpha of decay filter (ad) and 1 - ad.
     * 8 bytes, split as:
     * <ul>
     *  <li>Bytes 0 to 3: ad, default: 0x00800000</li>
     *  <li>Bytes 4 to 7: 1-ad, default: 0x00000000</li>
     * </ul>
     */
    DrcAd,
    // 0x3D-0x3F: Reserved.
    //! DRC-T. 4 bytes, default: 0xFDA21490
    DrcT = 0x40,
    //! DRC-K. 4 bytes, default: 0x03842109
    DrcK,
    //! DRC-O. 4 bytes, default: 0x00084210
    DrcO,
    // 0x43-0x45: Reserved.
    //! DRC Control register. 4 bytes, default: 0x00000000
    DrcCtrl = 0x46,
    // 0x47-0x4F: Reserved.
    //! Bank switch control register. 4 bytes, default: 0x0F708000
    BankSwitchCtrl = 0x50,
    // 0x51-0xF8: Reserved.
    //! Update device address register. 4 bytes, default: 0x00000036
    NewDevAddr = 0xF9,
    // 0xFA-0xFF: Reserved.
};

/**
 * @brief
 */
namespace I2CAddresses
{
[[maybe_unused]] static constexpr uint8_t TAS5707Default  = 0x36;
[[maybe_unused]] static constexpr uint8_t TAS5707Alt      = 0x38;
[[maybe_unused]] static constexpr uint8_t TAS5707ADefault = 0x3A;
[[maybe_unused]] static constexpr uint8_t TAS5707AAlt     = 0x3C;
}    // namespace I2CAddresses

/**
 * @addtogroup TAS5707 Clock Control Register
 * @{
 */

/**
 * @brief Enum listing the available sampling rate as well as their register values.
 */
namespace SampleRates
{
//! 32kHz.
[[maybe_unused]] static constexpr uint8_t Sr32kHz = 0b00000000;
//! 44.1/48kHz (Default).
[[maybe_unused]] static constexpr uint8_t Sr44_1Or48kHz = 0b01100000;
//! 16kHz.
[[maybe_unused]] static constexpr uint8_t Sr16kHz = 0b10000000;
//! 22.05/24kHz.
[[maybe_unused]] static constexpr uint8_t Sr22_05Or24kHz = 0b10100000;
//! 8kHz.
[[maybe_unused]] static constexpr uint8_t Sr8kHz = 0b11000000;
//! 11.025/12kHz.
[[maybe_unused]] static constexpr uint8_t Sr11_025Or12kHz = 0b11100000;

[[maybe_unused]] static constexpr bool IsSampleRate(uint8_t v)
{
    // A sample rate is valid if bits 0 to 4 are cleared.
    return ((v & 0x1F) == 0) && ((v & 0xE0) != 0x20) && ((v & 0xE0) != 0x40);
}
};    // namespace SampleRates

namespace MclkFrequencies
{
//! MCLK frequency = Fs * 64 (Only for 44.1kHz and 48kHz sample rate).
[[maybe_unused]] static constexpr uint8_t Fs64 = 0b00000000;
//! MCLK frequency = Fs * 128 (Only for 44.1kHz and 48kHz sample rate).
[[maybe_unused]] static constexpr uint8_t Fs128 = 0b00000100;
//! MCLK frequency = Fs * 192 (Only for 32/44.1/48kHz sample rate).
[[maybe_unused]] static constexpr uint8_t Fs192 = 0b00001000;
//! MCLK frequency = Fs * 256, not available at 8kHz (default).
[[maybe_unused]] static constexpr uint8_t Fs256 = 0b00001100;
//! MCLK frequency = Fs * 384.
[[maybe_unused]] static constexpr uint8_t Fs384 = 0b00010000;
//! MCLK frequency = Fs * 512.
[[maybe_unused]] static constexpr uint8_t Fs512 = 0b00010100;

[[maybe_unused]] static constexpr bool IsMclkFrequency(uint8_t v)
{
    // A MCLK frequency is valid if bits 0, 1, 5, 6 and 7 are cleared, and bits 3 are 4 are not both
    // set simultaneously.
    return ((v & 0xE3) == 0) && ((v & 0x18) != 0x18);
}
}    // namespace MclkFrequencies

/**
 * @}
 */

/**
 * @addtogroup TAS5707 Error Statuses
 * @{
 */
namespace ErrorCodes
{
//! Error on the master clock.
[[maybe_unused]] static constexpr uint8_t MclkError = 0x80;
//! Error on the PLL auto lock.
[[maybe_unused]] static constexpr uint8_t PllError = 0x40;
//! Error on the bit clock (SCLK).
[[maybe_unused]] static constexpr uint8_t SclkError = 0x20;
//! Error on the work clock (LRCLK).
[[maybe_unused]] static constexpr uint8_t LrclkError = 0x10;
//! Frame slip.
[[maybe_unused]] static constexpr uint8_t FrameSlip = 0x08;
//! Over-current, over-temperature, over-voltage or under-voltage error.
[[maybe_unused]] static constexpr uint8_t CriticalError = 0x02;
//! Over-temperature warning (Sets at around 125°).
[[maybe_unused]] static constexpr uint8_t TempWarning = 0x01;
}    // namespace ErrorCodes

/**
 * @}
 */

/**
 * @addtogroup TAS5707 System Control Register 1
 * @{
 */
namespace PwmDcBlock
{
//! PWM high-pass (dc blocking) disabled.
[[maybe_unused]] static constexpr uint8_t Disabled = 0x00;
//! PWM high-pass (dc blocking) enabled, -3dB @ <1Hz (Default).
[[maybe_unused]] static constexpr uint8_t Enabled = 0x80;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if bits 0 to 6 are clear.
    return (v & 0x7F) == 0;
}
}    // namespace PwmDcBlock

/**
 * @brief Mode of recovery after a clock error.
 */
namespace ClkErrRecoveryModes
{
//! Soft unmute on recovery from clock error.
[[maybe_unused]] static constexpr uint8_t Soft = 0x00;
//! Hard unmute on recovery from clock error (Default).
[[maybe_unused]] static constexpr uint8_t Hard = 0x20;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if all bits except bit 5 are clear.
    return (v & 0xDF) == 0;
}
}    // namespace ClkErrRecoveryModes

namespace DeEmphasisModes
{
//! No de-emphasis (Default).
[[maybe_unused]] static constexpr uint8_t None = 0x00;
//! De-emphasis for Fs = 32kHz.
[[maybe_unused]] static constexpr uint8_t Fs32kHz = 0x01;
//! De-emphasis for Fs = 48kHz.
[[maybe_unused]] static constexpr uint8_t Fs48kHz = 0x03;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if bits 2 to 7 are clear and bit 0 and 1 are not 0b10.
    return ((v & 0xFC) == 0) && ((v & 0x03) != 0x02);
}
}    // namespace DeEmphasisModes
/**
 * @}
 */

/**
 * @addtogroup TAS5707 Serial Data Interface Register
 * @{
 */
namespace SerialDataMode
{
//! Right justified, 16 bit data word.
[[maybe_unused]] static constexpr uint8_t RJ16Bits = 0x00;
//! Right justified, 20 bit data word.
[[maybe_unused]] static constexpr uint8_t RJ20Bits = 0x01;
//! Right justified, 24 bit data word.
[[maybe_unused]] static constexpr uint8_t RJ24Bits = 0x02;
//! I2S, 16 bit data word.
[[maybe_unused]] static constexpr uint8_t I2S16Bits = 0x03;
//! I2S, 20 bit data word.
[[maybe_unused]] static constexpr uint8_t I2S20Bits = 0x04;
//! I2S, 24 bit data word (Default).
[[maybe_unused]] static constexpr uint8_t I2S24Bits = 0x05;
//! Left justified, 16 bit data word.
[[maybe_unused]] static constexpr uint8_t LJ16Bits = 0x06;
//! Left justified, 20 bit data word.
[[maybe_unused]] static constexpr uint8_t LJ20Bits = 0x07;
//! Left justified, 24 bit data word.
[[maybe_unused]] static constexpr uint8_t LJ24Bits = 0x08;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if value is from 0 to 8.
    return v <= 0x08;
}
}    // namespace SerialDataMode
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 System Control Register 2
 * @{
 */
namespace ChannelEnable
{
//! Enter all channel shut down (Hard mute, default).
[[maybe_unused]] static constexpr uint8_t Disable = 0x40;
//! Exit all-channel shutdown (normal operation).
[[maybe_unused]] static constexpr uint8_t Enable = 0x00;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if all bits except bit 6 are clear.
    return (v & 0xBF) == 0;
}
}    // namespace ChannelEnable
     /**
      * @}
      */

/**
 * @addtogroup Soft Mute Register
 * @{
 */
namespace Ch1SoftMute
{
//! Soft mute channel 1.
[[maybe_unused]] static constexpr uint8_t Mute = 0x01;
//! Soft unmute channel 1 (Default).
[[maybe_unused]] static constexpr uint8_t Unmute = 0x00;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if all bits except bit 0 are clear.
    return (v & 0xFE) == 0;
}
}    // namespace Ch1SoftMute

namespace Ch2SoftMute
{
//! Soft mute channel 2.
[[maybe_unused]] static constexpr uint8_t Mute = 0x02;
//! Soft unmute channel 2 (Default).
[[maybe_unused]] static constexpr uint8_t Unmute = 0x00;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Valid if all bits except bit 2 are clear.
    return (v & 0xFD) == 0;
}
}    // namespace Ch2SoftMute
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Volume Registers
 */
namespace ChannelVolume
{
//! 0dB attenuation, default for individual channels.
[[maybe_unused]] static constexpr uint8_t NoAttenuation = 0b00110000;
//! Muted, default for master volume.
[[maybe_unused]] static constexpr uint8_t Muted = 0xFF;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // All values are valid, except between 0xCF and 0xFE.
    return (v < 0xCF) || (v > 0xFE);
}
}    // namespace ChannelVolume
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Master Fine Volume Register
 * @brief This register can be used to provide precision tuning of master volume.
 * @{
 */
namespace FineVolume
{
//! +0dB (Default).
[[maybe_unused]] static constexpr uint8_t DB0 = 0x80;
//! +0.125dB.
[[maybe_unused]] static constexpr uint8_t DB0_125 = 0x81;
//! +0.25dB.
[[maybe_unused]] static constexpr uint8_t DB0_25 = 0x82;
//! +0.375dB.
[[maybe_unused]] static constexpr uint8_t DB0_375 = 0x83;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Only values between 0x80 and 0x83 are valid.
    return (v >= 0x80) && (v <= 0x83);
}
}    // namespace FineVolume
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Volume Configuration Register
 * @brief Volume slew rate, used to control volume change and mute ramp rates.
 * This register controls the number of steps in a volume ramp. The rate at which these steps occurs
 * is dependent on the sample rate of the I2S data as follows: <table> <tr> <th>Sample Rate
 * (kHz)</th> <th>Approximate Ramp Rate</th>
 *  </tr>
 *  <tr>
 *      <td>8/16/32</td>
 *      <td>125us/step</td>
 *  </tr>
 *  <tr>
 *      <td>11.025/22.05/44.1</td>
 *      <td>90.7us/step</td>
 *  </tr>
 *  <tr>
 *      <td>12/24/48</td>
 *      <td>83.3us/step</td>
 *  </tr>
 * </table>
 * @{
 */
namespace VolumeSlewRates
{
//! Volume slew 512 steps (43ms volume ramp time at 48kHz).
[[maybe_unused]] static constexpr uint8_t Steps512 = 0x90;
//! Volume slew 1024 steps (85ms volume ramp time at 48kHz, Default).
[[maybe_unused]] static constexpr uint8_t Steps1024 = 0x91;
//! Volume slew 2048 steps (171ms volume ramp time at 48kHz).
[[maybe_unused]] static constexpr uint8_t Steps2048 = 0x92;
//! Volume slew 256 steps (21ms volume ramp time at 48kHz).
[[maybe_unused]] static constexpr uint8_t Steps256 = 0x93;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Rate is valid between 0x90 and 0x93.
    return (v >= 0x90) && (v <= 0x93);
}
}    // namespace VolumeSlewRates
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Modulation Limit Register
 * @brief The modulation limit is the maximum duty cycle of the PWM output waveform.
 * @{
 */
namespace ModulationLimit
{
//! 99.2%
[[maybe_unused]] static constexpr uint8_t P99_2 = 0x00;
//! 98.4%
[[maybe_unused]] static constexpr uint8_t P98_4 = 0x01;
//! 97.7% (Default).
[[maybe_unused]] static constexpr uint8_t P97_7 = 0x02;
//! 96.9%
[[maybe_unused]] static constexpr uint8_t P96_9 = 0x03;
//! 96.1%
[[maybe_unused]] static constexpr uint8_t P96_1 = 0x04;
//! 95.3%
[[maybe_unused]] static constexpr uint8_t P95_3 = 0x05;
//! 94.5%
[[maybe_unused]] static constexpr uint8_t P94_5 = 0x06;
//! 93.8%
[[maybe_unused]] static constexpr uint8_t P93_8 = 0x07;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    return (v <= 0x07);
}
}    // namespace ModulationLimit
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Inter-channel Delay Registers
 * @brief Internal PWM channels 1, 2, 1' and 2' are mapped into registers 0x11, 0x12, 0x13 and 0x14
 * respectively.
 *
 * @note These values have a high impact on audio performance (e.g., dynamic range, THD, crosstalk,
 * etc.). Therefore, appropriate ICD settings must be used. By default, the device has ICD settings
 * for AD mode. If used in BD mode, then update these registers before coming out of all-channel
 * shutdown.
 * @{
 */
namespace InterchannelDelays
{
//! Minimum absolute delay, 0 DCLK cycles.
[[maybe_unused]] static constexpr uint8_t MinimumDelay = 0x00;
//! Maximum positive delay, 31 * 4 DCLK cycles.
[[maybe_unused]] static constexpr uint8_t MaxPosDelay = 0x7C;
//! Maximum negative delay, -32 * 4 DCLK cycles.
[[maybe_unused]] static constexpr uint8_t MaxNegDelay = 0x80;
//! Default for channel 1 in AD mode (Default for 0x11).
[[maybe_unused]] static constexpr uint8_t Ch1ADMode = 0xAC;
//! Default for channel 1 in BD mode.
[[maybe_unused]] static constexpr uint8_t Ch1BDMode = 0xB8;
//! Default for channel 2 in AD mode (Default for 0x12).
[[maybe_unused]] static constexpr uint8_t Ch2ADMode = 0x54;
//! Default for channel 2 in BD mode.
[[maybe_unused]] static constexpr uint8_t Ch2BDMode = 0x60;
//! Default for channel 1' in AD mode (Default for 0x13).
[[maybe_unused]] static constexpr uint8_t Ch1PADMode = 0xAC;
//! Default for channel 1' in BD mode.
[[maybe_unused]] static constexpr uint8_t Ch1PBDMode = 0xA0;
//! Default for channel 2' in AD mode (Default for 0x14).
[[maybe_unused]] static constexpr uint8_t Ch2PADMode = 0x54;
//! Default for channel 2' in BD mode.
[[maybe_unused]] static constexpr uint8_t Ch2PBDMode = 0x48;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Any value is valid, as long as bit 0 and 1 are clear.
    return (v & 0x03) == 0;
}
}    // namespace InterchannelDelays
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Start/Stop Period Register
 * @brief This register is used to control the soft-start and soft-stop period following an
 * enter/exit all channel shut down command or change in the PDN state. This helps reduce pops and
 * clicks at start-up and shutdown. The times are only approximate and vary depending on device
 * activity level and I2S clock stability.
 * @{
 */
namespace StartStopPeriods
{
//! No 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t None = 0x00;
//! 16.5-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms16_5 = 0x08;
//! 23.9-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms23_9 = 0x09;
//! 31.4-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms31_4 = 0x0A;
//! 40.4-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms40_4 = 0x0B;
//! 53.9-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms53_9 = 0x0C;
//! 70.3-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms70_3 = 0x0D;
//! 94.2-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms94_2 = 0x0E;
//! 125.7-ms 50% duty cycle start/stop period (Default).
[[maybe_unused]] static constexpr uint8_t Ms125_7 = 0x0F;
//! 164.6-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms164_6 = 0x10;
//! 239.4-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms239_4 = 0x11;
//! 314.2-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms314_2 = 0x12;
//! 403.9-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms403_9 = 0x13;
//! 538.6-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms538_6 = 0x14;
//! 703.1-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms703_1 = 0x15;
//! 942.5-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms942_5 = 0x16;
//! 1256.6-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms1256_6 = 0x17;
//! 1728.1-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms1728_1 = 0x18;
//! 2513.6-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms2513_6 = 0x19;
//! 3299.1-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms3299_1 = 0x1A;
//! 4241.7-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms4241_7 = 0x1B;
//! 5655.6-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms5655_6 = 0x1C;
//! 7383.7-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms7383_7 = 0x1D;
//! 9897.3-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms9897_3 = 0x1E;
//! 13,196.4-ms 50% duty cycle start/stop period
[[maybe_unused]] static constexpr uint8_t Ms13196_4 = 0x1F;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Any value between 0x08 and 0x1F, as well as 0x00 are valid.
    return (v == 0x00) || ((v >= 0x08) && (v <= 0x1F));
}
}    // namespace StartStopPeriods
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Oscillator Trim Register
 * @brief Indicates the status of the TAS5707's internal oscillator used to automatically detect I2S
 * clock rates.
 * @{
 */
namespace OscillatorTrimStates
{
//! Oscillator trimming has not been done (Default).
[[maybe_unused]] static constexpr uint8_t NotDone = 0x00;
//! Oscillator trimming is complete.
[[maybe_unused]] static constexpr uint8_t Done = 0x40;
}    // namespace OscillatorTrimStates

namespace OscillatorTrimSelect
{
//! Select factory trim.
[[maybe_unused]] static constexpr uint8_t Factory = 0x00;
//! Factory trim is disabled (Default).
[[maybe_unused]] static constexpr uint8_t Disabled = 0x02;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    return (v & 0xFD) == 0;
}
}    // namespace OscillatorTrimSelect
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 BKND_ERR Register
 * @brief This register sets the time that the modulator waits before attempting to re-start the
 * power stage following the reception of a back-end error signal from the internal power stage.
 * @{
 */
namespace BackEndRestartDelays
{
//! Reset period = 299ms (Default).
[[maybe_unused]] static constexpr uint8_t Ms299 = 0x02;
//! Reset period = 449ms.
[[maybe_unused]] static constexpr uint8_t Ms449 = 0x03;
//! Reset period = 598ms.
[[maybe_unused]] static constexpr uint8_t Ms598 = 0x04;
//! Reset period = 748ms.
[[maybe_unused]] static constexpr uint8_t Ms748 = 0x05;
//! Reset period = 898ms.
[[maybe_unused]] static constexpr uint8_t Ms898 = 0x06;
//! Reset period = 1047ms.
[[maybe_unused]] static constexpr uint8_t Ms1047 = 0x07;
//! Reset period = 1197ms.
[[maybe_unused]] static constexpr uint8_t Ms1197 = 0x08;
//! Reset period = 1346ms.
[[maybe_unused]] static constexpr uint8_t Ms1346 = 0x09;
//! Reset period = 1496ms.
[[maybe_unused]] static constexpr uint8_t Ms1496 = 0x0A;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    return (v >= 0x02) && (v <= 0x0A);
}
}    // namespace BackEndRestartDelays
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Input Multiplexer Register
 * @brief This register controls the modulation scheme (AD or BD mode) as well as the routing of I2S
 * audio to the internal channels.
 * @{
 */

namespace Channel1ModulationModes
{
//! Channel-1 AD mode (Default).
[[maybe_unused]] static constexpr uint32_t AD = 0x00000000;
//! Channel-1 BD mode.
[[maybe_unused]] static constexpr uint32_t BD = 0x00800000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFF7FFFFF) == 0;
}
}    // namespace Channel1ModulationModes

namespace Channel2ModulationModes
{
//! Channel-2 AD mode (Default).
[[maybe_unused]] static constexpr uint32_t AD = 0x00000000;
//! Channel-2 BD mode.
[[maybe_unused]] static constexpr uint32_t BD = 0x00080000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFF7FFFF) == 0;
}
}    // namespace Channel2ModulationModes

namespace Channel1InputSources
{
//! SDIN-L to channel 1 (Default).
[[maybe_unused]] static constexpr uint32_t SDIN_L = 0x00000000;
//! SDIN-R to channel 1.
[[maybe_unused]] static constexpr uint32_t SDIN_R = 0x00100000;
//! Channel 1 to ground.
[[maybe_unused]] static constexpr uint32_t GND = 0x00600000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v == SDIN_L) || (v == SDIN_R) || (v == GND);
}
}    // namespace Channel1InputSources

namespace Channel2InputSources
{
//! SDIN-L to channel 2 (Default).
[[maybe_unused]] static constexpr uint32_t SDIN_L = 0x00000000;
//! SDIN-R to channel 2.
[[maybe_unused]] static constexpr uint32_t SDIN_R = 0x00010000;
//! Channel 2 to ground.
[[maybe_unused]] static constexpr uint32_t GND = 0x00060000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v == SDIN_L) || (v == SDIN_R) || (v == GND);
}
}    // namespace Channel2InputSources
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 PWM Output Mux Register
 * @brief This register selects which internal PWM channel is output to the external pins. Any
 * channel can be output to any external output pin.
 * @{
 */
namespace OutASources
{
//! Multiplex channel 1 to OUT_A (Default).
[[maybe_unused]] static constexpr uint32_t Ch1 = 0x00000000;
//! Multiplex channel 2 to OUT_A.
[[maybe_unused]] static constexpr uint32_t Ch2 = 0x00100000;
//! Multiplex channel 1' to OUT_A.
[[maybe_unused]] static constexpr uint32_t Ch1P = 0x00200000;
//! Multiplex channel 2' to OUT_A.
[[maybe_unused]] static constexpr uint32_t Ch2P = 0x00300000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFCFFFFF) == 0;
}
}    // namespace OutASources

namespace OutBSources
{
//! Multiplex channel 1 to OUT_B.
[[maybe_unused]] static constexpr uint32_t Ch1 = 0x00000000;
//! Multiplex channel 2 to OUT_B.
[[maybe_unused]] static constexpr uint32_t Ch2 = 0x00010000;
//! Multiplex channel 1' to OUT_B (Default).
[[maybe_unused]] static constexpr uint32_t Ch1P = 0x00020000;
//! Multiplex channel 2' to OUT_B.
[[maybe_unused]] static constexpr uint32_t Ch2P = 0x00030000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFCFFFF) == 0;
}
}    // namespace OutBSources

namespace OutCSources
{
//! Multiplex channel 1 to OUT_C.
[[maybe_unused]] static constexpr uint32_t Ch1 = 0x00000000;
//! Multiplex channel 2 to OUT_C (Default).
[[maybe_unused]] static constexpr uint32_t Ch2 = 0x00001000;
//! Multiplex channel 1' to OUT_C.
[[maybe_unused]] static constexpr uint32_t Ch1P = 0x00002000;
//! Multiplex channel 2' to OUT_C.
[[maybe_unused]] static constexpr uint32_t Ch2P = 0x00003000;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFFCFFF) == 0;
}
}    // namespace OutCSources

namespace OutDSources
{
//! Multiplex channel 1 to OUT_D.
[[maybe_unused]] static constexpr uint32_t Ch1 = 0x00000000;
//! Multiplex channel 2 to OUT_D.
[[maybe_unused]] static constexpr uint32_t Ch2 = 0x00000100;
//! Multiplex channel 1' to OUT_D.
[[maybe_unused]] static constexpr uint32_t Ch1P = 0x00000200;
//! Multiplex channel 2' to OUT_D (Default).
[[maybe_unused]] static constexpr uint32_t Ch2P = 0x00000300;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFFFCFF) == 0;
}
}    // namespace OutDSources
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 DRC Control Register
 * @brief Each DRC (Dynamic Range Control) can be enabled independently using the DRC control
 * register. The DRCs are disabled by default.
 * @{
 */
namespace DrcModes
{
//! DRC Turned OFF (Default).
[[maybe_unused]] static constexpr uint32_t Disabled = 0x00000000;
//! DRC Turned ON.
[[maybe_unused]] static constexpr uint32_t Enabled = 0x00000001;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFFFFFE) == 0;
}
}    // namespace DrcModes
     /**
      * @}
      */

/**
 * @addtogroup TAS5707 Bank Switch and EQ Control Register
 * @brief This register controls the automatic bank switching for different sample rates as well as
 * controlling the behavior of the EQ and biquad filters.
 * @{
 */
namespace BankSwitchingModes
{
//! No bank switching. All updates to DAP (Default).
[[maybe_unused]] static constexpr uint32_t NoSwitch = 0x00000000;
//! Configure bank 1 (32kHz by default).
[[maybe_unused]] static constexpr uint32_t Bank1 = 0x00000001;
//! Configure bank 2 (44.1/48 kHz by default).
[[maybe_unused]] static constexpr uint32_t Bank2 = 0x00000002;
//! Configure bank 3 (other sample rates by default).
[[maybe_unused]] static constexpr uint32_t Bank3 = 0x00000003;
//! Automatic bank selection.
[[maybe_unused]] static constexpr uint32_t Automatic = 0x00000004;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return v <= 4;
}
}    // namespace BankSwitchingModes

namespace BankMappingOverrides
{
//! Ignore bank-mapping, use the default mapping instead (Default).
[[maybe_unused]] static constexpr uint32_t Default = 0x00000000;
//! Use the overridden bank mapping.
[[maybe_unused]] static constexpr uint32_t Override = 0x00000020;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFFFFDF) == 0;
}
}    // namespace BankMappingOverrides

namespace EqModes
{
//! EQ ON (Default).
[[maybe_unused]] static constexpr uint32_t Enabled = 0x00000000;
//! EQ OFF (Bypasses BQ 0-6 of channels 1 and 2).
[[maybe_unused]] static constexpr uint32_t Disabled = 0x00000080;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFFFF7F) == 0;
}
}    // namespace EqModes

namespace BiquadModes
{
//! L and R filters are independent (Default).
[[maybe_unused]] static constexpr uint32_t Independent = 0x00000000;
//! L and R filters are ganged for EQ biquads: a write to left channel's BQ is also written to the
//! corresponding BQ of the right channel.
[[maybe_unused]] static constexpr uint32_t Ganged = 0x00000010;

[[maybe_unused]] static constexpr bool IsValid(uint32_t v)
{
    return (v & 0xFFFFFFEF) == 0;
}
}    // namespace BiquadModes

/**
 * @brief Selects which sample rates uses a specific bank.
 *
 * A bank can be used by multiple sample rates.
 *
 * The default values are:
 * <ul>
 *  <li>Bank 1:
 *      <ul>
 *          <li>32kHz</li>
 *      </ul>
 *  </li>
 *  <li>Bank 2:
 *      <ul>
 *          <li>44.1/48kHz</li>
 *      </ul>
 *  </li>
 *  <li>Bank 3:
 *      <ul>
 *          <li>8kHz</li>
 *          <li>11.025/12kHz</li>
 *          <li>16kHz</li>
 *          <li>22.025/24kHz</li>
 *      </ul>
 *  </li>
 */
namespace BankSampleRates
{
//! 32kHz uses bank.
[[maybe_unused]] static constexpr uint8_t Use32kHz = 0x80;
//! 44.1/48kHz uses bank.
[[maybe_unused]] static constexpr uint8_t Use44_1Or48kHz = 0x10;
//! 16kHz uses bank.
[[maybe_unused]] static constexpr uint8_t Use16kHz = 0x08;
//! 22.025/24kHz uses bank.
[[maybe_unused]] static constexpr uint8_t Use22_025Or24kHz = 0x04;
//! 8kHz uses bank.
[[maybe_unused]] static constexpr uint8_t Use8kHz = 0x02;
//! 11.025/12kHz uses bank.
[[maybe_unused]] static constexpr uint8_t Use11_025Or12kHz = 0x01;

[[maybe_unused]] static constexpr bool IsValid(uint8_t v)
{
    // Bits 5 and 6 must be 0b00.
    return (v & 0x60) == 0;
}

//! Bank 3 is used by 8kHz, 11.025/12kHz, 16kHz and 22.025/24kHz by default.
[[maybe_unused]] static constexpr uint8_t Bank3Default = 0x0F;
//! Bank 2 is used by 48kHz by default.
[[maybe_unused]] static constexpr uint8_t Bank2Default = 0x10;
//! Bank 1 is used by 32kHz by default.
[[maybe_unused]] static constexpr uint8_t Bank1Default = 0x80;

[[maybe_unused]] static constexpr uint32_t ToBank1(uint8_t v)
{
    // Bank 1 is bits 8 to 15.
    return (uint32_t)v << 8;
}

[[maybe_unused]] static constexpr uint32_t ToBank2(uint8_t v)
{
    // Bank 2 is bits 16 to 23.
    return ((uint32_t)v << 16) | 0x00600000;
}

[[maybe_unused]] static constexpr uint32_t ToBank3(uint8_t v)
{
    // Bank 3 is bits 24 to 31.
    return (uint32_t)v << 24;
}
}    // namespace BankSampleRates
/**
 * @}
 */
}    // namespace cep::Tas5707
#endif

#endif    // NILAI_TAS5707_ENUMS_H
