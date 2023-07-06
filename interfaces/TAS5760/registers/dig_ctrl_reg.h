/**
 * @file    pwr_ctrl_reg.h
 * @author  Samuel Martel
 * @date    2022-08-15
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

#ifndef GUARD_NILAI_INTERFACES_TAS5760_DIG_CTRL_REG_H
#define GUARD_NILAI_INTERFACES_TAS5760_DIG_CTRL_REG_H

#if defined(NILAI_USE_SW_TAS5760)
#    include <cstdint>

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup TAS5760
 * @{
 */

namespace Nilai::Interfaces::TAS5760
{
/**
 * @struct DigCtrlReg
 * @brief Register controlling the digital options.
 *
 * Refer to the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5760md.pdf?ts=1660571967342&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTAS5760MD>
 * datasheet</a> for more information.
 */
struct DigCtrlReg
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
    uint8_t InFmt : 3 = 0b100;
    /**
     * @brief Single Speed/Double Speed Mode Select
     *
     * <br>
     * @b 0: Serial audio port will accept single speed sample rates (that is: 32kHz, 44.1kHz
     * and 48kHz). <br>
     * @b 1: Serial audio port will accept double speed sample rates (that is: 88.2kHz and
     * 96kHz).
     */
    uint8_t SsDs : 1 = 0;
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
    uint8_t DigitalBoost : 2 = 0b01;

    const uint8_t Reserved : 1 = 0;
    /**
     * @brief High-Pass Filter Bypass.
     * <br>
     * @b 0: The internal high-pass filter in the digital path is not bypassed.
     * <br>
     * @b 1: The internal high-pass filter in the digital path is bypassed.
     */
    uint8_t HpfBypass : 1 = 0;

    /**
     * @brief Default constructor.
     *
     * Default values taken from the datasheet:
     * - Serial Audio Input Format: 0b100
     * - SS/DS: 0b0
     * - Digital Boost: 0b01
     * - Reserved: 0b0
     * - HPF Bypass: 0b0
     */
    constexpr DigCtrlReg() noexcept = default;

    /**
     * @brief Constructs the structure from the passed byte.
     * @param v The data to initialize the struct with.
     */
    constexpr explicit DigCtrlReg(uint8_t v) noexcept
    : InFmt((v & InFmtMask) >> InFmtShift),
      SsDs((v & SsDsMask) >> SsDsShift),
      DigitalBoost((v & DigitalBoostMask) >> DigitalBoostShift),
      HpfBypass((v & HpfBypassMask) >> HpfBypassShift)
    {
    }

    /**
     * @brief Converts the structure into a uint16_t.
     * @return The uint16_t representing the structure.
     */
    [[nodiscard]] explicit constexpr operator uint8_t() const noexcept
    {
        return (HpfBypass << HpfBypassShift) | (DigitalBoost << DigitalBoostShift) |
               (InFmt << InFmtShift);
    }

    /**
     * @brief Checks if two key status structures are identical.
     * @param o The other structure.
     * @returns True if the structures are identical.
     * @returns False if the structures are different.
     */
    constexpr bool operator==(const DigCtrlReg& o) const noexcept
    {
        return static_cast<uint8_t>(*this) == static_cast<uint8_t>(o);
    }

    /**
     * @brief Checks if two key status structures are different.
     * @param o The other structure.
     * @returns True if the structures are different.
     * @returns False if the structures are identical.
     */
    constexpr bool operator!=(const DigCtrlReg& o) const noexcept { return !(*this == o); }

private:
    static constexpr uint8_t HpfBypassMask    = 0x80;    //!< Bit-wise mask for HPF Bypass.
    static constexpr uint8_t DigitalBoostMask = 0x30;    //!< Bit-wise mask for Digital Boost.
    static constexpr uint8_t SsDsMask         = 0x08;    //!< Bit-wise mask for SS/DS.
    static constexpr uint8_t InFmtMask = 0x07;    //!< Bit-wise mask for Serial Audio Input Format.

    static constexpr uint8_t HpfBypassShift    = 7;    //!< Bit-wise shift for HPF Bypass.
    static constexpr uint8_t DigitalBoostShift = 4;    //!< Bit-wise shift for Digital Boost.
    static constexpr uint8_t SsDsShift         = 3;    //!< Bit-wise shift for SS/DS.
    static constexpr uint8_t InFmtShift        = 0;    //!< Bit-wise shift for Serial Audio
                                                       //!< Input Format.
};
}    // namespace Nilai::Interfaces::TAS5760

//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_TAS5760_DIG_CTRL_REG_H
