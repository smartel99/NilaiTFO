/**
 * @file    default_values.h
 * @author  Samuel Martel
 * @date    2022-07-13
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

#ifndef GUARD_AT24QT2120_BUILDER_DEFAULT_VALUES_H
#define GUARD_AT24QT2120_BUILDER_DEFAULT_VALUES_H

#if defined(NILAI_USE_AT24QT2120)
#    include "registers/registers.h"

#    include <array>

namespace Nilai::Interfaces::AT24QT2120
{
struct KeyValues
{
    uint8_t    DetectThreshold = 10;    //!< Defaults to 10 count.
    KeyOptions Configuration   = {};
    PulseScale PulseAndScale   = {};

    constexpr KeyValues() noexcept = default;
    constexpr bool operator==(const KeyValues& o) const noexcept
    {
        return DetectThreshold == o.DetectThreshold && Configuration == o.Configuration &&
               PulseAndScale == o.PulseAndScale;
    }
};

struct RegisterMap
{
public:
    uint8_t       Calibrate               = 0;
    uint8_t       Reset                   = 0;
    size_t        SamplingInterval        = 1;      //!< Defaults to 16 ms interval.
    uint8_t       TowardTouchDrift        = 20;     //!< Defaults to 3.2s/reference level.
    uint8_t       AwayFromTouchDrift      = 5;      //!< Defaults to 0.8s/reference level.
    uint8_t       DetectionIntegrator     = 4;      //!< Defaults to 4 consecutive measurements.
    uint8_t       TouchRecalibrationDelay = 255;    //!< Defaults to 40.8 seconds.
    uint8_t       DriftHoldTime           = 25;     //!< Defaults to 4 seconds.
    SliderOptions SliderOption            = {};
    uint8_t       ChargeTime              = 0;
    std::array<KeyValues, 11> Keys        = {};

    constexpr RegisterMap() noexcept = default;
    [[nodiscard]] constexpr bool IsDefault() const noexcept { return *this == RegisterMap {}; }
    constexpr bool               operator==(const RegisterMap& rhs) const noexcept
    {
        return Calibrate == rhs.Calibrate && Reset == rhs.Reset &&
               SamplingInterval == rhs.SamplingInterval &&
               TowardTouchDrift == rhs.TowardTouchDrift &&
               AwayFromTouchDrift == rhs.AwayFromTouchDrift &&
               DetectionIntegrator == rhs.DetectionIntegrator &&
               TouchRecalibrationDelay == rhs.TouchRecalibrationDelay &&
               DriftHoldTime == rhs.DriftHoldTime && SliderOption == rhs.SliderOption &&
               ChargeTime == rhs.ChargeTime && Keys == rhs.Keys;
    }
    bool operator!=(const RegisterMap& rhs) const { return !(rhs == *this); }
};
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_BUILDER_DEFAULT_VALUES_H
