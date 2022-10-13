/**
 * @file    primitives.h
 * @author  Samuel Martel
 * @date    2022-09-22
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

#ifndef GUARD_NILAI_INTERFACES_COMMANDS_PRIMITIVES_H
#define GUARD_NILAI_INTERFACES_COMMANDS_PRIMITIVES_H

#if defined(NILAI_USE_COMMAND_INTERFACE)

#    include <string>
#    include <type_traits>

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

namespace Nilai::Interface
{
/**
 * A primitive is any value that is arithmetical or a string.
 * @tparam T
 */
template<typename T>
concept CommandPrimitive = std::is_arithmetic_v<T> || std::same_as<T, std::string>;
}    // namespace Nilai::Interface

//@}
//@}
//@}
#endif
#endif    // GUARD_NILAI_INTERFACES_COMMANDS_PRIMITIVES_H
