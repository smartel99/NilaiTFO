/**
 * @file    ExtEvent.h
 * @author  Samuel Martel
 * @date    2022-03-03
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
#ifndef NILAI_EVENTS_EXTEVENT_H
#define NILAI_EVENTS_EXTEVENT_H

#if defined(NILAI_USE_EVENTS)

#    include "generic_event.h"

namespace Nilai::Events
{
/**
 * @brief External event triggered by a GPIO or by software.
 */
struct ExtEvent : public Event
{
    ExtEvent() : Event(EventTypes::Exti_Generic, EventCategories::External) {}
    ExtEvent(bool fromSoft, uint8_t ch, EventTypes t)
    : Event(t, EventCategories::External), IsFromSoft(fromSoft), SrcCh(ch)
    {
    }

    //! Was the event triggered by software?
    bool IsFromSoft = false;

    //! Which channel is the event on?
    uint8_t SrcCh = 0;
};
}    // namespace Nilai::Events

#endif

#endif    // NILAI_EVENTS_EXTEVENT_H
