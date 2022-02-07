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
#ifndef NILAI_TAS5705_ENUMS_H
#define NILAI_TAS5705_ENUMS_H

#if defined(NILAI_USE_TAS5705)

#    include <cstdint>

namespace cep::Tas5705
{
/**
* @addtogroup TAS5705 Clock Control Register
* @{
*/

/**
* @brief Enum listing the available sampling rate as well as their register values.
*/
namespace SampleRates
{
//! 32kHz.
[[maybe_unused]] static constexpr uint8_t Sr32kHz = 0b00000000;
//! 38kHz.
[[maybe_unused]] static constexpr uint8_t Sr38kHz = 0b00100000;
//! 44.1kHz.
[[maybe_unused]] static constexpr uint8_t Sr44_1kHz = 0b01000000;
//! 48kHz (Default).
[[maybe_unused]] static constexpr uint8_t Sr48kHz = 0b01100000;
//! 88.2kHz.
[[maybe_unused]] static constexpr uint8_t Sr88_2kHz = 0b10000000;
//! 96kHz.
[[maybe_unused]] static constexpr uint8_t Sr96kHz = 0b10100000;
//! 176.4kHz.
[[maybe_unused]] static constexpr uint8_t Sr176_4kHz = 0b11000000;
//! 192kHz.
[[maybe_unused]] static constexpr uint8_t Sr192kHz = 0b11100000;

[[maybe_unused]] static constexpr bool IsSampleRate(uint8_t v)
{
   // A sample rate is valid if bits 0 to 4 are cleared.
   return (v & 0x1F) == 0;
}
};    // namespace SampleRates

namespace MclkFrequencies
{
//! MCLK frequency = Fs * 64 (Not available for 32-, 44.1- and 48-kHz data rates).
[[maybe_unused]] static constexpr uint8_t Fs64 = 0b00000000;
//! MCLK frequency = Fs * 128 (Not available for 32kHz data rate).
[[maybe_unused]] static constexpr uint8_t Fs128 = 0b00000100;
//! MCLK frequency = Fs * 192.
[[maybe_unused]] static constexpr uint8_t Fs192 = 0b00001000;
//! MCLK frequency = Fs * 256 (default).
[[maybe_unused]] static constexpr uint8_t Fs256 = 0b00001100;
//! MCLK frequency = Fs * 384.
[[maybe_unused]] static constexpr uint8_t Fs384 = 0b00010000;
//! MCLK frequency = Fs * 512 (Not available for 176.4kHz and 192kHz data rates).
[[maybe_unused]] static constexpr uint8_t Fs512 = 0b00010100;

[[maybe_unused]] static constexpr bool IsMclkFrequency(uint8_t v)
{
   // A MCLK frequency is valid if bits 0, 1, 5, 6 and 7 are cleared, and bits 3 are 4 are not both
   // set simultaneously.
   return ((v & 0xE3) == 0) && ((v & 0x18) != 0x18);
}
}    // namespace MclkFrequencies

namespace SclkFrequencies
{
//! Bit clock (SCLK) frequency = 64 * Fs or 32 * Fs, selected in register 0x04 (Default).
[[maybe_unused]] static constexpr uint8_t Fs64OrFs32 = 0b00000000;
//! Bit clock (SCLK) frequency = 48 * Fs (Only available for MCLK frequencies 192*Fs and 384*Fs).
[[maybe_unused]] static constexpr uint8_t Fs48 = 0b00000010;

[[maybe_unused]] static constexpr bool IsSclkFrequency(uint8_t v)
{
   // A SCLK frequency is valid is all bits except bit 1 are cleared.
   return (v & 0xFD) == 0;
}
}    // namespace SclkFrequencies

namespace ClockStatuses
{
//! Clock not valid, in manual mode only (Default).
[[maybe_unused]] static constexpr uint8_t ClockNotValid = 0x00;
//! Clock valid, in manual mode only.
[[maybe_unused]] static constexpr uint8_t ClockValid = 0x01;

[[maybe_unused]] static constexpr bool IsClockStatus(uint8_t v)
{
   return (v & 0xFE) == 0;
}
}    // namespace ClockStatuses
/**
* @}
*/


}    // namespace cep::Tas5705
#endif

#endif    // NILAI_TAS5705_ENUMS_H
