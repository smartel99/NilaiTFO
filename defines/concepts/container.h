/**
 * @file    container.h
 * @author  Samuel Martel
 * @date    2022-09-29
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

#ifndef GUARD_NILAI_DEFINES_CONCEPTS_CONTAINER_H
#define GUARD_NILAI_DEFINES_CONCEPTS_CONTAINER_H

#include <concepts>
#include <utility>

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Concepts
 * @{
 */

namespace Nilai::Containers
{
template<typename T>
concept HasValueType = requires { typename T::value_type; };

template<typename T>
concept HasIterators = requires {
                           typename T::iterator;
                           typename T::const_iterator;
                           typename T::reverse_iterator;
                           typename T::const_reverse_iterator;
                       };

template<typename T>
concept Iterable =
  HasIterators<T> && requires(T t) {
                         {
                             t.begin()
                             } -> std::same_as<typename T::iterator>;
                         {
                             std::as_const(t).begin()
                             } -> std::same_as<typename T::const_iterator>;
                         {
                             t.cbegin()
                         }->std::same_as<typename T::const_iterator>;
                         {
                             t.rbegin()
                         }->std::same_as<typename T::reverse_iterator>;
                         {
                             std::as_const(t).rbegin()
                         }->std::same_as<typename T::const_reverse_iterator>;
                         {
                             t.crbegin()
                         }->std::same_as<typename T::const_reverse_iterator>;

                         {
                             t.end()
                         }->std::same_as<typename T::iterator>;
                         {
                             std::as_const(t).end()
                         }->std::same_as<typename T::const_iterator>;
                         {
                             t.cend()
                         }->std::same_as<typename T::const_iterator>;
                         {
                             t.rend()
                         }->std::same_as<typename T::reverse_iterator>;
                         {
                             std::as_const(t).rend()
                         }->std::same_as<typename T::const_reverse_iterator>;
                         {
                             t.crend()
                         }->std::same_as<typename T::const_reverse_iterator>;
                     };

}    // namespace Nilai::Containers
//!@}
//!@}

#endif    // GUARD_NILAI_DEFINES_CONCEPTS_CONTAINER_H
