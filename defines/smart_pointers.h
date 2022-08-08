/**
 * @file    smart_pointers.h
 * @author  Samuel Martel
 * @date    2022-05-04
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
#ifndef NILAI_SMARTPOINTERS_H
#define NILAI_SMARTPOINTERS_H

#include <memory>

namespace Nilai
{
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ptr = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Ptr<T> CreatePtr(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}
}    // namespace Nilai

#endif    // NILAI_SMARTPOINTERS_H
