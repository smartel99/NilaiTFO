/**
 * @file    i2s_event.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#ifndef NILAI_EVENTS_I2SEVENTS_H
#define NILAI_EVENTS_I2SEVENTS_H

#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_I2S_EVENTS)
#    if defined(NILAI_TEST)
#    else
#        include "generic_event.h"

#        include "../internal_config.h"
#        include NILAI_HAL_HEADER

namespace Nilai::Events
{
struct I2sEvent : public Event
{
    I2sEvent() : Event(EventTypes::I2S_Generic, EventCategories::I2S) {}
    I2sEvent(I2S_HandleTypeDef* i2s, EventTypes t) : Event(t, EventCategories::I2S), I2s(i2s) {}

    I2S_HandleTypeDef* I2s = nullptr;
};
}    // namespace Nilai::Events
#    endif
#endif

#endif    // NILAI_EVENTS_I2SEVENTS_H
