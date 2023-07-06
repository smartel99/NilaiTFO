/**
 * @file    constexpr_crc.h
 * @author  Samuel Martel
 * @date    2022-08-24
 * @brief Based on https://github.com/gcc-mirror/gcc/blob/master/libiberty/crc32.c
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

#ifndef GUARD_NILAI_SERVICES_CONSTEXPR_CRC_H
#define GUARD_NILAI_SERVICES_CONSTEXPR_CRC_H

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Services
 * @{
 */

/**
 * @addtogroup nilai_services_crc CRC
 * @{
 */

#include <array>
#include <bit>
#include <cstdint>

namespace Nilai::Services
{

constexpr uint32_t ConstexprCrc(const uint32_t* buf, std::size_t len, uint32_t initial = 0xFFFFFFFF)
{
    constexpr auto table = []()
    {
        uint32_t                  polynomial = 0x04C11DB7;
        std::array<uint32_t, 256> table      = {};
        for (uint32_t i = 0; i < table.size(); i++)
        {
            uint32_t c = i << 24;
            for (uint32_t j = 8; j > 0; --j)
            {
                c = ((c & 0x80000000) != 0) ? (c << 1) ^ polynomial : (c << 1);
            }
            table[i] = c;
        }

        return table;
    }();

    uint32_t c = initial;
    for (std::size_t i = 0; i < len; i++)
    {
        for (std::size_t j = 0; j < sizeof(uint32_t); j++)
        {
            uint8_t d;
            if constexpr (std::endian::native == std::endian::little)
            {
                constexpr std::size_t shifts[4] = {24, 16, 8, 0};
                d = static_cast<uint8_t>((buf[i] >> shifts[j]) & 0xFF);
            }
            else
            {
                d = static_cast<uint8_t>(buf[i] >> (j * 8));
            }

            c = (c << 8) ^ table[((c >> 24) ^ d) & 0xFF];
        }
    }

    return c;
}

// Value calculated from https://crccalc.com/?crc=12345678&method=crc32&datatype=hex&outtype=0
static_assert(
  []()
  {
      constexpr uint32_t x = 0x12345678;
      return ConstexprCrc(&x, 1);
  }() == 0xDF8A8A2B);

}    // namespace Nilai::Services
//!@}
//!@}
//!@}

#endif    // GUARD_NILAI_SERVICES_CONSTEXPR_CRC_H
