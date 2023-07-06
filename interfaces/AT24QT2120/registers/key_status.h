/**
 * @file    key_status.h
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

#ifndef GUARD_AT24QT2120_REGISTERS_KEY_STATUS_H
#define GUARD_AT24QT2120_REGISTERS_KEY_STATUS_H

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
 * @struct KeyStatus
 * @brief Indicates which keys are being detected.
 *
 * A value of 1 indicates a touched key.
 * A value of 0 indicates an untouched key.
 *
 * Any change in these bytes will cause the CHANGE line to go LOW.
 */
struct KeyStatus
{
    uint16_t : 4;          //!< Reserved bits.
    uint16_t Key11 : 1;    //!< Status of key #11.
    uint16_t Key10 : 1;    //!< Status of key #10.
    uint16_t Key9 : 1;     //!< Status of key #9.
    uint16_t Key8 : 1;     //!< Status of key #8.
    uint16_t Key7 : 1;     //!< Status of key #7.
    uint16_t Key6 : 1;     //!< Status of key #6.
    uint16_t Key5 : 1;     //!< Status of key #5.
    uint16_t Key4 : 1;     //!< Status of key #4.
    uint16_t Key3 : 1;     //!< Status of key #3.
    uint16_t Key2 : 1;     //!< Status of key #2.
    uint16_t Key1 : 1;     //!< Status of key #1.
    uint16_t Key0 : 1;     //!< Status of key #0.

    /**
     * @brief Default constructor.
     */
    constexpr KeyStatus() noexcept = default;

    /**
     * @brief Constructs the structure from the data read off the touch sensor.
     * @param v The data read from the touch sensor.
     */
    constexpr explicit KeyStatus(uint16_t v) noexcept
    {
        Key0  = (v & Key0Mask) >> Key0Shift;
        Key1  = (v & Key1Mask) >> Key1Shift;
        Key2  = (v & Key2Mask) >> Key2Shift;
        Key3  = (v & Key3Mask) >> Key3Shift;
        Key4  = (v & Key4Mask) >> Key4Shift;
        Key5  = (v & Key5Mask) >> Key5Shift;
        Key6  = (v & Key6Mask) >> Key6Shift;
        Key7  = (v & Key7Mask) >> Key7Shift;
        Key8  = (v & Key8Mask) >> Key8Shift;
        Key9  = (v & Key9Mask) >> Key9Shift;
        Key10 = (v & Key10Mask) >> Key10Shift;
        Key11 = (v & Key11Mask) >> Key11Shift;
    }

    /**
     * @brief Converts the structure into a uint16_t.
     * @return The uint16_t representing the structure.
     */
    [[nodiscard]] explicit constexpr operator uint16_t() const noexcept
    {
        return (Key0 << Key0Shift) | (Key1 << Key1Shift) | (Key2 << Key2Shift) |
               (Key3 << Key3Shift) | (Key4 << Key4Shift) | (Key5 << Key5Shift) |
               (Key6 << Key6Shift) | (Key7 << Key7Shift) | (Key8 << Key8Shift) |
               (Key9 << Key9Shift) | (Key10 << Key10Shift) | (Key11 << Key11Shift);
    }

    /**
     * @brief Checks if two key status structures are identical.
     * @param o The other structure.
     * @returns True if the structures are identical.
     * @returns False if the structures are different.
     */
    constexpr bool operator==(const KeyStatus& o) const noexcept
    {
        return static_cast<uint16_t>(*this) == static_cast<uint16_t>(o);
    }

    /**
     * @brief Checks if two key status structures are different.
     * @param o The other structure.
     * @returns True if the structures are different.
     * @returns False if the structures are identical.
     */
    constexpr bool operator!=(const KeyStatus& o) const noexcept { return !(*this == o); }

private:
    static constexpr uint16_t Key0Mask  = 0x0001;    //!< Bit-wise mask for 1st key.
    static constexpr uint16_t Key1Mask  = 0x0002;    //!< Bit-wise mask for 2nd key.
    static constexpr uint16_t Key2Mask  = 0x0004;    //!< Bit-wise mask for 3rd key.
    static constexpr uint16_t Key3Mask  = 0x0008;    //!< Bit-wise mask for 4th key.
    static constexpr uint16_t Key4Mask  = 0x0010;    //!< Bit-wise mask for 5th key.
    static constexpr uint16_t Key5Mask  = 0x0020;    //!< Bit-wise mask for 6th key.
    static constexpr uint16_t Key6Mask  = 0x0040;    //!< Bit-wise mask for 7th key.
    static constexpr uint16_t Key7Mask  = 0x0080;    //!< Bit-wise mask for 8th key.
    static constexpr uint16_t Key8Mask  = 0x0100;    //!< Bit-wise mask for 9th key.
    static constexpr uint16_t Key9Mask  = 0x0200;    //!< Bit-wise mask for 10th key.
    static constexpr uint16_t Key10Mask = 0x0400;    //!< Bit-wise mask for 11th key.
    static constexpr uint16_t Key11Mask = 0x0800;    //!< Bit-wise mask for 12th key.

    static constexpr uint16_t Key0Shift  = 0;     //!< Bit-wise shift for 1st key.
    static constexpr uint16_t Key1Shift  = 1;     //!< Bit-wise shift for 2nd key.
    static constexpr uint16_t Key2Shift  = 2;     //!< Bit-wise shift for 3rd key.
    static constexpr uint16_t Key3Shift  = 3;     //!< Bit-wise shift for 4th key.
    static constexpr uint16_t Key4Shift  = 4;     //!< Bit-wise shift for 5th key.
    static constexpr uint16_t Key5Shift  = 5;     //!< Bit-wise shift for 6th key.
    static constexpr uint16_t Key6Shift  = 6;     //!< Bit-wise shift for 7th key.
    static constexpr uint16_t Key7Shift  = 7;     //!< Bit-wise shift for 8th key.
    static constexpr uint16_t Key8Shift  = 8;     //!< Bit-wise shift for 9th key.
    static constexpr uint16_t Key9Shift  = 9;     //!< Bit-wise shift for 10th key.
    static constexpr uint16_t Key10Shift = 10;    //!< Bit-wise shift for 11th key.
    static constexpr uint16_t Key11Shift = 11;    //!< Bit-wise shift for 12th key.
};

//!@}
//!@}
//!@}
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_KEY_STATUS_H
