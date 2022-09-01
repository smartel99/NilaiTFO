/**
 * @file    sensor_status.h
 * @author  Samuel Martel
 * @date    2022-07-19
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

#ifndef GUARD_AT24QT2120_REGISTERS_SENSOR_STATUS_H
#define GUARD_AT24QT2120_REGISTERS_SENSOR_STATUS_H

#if defined(NILAI_USE_AT24QT2120)

#    include "detection_status.h"
#    include "key_status.h"

#    include "../../../defines/misc.h"

#    include <vector>

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
 * @struct SensorStatus
 * @brief Collection of the various statuses contained in the sensor.
 */
struct SensorStatus
{
    //! Detection status of the sensor. Also includes more generic status information.
    DetectionStatus ChipStatus = {};

    //! Detection status of each keys.
    KeyStatus KeyStatuses = {};

    /**
     * Reports the slider/wheel position. Only valid when the @c SDet bit is set in @ref
     * ChipStatus.
     */
    uint8_t SliderPosition = {};

    constexpr SensorStatus() noexcept = default;
    /**
     * @brief Initializes the sensor's statuses using 4 bytes of data received through I2C.
     * @param data 4 bytes of data received from the sensor through I2C.
     */
    explicit SensorStatus(const std::vector<uint8_t>& data) noexcept
    : ChipStatus(DetectionStatus {data[0]}),
      KeyStatuses(KeyStatus {Nilai::Pack<uint16_t>(data[1], data[2])}),
      SliderPosition(data[3])
    {
        NILAI_ASSERT(data.size() == 4, "bad data");
    }

    bool operator==(const SensorStatus& rhs) const
    {
        return ChipStatus == rhs.ChipStatus && KeyStatuses == rhs.KeyStatuses &&
               SliderPosition == rhs.SliderPosition;
    }
    bool operator!=(const SensorStatus& rhs) const { return !(rhs == *this); }
};

//!@}
//!@}
//!@}
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_SENSOR_STATUS_H
