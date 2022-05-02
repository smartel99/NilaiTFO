/**
 * @file    Exti.cpp
 * @author  Samuel Martel
 * @date    2022-03-07
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
#if defined(NILAI_USE_EVENTS)
#    include "../../defines/Events/Events.h"
#    include "../../processes/application.hpp"

static uint8_t PinIdToNum(uint16_t pin);

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    uint8_t               pinNum = PinIdToNum(pin);
    cep::Events::ExtEvent e(false, pinNum, cep::Events::EventTypes(pinNum - 1));
    // Pin number goes from 1 to 16,
    cep::Application::Get()->DispatchEvent(&e);
}

/**
 * @brief Converts a bit-coded pin into its number.
 *
 * Example: 0b10000000 becomes 8
 *
 * @param pin The pin ID.
 * @return The pin number.
 */
uint8_t PinIdToNum(uint16_t pin)
{
    uint8_t pos = 1;

    // We assume that the pin is never going to be 0.
    while (pin != 1)
    {
        ++pos;
        pin >>= 1;
    }

    return pos;
}
#endif
