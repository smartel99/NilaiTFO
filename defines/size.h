/**
 * @file    size.h
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

#ifndef GUARD_NILAI_DEFINES_SIZE_H
#define GUARD_NILAI_DEFINES_SIZE_H

#include <concepts>
#include <cstddef>

/**
 * @addtogroup Nilai
 * @{
 */

namespace Nilai
{
template<typename T>
consteval size_t Size()
{
    if constexpr (requires {
                      {
                          T::size
                          } -> std::same_as<size_t>;
                  })
    {
        return T::size;
    }
    else if constexpr (!std::same_as<T, void>)
    {
        return sizeof(T);
    }
    else
    {
        return 0;
    }
}
}    // namespace Nilai

//!@}
#endif    // GUARD_NILAI_DEFINES_SIZE_H
