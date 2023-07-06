/**
 * @file    crc.h
 * @author  Samuel Martel
 * @date    2022-08-24
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

#ifndef GUARD_NILAI_SERVICES_CRC_H
#define GUARD_NILAI_SERVICES_CRC_H

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
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Nilai::Services
{
uint32_t Crc(const uint32_t* data, size_t len, uint32_t initial = 0xFFFFFFFF);

template<size_t N>
static uint32_t Crc(const std::array<uint32_t, N>& array, uint32_t initial = 0xFFFFFFFF)
{
    return Crc(array.data(), array.size(), initial);
}

[[maybe_unused]] static uint32_t Crc(const std::vector<uint32_t>& array,
                                     uint32_t                     initial = 0xFFFFFFFF)
{
    return Crc(array.data(), array.size(), initial);
}
}    // namespace Nilai::Services
//!@}
//!@}
//!@}

#endif    // GUARD_NILAI_SERVICES_CRC_H
