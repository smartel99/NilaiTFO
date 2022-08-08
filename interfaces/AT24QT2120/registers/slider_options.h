/**
 * @file    slider_options.h
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

#ifndef GUARD_AT24QT2120_REGISTERS_SLIDER_OPTIONS_H
#define GUARD_AT24QT2120_REGISTERS_SLIDER_OPTIONS_H

#if defined(NILAI_USE_AT24QT2120)
#    include <cstdint>
namespace Nilai::Interfaces::AT24QT2120
{
/**
 * Options to enable/disable sliders and wheels.
 */
struct SliderOptions
{
    uint8_t : 6;
    //! Setting this bit allows a wheel to be configured. If not set, and Enable is set, it
    //! defaults to a slider.
    uint8_t Wheel : 1;
    //! Setting this bit enables a Slider or a Wheel to be configured. Only the first three
    //! channels (0, 1 and 2) can be used.
    uint8_t Enable : 1;

    constexpr SliderOptions() noexcept = default;
    constexpr explicit SliderOptions(uint8_t v) noexcept
    {
        Wheel  = (v & WheelMask) >> WheelShift;
        Enable = (v & EnableMask) >> EnableShift;
    }
    constexpr SliderOptions(bool isEnabled, bool isWheel) noexcept
    : Wheel(isWheel), Enable(isEnabled)
    {
    }

    constexpr explicit operator uint8_t() const noexcept
    {
        return (Wheel << WheelShift) | (Enable << EnableShift);
    }

    constexpr bool operator==(const SliderOptions& o) const noexcept
    {
        return Wheel == o.Wheel && Enable == o.Enable;
    }

private:
    static constexpr uint8_t EnableMask = 0x80;
    static constexpr uint8_t WheelMask  = 0x40;

    static constexpr uint8_t EnableShift = 8;
    static constexpr uint8_t WheelShift  = 7;
};
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_SLIDER_OPTIONS_H
