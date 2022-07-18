/**
 * @file    firmware_version.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */

#ifndef GUARD_AT24QT2120_REGISTERS_FIRMWARE_VERSION_H
#define GUARD_AT24QT2120_REGISTERS_FIRMWARE_VERSION_H

#if defined(NILAI_USE_AT24QT2120)
namespace Nilai::Interfaces::AT24QT2120
{
/**
 * Firmware version of the AT24QT2120, in the Major.Minor format.
 */
struct FirmwareVersion
{
    //! Holds the minor firmware version.
    uint8_t Minor : 4;
    //! Holds the major firmware version.
    uint8_t Major : 4;

    constexpr FirmwareVersion() noexcept = default;
    constexpr explicit FirmwareVersion(uint8_t v) noexcept
    {
        Minor = (v & MinorMask) >> MinorShift;
        Major = (v & MajorMask) >> MajorShift;
    }

    constexpr explicit operator uint8_t() const
    {
        return (Major << MajorShift) | (Minor << MinorShift);
    }

    constexpr bool operator==(const FirmwareVersion& o) const noexcept
    {
        return Major == o.Major && Minor == o.Minor;
    }

private:
    static constexpr uint8_t MinorMask = 0x0F;
    static constexpr uint8_t MajorMask = 0xF0;

    static constexpr uint8_t MinorShift = 0;
    static constexpr uint8_t MajorShift = 4;
};
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_FIRMWARE_VERSION_H
