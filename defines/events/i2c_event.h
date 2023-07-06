/**
 * @file    i2c_event.h
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
#ifndef NILAI_EVENTS_I2CEVENTS_H
#define NILAI_EVENTS_I2CEVENTS_H

#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_I2C_EVENTS)
#    if defined(NILAI_TEST)
#    else
#        include "generic_event.h"

#        include "../internal_config.h"
#        include NILAI_HAL_HEADER

namespace Nilai::Events
{
struct I2cEvent : public Event
{
    I2cEvent() : Event(EventTypes::I2C_Generic, EventCategories::I2C) {}
    I2cEvent(I2C_HandleTypeDef* i2c, EventTypes t) : Event(t, EventCategories::Timer), I2c(i2c) {}

    I2C_HandleTypeDef* I2c = nullptr;
};

struct I2cAddrEvent : public I2cEvent
{
    I2cAddrEvent(I2C_HandleTypeDef* i2c, uint8_t direction, uint16_t addrMatchCode)
    : I2cEvent(i2c, EventTypes::I2C_Addr), Direction(direction), MatchCode(addrMatchCode)
    {
    }

    uint8_t  Direction = 0;
    uint16_t MatchCode = 0;
};
}    // namespace Nilai::Events


#    endif
#endif

#endif    // NILAI_EVENTS_I2CEVENTS_H
