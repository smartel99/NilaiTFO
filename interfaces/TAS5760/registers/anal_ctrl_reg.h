/**
 * @file    anal_ctrl_reg.h
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

#ifndef GUARD_NILAI_INTERFACES_TAS5760_ANAL_CTRL_REG_H
#define GUARD_NILAI_INTERFACES_TAS5760_ANAL_CTRL_REG_H

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
 * @struct AnalCtrlReg
 * @brief Register controlling analog-related options.
 *
 * Refer to the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5760md.pdf?ts=1660571967342&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTAS5760MD>
 * datasheet</a> for more information.
 */
struct AnalCtrlReg
{
    const uint8_t Reserved : 1 = 1;
    /**
     * @brief Channel Selection for PBTL Mode
     *
     * <br>
     * @b 0: When placed in PBTL mode, the audio information from the right channel of the
     * serial audio input stream is used by the speaker amplifier. <br>
     * @b 1: When placed in PBTL mode, the audio information from the left channel of the serial
     * audio input stream is used by the speaker amplifier.
     */
    uint8_t PbtlChSel : 1 = 0;
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
    uint8_t AGain : 2 = 0;
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
    uint8_t PwmRateSel : 3 = 0b101;
    /**
     * @brief PBTL Enable
     *
     * <br>
     * @b 0: Device is placed in BTL (Bridge-Tied Load) mode.
     * <br>
     * @b 1: Device is placed in PBTL (Parallel Bridge-Tied Load) mode.
     */
    uint8_t PbtlEnable : 1 = 0;

    /**
     * @brief Default constructor.
     *
     * Default values taken from the datasheet:
     * - PBTL Ch Sel: 0b0
     * - A_GAIN: 0b0
     * - PWM Rate Select: 0b101
     * - PBTL Enable: 0b0
     */
    constexpr AnalCtrlReg() noexcept = default;

    /**
     * @brief Constructs the structure from the passed byte.
     * @param v The data to initialize the struct with.
     */
    constexpr explicit AnalCtrlReg(uint8_t v) noexcept
    : PbtlChSel((v & PbtlChSelMask) >> PbtlChSelShift),
      AGain((v & AGainMask) >> AGainShift),
      PwmRateSel((v & PwmRateSelMask) >> PwmRateSelShift),
      PbtlEnable((v & PbtlEnMask) >> PbtlEnShift)
    {
    }

    /**
     * @brief Converts the structure into a uint8_t.
     * @return The uint8_t representing the structure.
     */
    [[nodiscard]] explicit constexpr operator uint8_t() const noexcept
    {
        return (PbtlChSel << PbtlChSelShift) | (AGain << AGainShift) |
               (PwmRateSel << PwmRateSelShift) | (PbtlEnable << PbtlEnShift);
    }

    /**
     * @brief Checks if two key status structures are identical.
     * @param o The other structure.
     * @returns True if the structures are identical.
     * @returns False if the structures are different.
     */
    constexpr bool operator==(const AnalCtrlReg& o) const noexcept
    {
        return static_cast<uint8_t>(*this) == static_cast<uint8_t>(o);
    }

    /**
     * @brief Checks if two key status structures are different.
     * @param o The other structure.
     * @returns True if the structures are different.
     * @returns False if the structures are identical.
     */
    constexpr bool operator!=(const AnalCtrlReg& o) const noexcept { return !(*this == o); }

private:
    static constexpr uint8_t PbtlChSelMask = 0x02;    //!< Bit-wise mask for PBTL channel selection.
    static constexpr uint8_t AGainMask     = 0x0C;    //!< Bit-wise mask for Analog Gain.
    static constexpr uint8_t PwmRateSelMask = 0x70;    //!< Bit-wise mask for PWM Rate Selection.
    static constexpr uint8_t PbtlEnMask     = 0x80;    //!< Bit-wise mask for PBTL enable.

    static constexpr uint8_t PbtlChSelShift  = 1;    //!< Bit-wise shift for PBTL channel selection.
    static constexpr uint8_t AGainShift      = 2;    //!< Bit-wise shift for Analog Gain.
    static constexpr uint8_t PwmRateSelShift = 4;    //!< Bit-wise shift for PWM Rate Selection.
    static constexpr uint8_t PbtlEnShift     = 7;    //!< Bit-wise shift for PBTL enable.
};
}    // namespace Nilai::Interfaces::TAS5760

//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_TAS5760_ANAL_CTRL_REG_H
