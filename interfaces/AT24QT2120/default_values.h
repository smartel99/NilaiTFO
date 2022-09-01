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
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup AT24QT2120
 * @{
 */

/**
 * @struct KeyValues
 * @brief Contains the options for a single key.
 */
struct KeyValues
{
    /**
     * @brief Detection threshold of the key.
     *
     * Defaults to 10 counts.
     */
    uint8_t DetectThreshold = 10;

    /**
     * @brief Key-specific options.
     */
    KeyOptions Configuration = {};

    /**
     * @brief Pulse and Scale of the key.
     *
     * @note Refer to the <a
     * href=https://ww1.microchip.com/downloads/en/DeviceDoc/doc9634.pdf>datasheet</a> for more
     * information.
     */
    PulseScale PulseAndScale = {};

    constexpr KeyValues() noexcept = default;
    constexpr bool operator==(const KeyValues& o) const noexcept
    {
        return DetectThreshold == o.DetectThreshold && Configuration == o.Configuration &&
               PulseAndScale == o.PulseAndScale;
    }
};

/**
 * @struct RegisterMap
 * @brief List of the touch sensor's registers.
 *
 * These includes the default values for each registers.
 */
struct RegisterMap
{
public:
    uint8_t       Calibrate               = 0;
    uint8_t       Reset                   = 0;
    size_t        SamplingInterval        = 1;        //!< Defaults to 16 ms interval.
    uint8_t       TowardTouchDrift        = 20;       //!< Defaults to 3.2s/reference level.
    uint8_t       AwayFromTouchDrift      = 5;        //!< Defaults to 0.8s/reference level.
    uint8_t       DetectionIntegrator     = 4;        //!< Defaults to 4 consecutive measurements.
    size_t        TouchRecalibrationDelay = 40800;    //!< Defaults to 40.8 seconds.
    size_t        DriftHoldTime           = 4000;     //!< Defaults to 4 seconds.
    SliderOptions SliderOption            = {};
    uint8_t       ChargeTime              = 0;
    std::array<KeyValues, 12> Keys        = {};

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
//!@}
//!@}
//!@}
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_BUILDER_DEFAULT_VALUES_H
