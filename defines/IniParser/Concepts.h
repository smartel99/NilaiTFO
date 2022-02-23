/**
 * @file    Concepts.h
 * @author  Samuel Martel
 * @date    2022-02-23
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef DERISKING_SD_DAC_I2S_CONCEPTS_H
#define DERISKING_SD_DAC_I2S_CONCEPTS_H

#if defined(NILAI_USE_INI_PARSER)

#    if !defined(__cpp_concepts)
#        error The IniParser requires concepts (C++20)!
#    endif

#    include <string>
#    include <type_traits>

namespace cep
{
template<typename T>
concept IsIniType = requires(T t)
{
    std::is_arithmetic<T>() || std::is_same<T, std::string>();
};

template<typename T>
concept IsInteger = requires(T t)
{
    std::is_integral<T>() && std::is_signed<T>();
};

template<typename T>
concept IsUnsignedInteger = requires(T t)
{
    std::is_unsigned<T>();
};

template<typename T>
concept IsFloatingPoint = requires(T t)
{
    std::is_floating_point<T>();
};

template<typename T>
concept IsBool = requires(T t)
{
    std::is_same<T, bool>();
};

}    // namespace cep
#endif

#endif    // DERISKING_SD_DAC_I2S_CONCEPTS_H
