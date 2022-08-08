/**
 * @file    pulse_scale.h
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

#ifndef GUARD_AT24QT2120_REGISTERS_PULSE_SCALE_H
#define GUARD_AT24QT2120_REGISTERS_PULSE_SCALE_H

#if defined(NILAI_USE_AT24QT2120)
#    include <cstdint>
namespace Nilai::Interfaces::AT24QT2120
{
/**
 * Contains the options for a single key.
 *
 * @note It is not possible to use channel 0 or channel 1 as outputs.
 */
struct PulseScale
{
    uint8_t Scale : 4;
    uint8_t Pulse : 4;

    constexpr PulseScale() = default;
    constexpr explicit PulseScale(uint8_t v)
    {
        Scale = (v & ScaleMask) >> ScaleShift;
        Pulse = (v & PulseMask) >> PulseShift;
    }

    constexpr explicit operator uint8_t() const
    {
        return (Scale << ScaleShift) | (Pulse << PulseShift);
    }

    constexpr bool operator==(const PulseScale& o) const noexcept
    {
        return Scale == o.Scale && Pulse == o.Pulse;
    }

private:
    static constexpr uint8_t ScaleMask = 0x0F;
    static constexpr uint8_t PulseMask = 0xF0;

    static constexpr uint8_t ScaleShift = 0;
    static constexpr uint8_t PulseShift = 4;
};
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_PULSE_SCALE_H
