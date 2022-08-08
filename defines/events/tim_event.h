/**
 * @file    tim_event.h
 * @author  Samuel Martel
 * @date    2022-03-15
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
#ifndef NILAI_EVENTS_TIMEVENT_H
#define NILAI_EVENTS_TIMEVENT_H

#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_TIMER_EVENTS)
#    if defined(NILAI_TEST)
#    else
#        include "generic_event.h"

#        include "../internal_config.h"
#        include NILAI_HAL_HEADER

namespace Nilai::Events
{
struct TimEvent : public Event
{
    TimEvent() : Event(EventTypes::Tim_Generic, EventCategories::Timer) {}
    TimEvent(TIM_HandleTypeDef* tim, EventTypes t) : Event(t, EventCategories::Timer), Tim(tim) {}

    TIM_HandleTypeDef* Tim = nullptr;
};
}    // namespace Nilai::Events
#    endif
#endif

#endif    // NILAI_EVENTS_TIMEVENT_H
