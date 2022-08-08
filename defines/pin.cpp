/**
 * @file    pin.cpp
 * @author  Samuel Martel
 * @date    2022-02-09
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
#include "pin.h"

#include "internal_config.h"

#include "macros.h"

namespace Nilai
{

void Pin::Set(bool state) const
{
    NILAI_ASSERT(port != nullptr, "Port is null!");
    /**
     * The BSSR register is split into two 16-bits part:
     *  - The lower 16 bits are used to set a pin
     *  - The upper 16 bits are used to clear a pin
     * This means that to set pin 1, a 1 should be written at bit 0. To clear the same pin, a 1
     * should instead be written to bit 16.
     */
    if (state)
    {
        port->NILAI_GPIO_BSRR_REG = pin;
    }
    else
    {
        port->NILAI_GPIO_BSRR_REG = (uint32_t)pin << 16U;
    }
}
bool Pin::Get() const
{
    NILAI_ASSERT(port != nullptr, "Port is null!");
    return (port->NILAI_GPIO_IDR_REG & pin) != 0;
}

void Pin::Toggle() const
{
    Set(!Get());
}

}    // namespace Nilai
