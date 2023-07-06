/**
 * @file    function.h
 * @author  Samuel Martel
 * @date    2022-08-16
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

#ifndef GUARD_NILAI_FUNCTION_H
#define GUARD_NILAI_FUNCTION_H

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup type_traits Type Traits
 * @{
 */

#include <tuple>

namespace Nilai
{
/**
 * @struct Function
 * @brief Metaprogramming interface that gets the return type of a function.
 * @tparam Result The return value of the function.
 */
template<typename Result>
struct Function
{
    using returnType = Result;
    using arguments  = std::tuple<void>;
    struct Argument
    {
        using type = void;
    };
};

/**
 * @struct Function
 * @brief Metaprogramming interface that gets the return type of a function, as well as a list of
 * the parameters.
 * @tparam Result The return type of the function.
 * @tparam Args The parameters of the function.
 */
template<typename Result, typename... Args>
struct Function<Result(Args...)>
{
    using returnType = Result;
    using arguments  = std::tuple<Args...>;

    template<std::size_t i>
    struct Argument
    {
        using type = typename std::tuple_element_t<i, arguments>;
    };
};

/**
 * @struct Function
 * @brief Metaprogramming interface that gets the return type of a non-static member function, as
 * well as a list of the parameters.
 * @tparam Class The class in which the function
 * @tparam Result
 * @tparam Args
 */
template<typename Class, typename Result, typename... Args>
struct Function<Result (Class::*)(Args...)>
{
    using returnType = Result;
    using arguments  = std::tuple<Args...>;

    template<std::size_t i>
    struct Argument
    {
        using type = typename std::tuple_element_t<i, arguments>;
    };
};


}    // namespace Nilai
//!@}
//!@}

#endif    // GUARD_NILAI_FUNCTION_H
