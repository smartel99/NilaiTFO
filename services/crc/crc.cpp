/**
 * @file    crc.cpp
 * @author  Samuel Martel
 * @date    2022-08-31
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

#include "crc.h"

#include "../../defines/internal_config.h"
#include NILAI_HAL_HEADER

namespace Nilai::Services
{

uint32_t Crc(const uint32_t* data, size_t len, uint32_t initial)
{
#if defined(HAL_CRC_MODULE_ENABLED)
    auto* crc = reinterpret_cast<CRC_TypeDef*>(CRC_BASE);
    crc->DR   = initial;
    for (size_t i = 0; i < len; i++)
    {
        crc->DR = data[i];
    }
    return crc->DR;
#else
    // TODO Implement crc
    (void)data;
    (void)len;
    (void)initial;
    return 0;
#endif
}
}    // namespace Nilai::Services
