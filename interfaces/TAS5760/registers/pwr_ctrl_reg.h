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

#ifndef GUARD_NILAI_INTERFACES_TAS5760_PWR_CTRL_REG_H
#define GUARD_NILAI_INTERFACES_TAS5760_PWR_CTRL_REG_H

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
 * @struct PwrCtrlReg
 * @brief Register controlling power-related options.
 *
 * Refer to the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5760md.pdf?ts=1660571967342&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTAS5760MD>
 * datasheet</a> for more information.
 */
struct PwrCtrlReg
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

    /**
     * @brief Default constructor.
     */
    constexpr PwrCtrlReg() noexcept = default;

    /**
     * @brief Constructs the structure from the passed byte.
     * @param v The data to initialize the struct with.
     */
    constexpr explicit PwrCtrlReg(uint8_t v) noexcept
    {
        SpkShutdown = (v & SpkShutdownMask) >> SpkShutdownShift;
        SpkSleep    = (v & SpkSleepMask) >> SpkSleepShift;
        DigClipLev  = (v & DigClipLevMask) >> DigClipLevShift;
    }

    /**
     * @brief Converts the structure into a uint16_t.
     * @return The uint16_t representing the structure.
     */
    [[nodiscard]] explicit constexpr operator uint8_t() const noexcept
    {
        return (SpkShutdown << SpkShutdownShift) | (SpkSleep << SpkSleepShift) |
               (DigClipLev << DigClipLevShift);
    }

    /**
     * @brief Checks if two key status structures are identical.
     * @param o The other structure.
     * @returns True if the structures are identical.
     * @returns False if the structures are different.
     */
    constexpr bool operator==(const PwrCtrlReg& o) const noexcept
    {
        return static_cast<uint8_t>(*this) == static_cast<uint8_t>(o);
    }

    /**
     * @brief Checks if two key status structures are different.
     * @param o The other structure.
     * @returns True if the structures are different.
     * @returns False if the structures are identical.
     */
    constexpr bool operator!=(const PwrCtrlReg& o) const noexcept { return !(*this == o); }

private:
    static constexpr uint8_t SpkShutdownMask = 0x01;    //!< Bit-wise mask for SPK_SD.
    static constexpr uint8_t SpkSleepMask    = 0x02;    //!< Bit-wise mask for SPK_SLEEP.
    static constexpr uint8_t DigClipLevMask  = 0xFC;    //!< Bit-wise mask for DigClipLev[19:14].

    static constexpr uint8_t SpkShutdownShift = 0;    //!< Bit-wise shift for SPK_SD.
    static constexpr uint8_t SpkSleepShift    = 1;    //!< Bit-wise shift for SPK_SLEEP.
    static constexpr uint8_t DigClipLevShift  = 2;    //!< Bit-wise shift for DigClipLev[19:14].
};
}    // namespace Nilai::Interfaces::TAS5760

//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_TAS5760_PWR_CTRL_REG_H
