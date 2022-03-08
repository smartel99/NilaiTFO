/**
 * @file    Events.cpp
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
#include "Events.h"

#if defined(NILAI_USE_EVENTS)
namespace cep::Events
{
static constexpr size_t PinIdToNum(uint16_t pin);

EventTypes PinToEvent(const Pin& p)
{
    constexpr EventTypes events[] = {
      EventTypes::Exti0,
      EventTypes::Exti1,
      EventTypes::Exti2,
      EventTypes::Exti3,
      EventTypes::Exti4,
      EventTypes::Exti5,
      EventTypes::Exti6,
      EventTypes::Exti7,
      EventTypes::Exti8,
      EventTypes::Exti9,
      EventTypes::Exti10,
      EventTypes::Exti11,
      EventTypes::Exti12,
      EventTypes::Exti13,
      EventTypes::Exti14,
      EventTypes::Exti15,
    };

    return events[PinIdToNum(p.pin)];
}

constexpr size_t PinIdToNum(uint16_t pin)
{
    uint8_t pos = 0;

    // We assume that the pin is never going to be 0.
    while (pin != 1)
    {
        ++pos;
        pin >>= 1;
    }

    return pos;
}
}    // namespace cep::Events
#endif