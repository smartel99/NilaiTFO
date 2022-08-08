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
 * General status flags.
 *
 * A change in these bytes will cause the CHANGE line to go low.
 *
 * @note If the slider or wheel is enabled, the SDET bit will be set when it is in a detected state.
 * Also, the relevant Key Status bit (0..2) and the TDET bit will be set. These bits can be ignored
 * if the SDET bit is set as the slider/wheel takes priority over the keys.
 */
struct DetectionStatus
{
    //! This bit is set if any of the keys are in detect.
    uint8_t TDet : 1;
    //! This bit is set if any of the slider/wheel channels are in detect.
    uint8_t SDet : 1;
    uint8_t : 4;
    //! This bit is set if the time to acquire all key signals exceeds 16ms.
    uint8_t Overflow : 1;
    //! This bit is set during a calibration sequence.
    uint8_t Calibrate : 1;

    constexpr DetectionStatus() noexcept = default;
    constexpr explicit DetectionStatus(uint8_t v) noexcept
    {
        TDet      = (v & TDetMask) >> TDetShift;
        SDet      = (v & SDetMask) >> SDetShift;
        Overflow  = (v & OverflowMask) >> OverflowShift;
        Calibrate = (v & CalibrateMask) >> CalibrateShift;
    }

    [[nodiscard]] constexpr explicit operator uint8_t() const noexcept
    {
        return (TDet << TDetShift) | (SDet << SDetShift) | (Overflow << OverflowShift) |
               (Calibrate << CalibrateShift);
    }

    bool operator==(const DetectionStatus& rhs) const
    {
        return TDet == rhs.TDet && SDet == rhs.SDet && Overflow == rhs.Overflow &&
               Calibrate == rhs.Calibrate;
    }
    bool operator!=(const DetectionStatus& rhs) const { return !(rhs == *this); }

private:
    static constexpr uint8_t TDetMask      = 0x01;
    static constexpr uint8_t SDetMask      = 0x02;
    static constexpr uint8_t OverflowMask  = 0x40;
    static constexpr uint8_t CalibrateMask = 0x80;

    static constexpr uint8_t TDetShift      = 0;
    static constexpr uint8_t SDetShift      = 1;
    static constexpr uint8_t OverflowShift  = 6;
    static constexpr uint8_t CalibrateShift = 7;
};
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_DETECTION_STATUS_H
