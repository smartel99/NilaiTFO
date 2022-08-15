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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */

#ifndef GUARD_NILAI_INTERFACES_TAS5760_FAULT_CFG_AND_ERR_STATUS_REG_H
#define GUARD_NILAI_INTERFACES_TAS5760_FAULT_CFG_AND_ERR_STATUS_REG_H

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
 * @struct FaultCfgAndErrStatusReg
 * @brief Register controlling global volume options.
 *
 * Refer to the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5760md.pdf?ts=1660571967342&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTAS5760MD>
 * datasheet</a> for more information.
 */
struct FaultCfgAndErrStatusReg
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
    uint8_t : 2;

    /**
     * @brief Default constructor.
     */
    constexpr FaultCfgAndErrStatusReg() noexcept = default;

    /**
     * @brief Constructs the structure from the passed byte.
     * @param v The data to initialize the struct with.
     */
    constexpr explicit FaultCfgAndErrStatusReg(uint8_t v) noexcept
    {
        OverTemp       = (v & OverTempMask) >> OverTempShift;
        DCErr          = (v & DCErrMask) >> DCErrShift;
        OverCurr       = (v & OverCurrMask) >> OverCurrShift;
        ClkErr         = (v & ClkErrMask) >> ClkErrShift;
        OverCurrThresh = (v & OverCurrThreshMask) >> OverCurrThreshShift;
    }

    /**
     * @brief Converts the structure into a uint16_t.
     * @return The uint16_t representing the structure.
     */
    [[nodiscard]] explicit constexpr operator uint8_t() const noexcept
    {
        return (OverTemp << OverTempShift) | (DCErr << DCErrShift) | (OverCurr << OverCurrShift) |
               (ClkErr << ClkErrShift) | (OverCurrThresh << OverCurrThreshShift);
    }

    /**
     * @brief Checks if two key status structures are identical.
     * @param o The other structure.
     * @returns True if the structures are identical.
     * @returns False if the structures are different.
     */
    constexpr bool operator==(const FaultCfgAndErrStatusReg& o) const noexcept
    {
        return static_cast<uint8_t>(*this) == static_cast<uint8_t>(o);
    }

    /**
     * @brief Checks if two key status structures are different.
     * @param o The other structure.
     * @returns True if the structures are different.
     * @returns False if the structures are identical.
     */
    constexpr bool operator!=(const FaultCfgAndErrStatusReg& o) const noexcept
    {
        return !(*this == o);
    }

private:
    static constexpr uint8_t OverTempMask       = 0x01;    //!< Bit-wise mask for Over temperature.
    static constexpr uint8_t DCErrMask          = 0x02;    //!< Bit-wise mask for DC Error.
    static constexpr uint8_t OverCurrMask       = 0x04;    //!< Bit-wise mask for Over current.
    static constexpr uint8_t ClkErrMask         = 0x08;    //!< Bit-wise mask for Clock error.
    static constexpr uint8_t OverCurrThreshMask = 0x30;    //!< Bit-wise mask for Over current
                                                           //!< threshold.

    static constexpr uint8_t OverTempShift       = 0;    //!< Bit-wise shift for Over temperature.
    static constexpr uint8_t DCErrShift          = 1;    //!< Bit-wise shift for DC Error.
    static constexpr uint8_t OverCurrShift       = 2;    //!< Bit-wise shift for Over current.
    static constexpr uint8_t ClkErrShift         = 3;    //!< Bit-wise shift for Clock error.
    static constexpr uint8_t OverCurrThreshShift = 4;    //!< Bit-wise shift for Over current
                                                         //!< threshold.
};
}    // namespace Nilai::Interfaces::TAS5760

//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_TAS5760_FAULT_CFG_AND_ERR_STATUS_REG_H
