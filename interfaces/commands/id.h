/**
 * @file    id.h
 * @author  Samuel Martel
 * @date    2023-11-28
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


#ifndef GUARD_NILAI_INTERFACES_COMMANDS_ID_H
#define GUARD_NILAI_INTERFACES_COMMANDS_ID_H

#if defined(NILAI_USE_COMMAND_INTERFACE)
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup Commands
 * @{
 */
namespace Nilai::Interfaces
{
namespace Details
{
template<typename T>
consteval bool ValidCommandIdType()
{
    static_assert(std::is_enum_v<T>,
                  "Command ID must either be a uint8_t or an enum!");
    static_assert(std::is_same_v<std::underlying_type_t<T>, uint8_t>,
                  "Underlying type of the enum must be a uint8_t!");
    return true;
}
} // namespace Details

template<typename T>
concept ValidCommandId =
    requires { T::id; } && std::same_as<std::remove_cvref_t<decltype(T::id)>, uint8_t>;

template<typename T>
concept CommandId = std::same_as<T, uint8_t> || Details::ValidCommandIdType<T>();
}

//!@}
//!@}
//!@}
#endif
#endif //ID_H
