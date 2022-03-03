/**
 * @file    Events.h
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
#ifndef NILAI_EVENTS_H
#define NILAI_EVENTS_H

#if defined(NILAI_USE_EVENTS)

#    include "DataEvent.h"
#    include "ExtEvent.h"
#    include "GenericEvent.h"


namespace cep::Events
{
/**
 * @brief List of the possible events.
 */
enum class EventTypes
{
    Exti0 = 0,    //! External Event on pin 0 AND/OR Software-triggered event.
    Exti1,        //! External Event on pin 1 AND/OR Software-triggered event.
    Exti2,        //! External Event on pin 2 AND/OR Software-triggered event.
    Exti3,        //! External Event on pin 3 AND/OR Software-triggered event.
    Exti4,        //! External Event on pin 4 AND/OR Software-triggered event.
    Exti5,        //! External Event on pin 5 AND/OR Software-triggered event.
    Exti6,        //! External Event on pin 6 AND/OR Software-triggered event.
    Exti7,        //! External Event on pin 7 AND/OR Software-triggered event.
    Exti8,        //! External Event on pin 8 AND/OR Software-triggered event.
    Exti9,        //! External Event on pin 9 AND/OR Software-triggered event.
    Exti10,       //! External Event on pin 10 AND/OR Software-triggered event.
    Exti11,       //! External Event on pin 11 AND/OR Software-triggered event.
    Exti12,       //! External Event on pin 12 AND/OR Software-triggered event.
    Exti13,       //! External Event on pin 13 AND/OR Software-triggered event.
    Exti14,       //! External Event on pin 14 AND/OR Software-triggered event.
    Exti15,       //! External Event on pin 15 AND/OR Software-triggered event.

    DataEvent,    //! A software event carrying data.

    //! Number of events, should always be last.
    Count
};

enum class SoftwareEvents
{
    Event0  = (int)EventTypes::Exti0,
    Event1  = (int)EventTypes::Exti1,
    Event2  = (int)EventTypes::Exti2,
    Event3  = (int)EventTypes::Exti3,
    Event4  = (int)EventTypes::Exti4,
    Event5  = (int)EventTypes::Exti5,
    Event6  = (int)EventTypes::Exti6,
    Event7  = (int)EventTypes::Exti7,
    Event8  = (int)EventTypes::Exti8,
    Event9  = (int)EventTypes::Exti9,
    Event10 = (int)EventTypes::Exti10,
    Event11 = (int)EventTypes::Exti11,
    Event12 = (int)EventTypes::Exti12,
    Event13 = (int)EventTypes::Exti13,
    Event14 = (int)EventTypes::Exti14,
};
}    // namespace cep::Events
#endif

#endif    // NILAI_EVENTS_H
