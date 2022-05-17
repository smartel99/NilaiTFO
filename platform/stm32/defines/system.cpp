/**
 * @file    system.cpp
 * @author  Samuel Martel
 * @date    2022-05-17
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
#include "../../../defines/system.h"
#include "../../../defines/internal_config.h"
#include NILAI_HAL_HEADER

namespace Nilai::System
{
void Reset()
{
    HAL_NVIC_SystemReset();
}

bool IsDebuggerConnected()
{
    return (((*(volatile uint32_t*)0xE000EDF0) & (1 << 0)) != 0u);
}
}    // namespace Nilai::System
