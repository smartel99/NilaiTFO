/**
 * @file    serializer.h
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

#ifndef GUARD_NILAI_SERVICES_SERIALIZER_H
#define GUARD_NILAI_SERVICES_SERIALIZER_H


#include <algorithm>
#include <array>
#include <bit>
#include <string>
#include <type_traits>
#include <vector>
#include <utility>

/**
 * @addtogroup Nilai
 * @{
 */

namespace Nilai
{
/**
 * A serializable container is a container that can be iterated over, and who's values are also
 * serializable.
 *
 * For a container to be serializable, the following requirements needs to be met:
 * <ul>
 *  <li>Member Types: <ul>
 *      <li>@c value_type: The type of the value. Must meet <a
 * href=https://en.cppreference.com/w/cpp/types/is_arithmetic>@c std::is_arithmetic</a>.</li>
 *      <li>@c iterator: Iterator to @c value_type (e.g. @c value_type* ).</li>
 *      <li>@c const_iterator: Constant iterator @c value_type (e.g. @c const @c value_type* ).</li>
 *      <li>@c size_type: Unsigned integer type (usually @c std::size_t ).</li>
 *  </ul></li>
 *  <li>Member Functions: <ul>
 *
 *  </ul></li>
 * </ul>
 * @tparam T
 */
template<typename T>
concept SerializableContainer = requires(T t) {
                                    typename T::value_type;

                                    typename T::iterator;
                                    typename T::const_iterator;
                                    {
                                        t.begin()
                                        } -> std::same_as<typename T::iterator>;
                                    {
                                        std::as_const(t).begin()
                                        } -> std::same_as<typename T::const_iterator>;

                                    {
                                        t.end()
                                        } -> std::same_as<typename T::iterator>;
                                    {
                                        std::as_const(t).end()
                                        } -> std::same_as<typename T::const_iterator>;

                                    {
                                        t.size()
                                        } -> std::same_as<typename T::size_type>;
                                    {
                                        std::as_const(t).size()
                                        } -> std::same_as<typename T::size_type>;
                                } && std::is_arithmetic_v<typename T::value_type>;



/**
 * A type that can be serialized.
 * @tparam T
 */
template<typename T>
concept Serializable = std::is_arithmetic_v<T> || SerializableContainer<T> ||
                       std::convertible_to<T, std::vector<uint8_t>>;


template<Serializable T>
std::vector<uint8_t> Serialize(const T& t) noexcept
{
    std::vector<uint8_t> serialized;

    auto serializeBytes = [&serialized]<typename TT>(TT tt)
    {
        auto ttAsBytes = std::bit_cast<std::array<uint8_t, sizeof(TT)>>(tt);
        if constexpr (std::endian::native == std::endian::big)
        {
            for (auto byte = ttAsBytes.begin(); byte != ttAsBytes.end(); byte++)
            {
                serialized.push_back(*byte);
            }
        }
        else
        {
            for (auto byte = ttAsBytes.rbegin(); byte != ttAsBytes.rend(); byte++)
            {
                serialized.push_back(*byte);
            }
        }
    };

    if constexpr (SerializableContainer<T>)
    {
        constexpr size_t bytesPerObj = sizeof(typename T::value_type);
        serialized.reserve(t.size() * bytesPerObj);

        for (const auto& item : t)
        {
            serializeBytes(item);
        }
    }
    else if constexpr (std::convertible_to<T, std::vector<uint8_t>>)
    {
        return static_cast<std::vector<uint8_t>>(t);
    }
    else
    {
        serialized.reserve(sizeof(T));
        serializeBytes(t);
    }

    return serialized;
}


}    // namespace Nilai

//@}
#endif    // GUARD_NILAI_SERVICES_SERIALIZER_H
