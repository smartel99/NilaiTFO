/**
 * @file    SpiEvent.h
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
#ifndef NILAI_EVENTS_SPIEVENT_H
#define NILAI_EVENTS_SPIEVENT_H

#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_SPI_EVENTS)
#    include "GenericEvent.h"

#    include "../internalConfig.h"
#    include NILAI_HAL_HEADER

namespace cep::Events
{
struct SpiEvent : public Event
{
    SpiEvent() : Event(EventTypes::SPI_Generic, EventCategories::Spi) {}
    SpiEvent(SPI_HandleTypeDef* spi, EventTypes t) : Event(t, EventCategories::Spi), Spi(spi) {}

    SPI_HandleTypeDef* Spi = nullptr;
};
}    // namespace cep::Events
#endif

#endif    // NILAI_EVENTS_SPIEVENT_H
