/**
 * @file    SaiEvent.h
 * @author  Samuel Martel
 * @date    2022-05-03
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
#ifndef NILAI_EVENTS_SAIEVENT_H
#define NILAI_EVENTS_SAIEVENT_H

#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_SAI_EVENTS)
#    include "generic_event.h"

#    include "../internal_config.h"
#    include NILAI_HAL_HEADER

namespace Nilai::Events
{
struct SaiEvent : public Event
{
    SaiEvent() : Event(EventTypes::SAI_Generic, EventCategories::Sai) {}
    SaiEvent(SAI_HandleTypeDef* sai, EventTypes t) : Event(t, EventCategories::Sai), Sai(sai) {}

    SAI_HandleTypeDef* Sai = nullptr;
};
}    // namespace Nilai::Events
#endif

#endif    // NILAI_EVENTS_SAIEVENT_H
