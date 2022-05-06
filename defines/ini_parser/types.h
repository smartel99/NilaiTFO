/**
 * @file    Types.h
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
#ifndef NILAI_TYPES_H
#define NILAI_TYPES_H

#if defined(NILAI_USE_INI_PARSER)

#    include "Concepts.h"
#    include <algorithm>
#    include <array>
#    include <string>

namespace Nilai
{
#    if defined(__cpp_concepts)
template<Nilai::IsIniType T>
#    else
template<typename T>
#    endif
inline static T StrToVal(const std::string& s)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return s;
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        static constexpr std::array trues = {"true", "on", "active"};
        // Convert the string to lowercase.
        std::string ls;
        std::transform(s.begin(), s.end(), ls.begin(), ::tolower);

        return std::find(trues.begin(), trues.end(), s) != trues.end();
    }
    else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
    {
        return static_cast<T>(std::stol(s));
    }
    else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>)
    {
        return static_cast<T>(std::stoul(s));
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        return static_cast<T>(std::stold(s));
    }
}

}    // namespace Nilai

#endif

#endif    // NILAI_TYPES_H
