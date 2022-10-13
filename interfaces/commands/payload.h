/**
 * @file    payload.h
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

#ifndef GUARD_NILAI_INTERFACES_COMMANDS_PAYLOAD_H
#define GUARD_NILAI_INTERFACES_COMMANDS_PAYLOAD_H

#if defined(NILAI_USE_COMMAND_INTERFACE)

#    include <string>
#    include <type_traits>
#    include <vector>

#    include "../../services/serializer.h"
#    include "primitives.h"

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
template<typename T>
consteval bool CommandHasPayload()
    requires requires(T t) {
                 typename T::payload_type;
                 t.payload;

                 {
                     T::payload_size
                     } -> std::same_as<size_t>;
             } && std::same_as<decltype(T::payload), typename T::payload_type>
{
    return true;
}

template<typename T>
consteval bool CommandHasPayload()
{
    return false;
}

/**
 * A command payload can be void (no payload to be sent), or any type that can be converted to a
 * vector of uint8_t.
 */
template<typename T>
concept CommandPayload =
  std::same_as<T, void> || std::same_as<T, std::vector<uint8_t>> ||
  std::convertible_to<T, std::vector<uint8_t>> || requires(T t) { Nilai::Serialize(t); };

template<typename T>
concept CommandConstructibleFromArray =
  requires { std::constructible_from<T, std::array<uint8_t, T::payload_size>>; };

template<typename T>
concept CommandPayloadConstructibleFromArray =
  requires {
      std::constructible_from<typename T::payload_type, std::array<uint8_t, T::payload_size>>;
  };

template<typename T>
concept ValidCommandPayload = !
CommandHasPayload<T>() ||
  (CommandConstructibleFromArray<T> || CommandPayloadConstructibleFromArray<T>);

}    // namespace Nilai::Interfaces

//@}
//@}
//@}
#endif
#endif    // GUARD_NILAI_INTERFACES_COMMANDS_PAYLOAD_H
