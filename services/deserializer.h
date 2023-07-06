/**
 * @file    deserializer.h
 * @author  Samuel Martel
 * @date    2022-10-11
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

#ifndef GUARD_NILAI_SERVICES_DESERIALIZER_H
#define GUARD_NILAI_SERVICES_DESERIALIZER_H

#include "../defines/misc.h"

#include <string>
#include <type_traits>

/**
 * @addtogroup Nilai
 * @{
 */

namespace Nilai
{
namespace Internal
{
template<typename T>
concept Primitives = std::is_arithmetic_v<T> || std::same_as<T, std::string>;
}

template<Internal::Primitives T, uint8_t... Is>
T Deserialize(const std::vector<uint8_t>&                                   v,
              [[maybe_unused]] const std::integer_sequence<uint8_t, Is...>& is)
{
    return Pack<T>(v[Is]...);
}

template<typename T, size_t N = sizeof(T)>
T Deserialize(const std::vector<uint8_t>& v)
    requires Internal::Primitives<T> || std::constructible_from<T, const std::vector<uint8_t>&>
{
    if constexpr (std::is_same_v<T, float>)
    {
        static_assert(sizeof(uint32_t) == sizeof(float));
        return std::bit_cast<T>(Deserialize<uint32_t>(v));
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        static_assert(sizeof(uint64_t) == sizeof(double));
        return std::bit_cast<T>(Deserialize<uint64_t>(v));
    }
    else if constexpr (std::is_integral_v<T>)
    {
        T      t     = {};
        size_t bytes = std::min(N, v.size());
        for (size_t i = 0; i < bytes; i++)
        {
            t |= static_cast<T>(v[i]) << ((N * 8) - 8 - (i * 8));
        }
        return t;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        return {v.begin(), v.end()};
    }
    else
    {
        return T {v};
    }
}
}    // namespace Nilai

//!@}

#endif    // GUARD_NILAI_SERVICES_DESERIALIZER_H
