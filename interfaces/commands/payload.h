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

#    include "../../services/deserializer.h"
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
template<class Class, typename = void>
struct CommandPayloadMemberType
{
    using type = void;
};

template<class Class>
struct CommandPayloadMemberType<Class, std::void_t<decltype(std::declval<Class>().payload)>>
{
    using type = decltype(Class::payload);
};

template<class Class, typename = void>
struct CommandPayloadMemberSize
{
    static constexpr size_t value = 0;
};


template<class Class>
struct CommandPayloadMemberSize<Class, std::void_t<decltype(sizeof(Class))>>
{
    static constexpr size_t value = sizeof(Class);
};

static_assert(CommandPayloadMemberSize<void>::value == 0);
static_assert(CommandPayloadMemberSize<int>::value == sizeof(int));


template<typename T>
consteval bool CommandHasPayload()
{
    using payload_type = typename CommandPayloadMemberType<T>::type;
    return requires(T t) {
               typename T::payload_type;
               t.payload;
               t.payload_size;
           } && std::same_as<payload_type, typename T::payload_type> &&
           !std::same_as<payload_type, void> &&
           std::same_as<decltype(T::payload_size), const size_t>;
}

template<typename T>
concept SerializableCommandPayload =
  std::same_as<T, void> || std::same_as<T, std::vector<uint8_t>> ||
  std::convertible_to<T, std::vector<uint8_t>> || requires(T t) { Nilai::Serialize(t); };

template<typename T>
concept DeserializableCommandPayload =
  std::same_as<T, void> || std::constructible_from<T, std::vector<uint8_t>> ||
  requires { Nilai::Deserialize<T>({}); };

/**
 * A command payload can be void (no payload to be sent), or any type that can be converted to a
 * vector of uint8_t.
 */
template<typename T>
concept CommandPayload = SerializableCommandPayload<T> && DeserializableCommandPayload<T>;

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
