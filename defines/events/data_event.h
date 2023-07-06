/**
 * @file    data_event.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#ifndef NILAI_EVENTS_DATAEVENT_H
#define NILAI_EVENTS_DATAEVENT_H

#if defined(NILAI_USE_EVENTS)

#    include "generic_event.h"

#    include <cstdlib>
#    include <cstring>

namespace Nilai::Events
{
struct DataEvent : public Event
{
    void*  Data = nullptr;
    size_t Len  = 0;

    template<typename T>
    DataEvent(const T& data) : Event(EventTypes::DataEvent, EventCategories::Data)
    {
        Len  = sizeof(T);
        Data = malloc(Len);
        memcpy(Data, reinterpret_cast<void*>(const_cast<T*>(&data)), Len);
    }
    ~DataEvent() override { free(Data); }

    template<typename T>
    const T& As() const
    {
        return *reinterpret_cast<T*>(Data);
    }
};
}    // namespace Nilai::Events

#endif

#endif    // NILAI_EVENTS_DATAEVENT_H
