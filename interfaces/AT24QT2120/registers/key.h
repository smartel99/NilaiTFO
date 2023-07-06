/**
 * @file    key.h
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

#ifndef GUARD_AT24QT2120_REGISTERS_KEY_H
#define GUARD_AT24QT2120_REGISTERS_KEY_H

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
 * @enum Keys
 * @brief List of the different keys of the sensor.
 */
enum class Keys
{
    Key0 = 0,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,
    Key10,
    Key11,

    //! Number of keys available. Should always be last.
    Count
};

//!@}
//!@}
//!@}
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_KEY_H
