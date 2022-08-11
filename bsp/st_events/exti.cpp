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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#if defined(NILAI_USE_EVENTS)
#    include "../../defines/events/events.h"
#    include "../../processes/application.h"
#    include <defines/macros.h>

using EventTypes = Nilai::Events::EventTypes;

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Events
 * @{
 */

/**
 * @addtogroup nilai_exti_events EXTI
 * @{
 */

static constexpr uint8_t    PinIdToNum(uint16_t pin) noexcept;
static constexpr EventTypes PinIdToEventType(uint16_t pin) noexcept;

/**
 * @brief Callback function invoked by the HAL's EXTI interrupt request handler.
 * @param pin The ID of the pin that triggered the interrupt.
 */
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    uint8_t                 pinNum = PinIdToNum(pin);
    Nilai::Events::ExtEvent e(false, pinNum, PinIdToEventType(pin));
    Nilai::Application::Get()->DispatchEvent(&e);
}

/**
 * @brief Converts a bit-coded pin into its number.
 *
 * Example: 0b10000000 becomes 8
 *
 * @param pin The pin ID.
 * @return The pin number.
 */
constexpr uint8_t PinIdToNum(uint16_t pin) noexcept
{
    // A valid pin ID only has 1 bit set.
    NILAI_ASSERT(std::popcount(pin) == 1, "Invalid pin ID %#04x", pin);
    return std::countr_zero(pin) + 1;
}

/**
 * @brief Converts a bit-coded pin into its corresponding EXTI event.
 * @param pin The pin ID.
 * @return The corresponding EXTI event.
 */
constexpr EventTypes PinIdToEventType(uint16_t pin) noexcept
{
    switch (pin)
    {
        case 0x0001: return EventTypes::Exti0;
        case 0x0002: return EventTypes::Exti1;
        case 0x0004: return EventTypes::Exti2;
        case 0x0008: return EventTypes::Exti3;
        case 0x0010: return EventTypes::Exti4;
        case 0x0020: return EventTypes::Exti5;
        case 0x0040: return EventTypes::Exti6;
        case 0x0080: return EventTypes::Exti7;
        case 0x0100: return EventTypes::Exti8;
        case 0x0200: return EventTypes::Exti9;
        case 0x0400: return EventTypes::Exti10;
        case 0x0800: return EventTypes::Exti11;
        case 0x1000: return EventTypes::Exti12;
        case 0x2000: return EventTypes::Exti13;
        case 0x4000: return EventTypes::Exti14;
        case 0x8000: return EventTypes::Exti15;
        default: NILAI_ASSERT(false, "Invalid pin ID %#04x", pin); return EventTypes::Exti_Generic;
    }
}

//!@}
//!@}
//!@}
#endif
