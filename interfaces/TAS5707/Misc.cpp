/**
 * @file    Misc.cpp
 * @author  Samuel Martel
 * @date    2022-02-08
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
#include "misc.h"

#if defined(NILAI_USE_TAS5707)

namespace Nilai::Tas5707
{
std::array<uint8_t, 20> BiquadFilter::Serialize() const
{
    return {
      (uint8_t)(b0 >> 24), (uint8_t)(b0 >> 16), (uint8_t)(b0 >> 8), (uint8_t)(b0),
      (uint8_t)(b1 >> 24), (uint8_t)(b1 >> 16), (uint8_t)(b1 >> 8), (uint8_t)(b1),
      (uint8_t)(b2 >> 24), (uint8_t)(b2 >> 16), (uint8_t)(b2 >> 8), (uint8_t)(b2),
      (uint8_t)(a1 >> 24), (uint8_t)(a1 >> 16), (uint8_t)(a1 >> 8), (uint8_t)(a1),
      (uint8_t)(a2 >> 24), (uint8_t)(a2 >> 16), (uint8_t)(a2 >> 8), (uint8_t)(a2),
    };
}
bool BiquadFilter::operator==(const BiquadFilter& o) const
{
    return b0 == o.b0 && b1 == o.b1 && b2 == o.b2 && a1 == o.a1 && a2 == o.a2;
}
std::array<uint8_t, 8> DynRangeCtrlCoeffs::Serialize() const
{
    return {
      (uint8_t)(a >> 24),
      (uint8_t)(a >> 16),
      (uint8_t)(a >> 8),
      (uint8_t)(a),
      (uint8_t)(oa >> 24),
      (uint8_t)(oa >> 16),
      (uint8_t)(oa >> 8),
      (uint8_t)(oa),
    };
}
}    // namespace Nilai::Tas5707

#endif