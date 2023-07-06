/**
 * @file    detection_status.h
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

#ifndef GUARD_AT24QT2120_REGISTERS_DETECTION_STATUS_H
#define GUARD_AT24QT2120_REGISTERS_DETECTION_STATUS_H

#if defined(NILAI_USE_AT24QT2120)
#    include <cstdint>
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
 * @struct DetectionStatus
 * @brief General status flags.
 *
 * A change in these bytes will cause the CHANGE line to go low.
 *
 * @note If the slider or wheel is enabled, the @ref SDet bit will be set when it is in a detected
 * state.
 * Also, the relevant Key Status bit (0..2) and the @ref TDet bit will be set. These bits can be
 * ignored
 * if the @ref SDet bit is set as the slider/wheel takes priority over the keys.
 */
struct DetectionStatus
{
    /**
     * @brief This bit is set if any of the keys are in detect.
     */
    uint8_t TDet : 1;
    /**
     * @brief This bit is set if any of the slider/wheel channels are in detect.
     */
    uint8_t SDet : 1;
    /**
     * @brief Reserved bits.
     */
    uint8_t : 4;
    /**
     * @brief This bit is set if the time to acquire all key signals exceeds 16ms.
     */
    uint8_t Overflow : 1;
    /**
     * @brief This bit is set during a calibration sequence.
     */
    uint8_t Calibrate : 1;

    /**
     * @brief Default constructor.
     */
    constexpr DetectionStatus() noexcept = default;
    /**
     * @brief Constructs the detection status with the byte received from the sensor.
     * @param v The data received from the sensor.
     */
    constexpr explicit DetectionStatus(uint8_t v) noexcept
    {
        TDet      = (v & TDetMask) >> TDetShift;
        SDet      = (v & SDetMask) >> SDetShift;
        Overflow  = (v & OverflowMask) >> OverflowShift;
        Calibrate = (v & CalibrateMask) >> CalibrateShift;
    }

    /**
     * @brief Converts the structure into a byte.
     * @return The byte
     */
    [[nodiscard]] constexpr explicit operator uint8_t() const noexcept
    {
        return (TDet << TDetShift) | (SDet << SDetShift) | (Overflow << OverflowShift) |
               (Calibrate << CalibrateShift);
    }

    /**
     * @brief Check if two @ref DetectionStatus are identical.
     * @param rhs The other @ref DetectionStatus.
     * @returns True if every members of both structures are identical.
     * @returns False otherwise.
     */
    bool operator==(const DetectionStatus& rhs) const
    {
        return TDet == rhs.TDet && SDet == rhs.SDet && Overflow == rhs.Overflow &&
               Calibrate == rhs.Calibrate;
    }

    /**
     * @brief Check if two @ref DetectionStatus are different.
     * @param rhs The other @ref DetectionStatus.
     * @returns True if one or more members of both structures are not identical.
     * @returns False if both structures are identical.
     */
    bool operator!=(const DetectionStatus& rhs) const { return !(rhs == *this); }

private:
    //! Bit-wise mask for @ref TDet.
    static constexpr uint8_t TDetMask = 0x01;
    //! Bit-wise mask for @ref SDet.
    static constexpr uint8_t SDetMask = 0x02;
    //! Bit-wise mask for @ref Overflow.
    static constexpr uint8_t OverflowMask = 0x40;
    //! Bit-wise mask for @ref Calibrate.
    static constexpr uint8_t CalibrateMask = 0x80;

    //! Bit-wise shift for @ref TDet.
    static constexpr uint8_t TDetShift = 0;
    //! Bit-wise shift for @ref SDet.
    static constexpr uint8_t SDetShift = 1;
    //! Bit-wise shift for @ref Overflow.
    static constexpr uint8_t OverflowShift = 6;
    //! Bit-wise shift for @ref Calibrate.
    static constexpr uint8_t CalibrateShift = 7;
};

//!@}
//!@}
//!@}
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_DETECTION_STATUS_H
