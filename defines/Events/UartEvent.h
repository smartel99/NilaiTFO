/**
 * @file    UartEvent.h
 * @author  Samuel Martel
 * @date    2022-05-02
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
#ifndef NILAI_EVENTS_UARTEVENT_H
#define NILAI_EVENTS_UARTEVENT_H

#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_UART_EVENTS)
#    include "GenericEvent.h"

#    include "../../defines/internalConfig.h"
#    include NILAI_HAL_HEADER

namespace cep::Events
{
struct UartEvent : public Event
{
    UartEvent() : Event(EventTypes::UART_Generic, EventCategories::Uart) {}
    UartEvent(UART_HandleTypeDef* uart, EventTypes t) : Event(t, EventCategories::Uart), Uart(uart)
    {
    }

    UART_HandleTypeDef* Uart = nullptr;
};

struct UartRxEvent : public UartEvent
{
    UartRxEvent(UART_HandleTypeDef* uart, uint16_t size)
    : UartEvent(uart, EventTypes::UART_RxEvent), Size(size)
    {
    }

    uint16_t Size = 0;
};
}    // namespace cep::Events

#endif

#endif    // NILAI_EVENTS_UARTEVENT_H
