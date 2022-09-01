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

#ifndef GUARD_NILAI_INTERFACES_TAS5760_VOL_CTRL_CONFIG_REG_H
#define GUARD_NILAI_INTERFACES_TAS5760_VOL_CTRL_CONFIG_REG_H

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
 * @struct VolCtrConfigReg
 * @brief Register controlling global volume options.
 *
 * Refer to the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5760md.pdf?ts=1660571967342&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTAS5760MD>
 * datasheet</a> for more information.
 */
struct VolCtrConfigReg
{
    /**
     * @brief Mute Left Channel.
     *
     * <br>
     * @b 0: The left channel is not muted.
     * <br>
     * @b 1: The left channel is muted. In software mute, most analog and digital blocks remain
     * active and the speaker amplifier outputs transition to a 50/50 duty cycle.
     */
    uint8_t MuteL : 1 = 0;
    /**
     * @brief Mute Right Channel.
     *
     * <br>
     * @b 0: The right channel is not muted.
     * <br>
     * @b 1: The right channel is muted. In software mute, most analog and digital blocks remain
     * active and the speaker amplifier outputs transition to a 50/50 duty cycle.
     */
    uint8_t MuteR : 1 = 0;

    const uint8_t Reserved : 5 = 0b00000;
    /**
     * @brief Volume Fade Enable
     *
     * <br>
     * @b 0: Volume fading is disabled.
     * <br>
     * @b 1: Volume fading is enabled.
     */
    uint8_t Fade : 1 = 1;

    /**
     * @brief Default constructor.
     *
     * Default values taken from the datasheet:
     * - Mute L : 0b0
     * - Mute R : 0b0
     * - Fade : 0b1
     */
    constexpr VolCtrConfigReg() noexcept = default;

    /**
     * @brief Constructs the structure from the passed byte.
     * @param v The data to initialize the struct with.
     */
    constexpr explicit VolCtrConfigReg(uint8_t v) noexcept
    : MuteL((v & MuteLMask) >> MuteLShift),
      MuteR((v & MuteRMask) >> MuteRShift),
      Fade((v & FadeMask) >> FadeShift)
    {
    }

    /**
     * @brief Converts the structure into a uint8_t.
     * @return The uint8_t representing the structure.
     */
    [[nodiscard]] explicit constexpr operator uint8_t() const noexcept
    {
        return (MuteL << MuteLShift) | (MuteR << MuteRShift) | (Fade << FadeShift);
    }

    /**
     * @brief Checks if two key status structures are identical.
     * @param o The other structure.
     * @returns True if the structures are identical.
     * @returns False if the structures are different.
     */
    constexpr bool operator==(const VolCtrConfigReg& o) const noexcept
    {
        return static_cast<uint8_t>(*this) == static_cast<uint8_t>(o);
    }

    /**
     * @brief Checks if two key status structures are different.
     * @param o The other structure.
     * @returns True if the structures are different.
     * @returns False if the structures are identical.
     */
    constexpr bool operator!=(const VolCtrConfigReg& o) const noexcept { return !(*this == o); }

private:
    static constexpr uint8_t MuteLMask = 0x01;    //!< Bit-wise mask for Mute L.
    static constexpr uint8_t MuteRMask = 0x02;    //!< Bit-wise mask for Mute R.
    static constexpr uint8_t FadeMask  = 0x80;    //!< Bit-wise mask for Fade.

    static constexpr uint8_t MuteLShift = 0;    //!< Bit-wise shift for Mute L.
    static constexpr uint8_t MuteRShift = 1;    //!< Bit-wise shift for Mute R.
    static constexpr uint8_t FadeShift  = 7;    //!< Bit-wise shift for Fade.
};
}    // namespace Nilai::Interfaces::TAS5760

//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_TAS5760_VOL_CTRL_CONFIG_REG_H
