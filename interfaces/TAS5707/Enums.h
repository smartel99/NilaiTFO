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
namespace ClkErrRecoveryMode
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
}    // namespace ClkErrRecoveryMode

namespace DeEmphasisMode
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
}    // namespace DeEmphasisMode
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

}    // namespace cep::Tas5707
#endif

#endif    // NILAI_TAS5707_ENUMS_H
