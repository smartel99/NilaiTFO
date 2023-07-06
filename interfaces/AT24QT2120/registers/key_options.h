/**
 * @file    key_options.h
 * @author  Samuel Martel
 * @date    2022-07-12
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

#ifndef GUARD_AT24QT2120_REGISTERS_KEY_OPTIONS_H
#define GUARD_AT24QT2120_REGISTERS_KEY_OPTIONS_H

#if defined(NILAI_USE_AT24QT2120)
#    include <cstdint>
namespace Nilai::Interfaces::AT24QT2120
{
/**
 * Contains the options for a single key.
 *
 * @note It is not possible to use channel 0 or channel 1 as outputs.
 */
struct KeyOptions
{
    /**
     * If set to 0, channel is used as a key. Otherwise, it is used as an output.
     */
    uint8_t Enable : 1;
    /**
     * When Enable is set to 1, this bit sets the output state of the channel.
     */
    uint8_t GPO : 1;
    /**
     * Selects the group in which the key belongs.
     */
    uint8_t AKS : 2;
    /**
     * If set to 1, marks the channel as a guard, meaning that it will not affect the
     * detection status, and the CHANGE line will not go low.
     */
    uint8_t Guard : 1;
    uint8_t : 3;


    constexpr KeyOptions() = default;
    constexpr explicit KeyOptions(uint8_t v)
    {
        Enable = (v & EnableMask) >> EnableShift;
        GPO    = (v & GpoMask) >> GpoShift;
        AKS    = (v & AksMask) >> AksShift;
        Guard  = (v & GuardMask) >> GuardShift;
    }

    constexpr explicit operator uint8_t() const
    {
        return (Enable << EnableShift) | (GPO << GpoShift) | (AKS << AksShift) |
               (Guard << GuardShift);
    }

    constexpr bool operator==(const KeyOptions& o) const noexcept
    {
        return Enable == o.Enable && GPO == o.GPO && AKS == o.AKS && Guard == o.Guard;
    }

private:
    static constexpr uint8_t EnableMask = 0x01;
    static constexpr uint8_t GpoMask    = 0x02;
    static constexpr uint8_t AksMask    = 0x0C;
    static constexpr uint8_t GuardMask  = 0x10;

    static constexpr uint8_t EnableShift = 0x00;
    static constexpr uint8_t GpoShift    = 0x01;
    static constexpr uint8_t AksShift    = 0x02;
    static constexpr uint8_t GuardShift  = 0x04;
};
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_KEY_OPTIONS_H
