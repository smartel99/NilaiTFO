/**
 * @file    generic_event.h
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
#ifndef NILAI_EVENTS_GENERICEVENT_H
#define NILAI_EVENTS_GENERICEVENT_H

#if defined(NILAI_USE_EVENTS)

#    include "../../services/time.h"
#    include "types.h"

#    include <cstdint>
#    include <functional>

namespace Nilai::Events
{
/**
 * @brief Generic event structure.
 */
struct Event
{
    Event(EventTypes t, EventCategories c) : Timestamp(Nilai::GetTime()), Type(t), Category(c) {}
    virtual ~Event() = default;

    uint32_t        Timestamp = 0;
    EventTypes      Type;
    EventCategories Category;

    template<typename T>
    T& As() noexcept
        requires std::derived_from<T, Event>
    {
        return *reinterpret_cast<T*>(this);
    }

    [[nodiscard]] bool Is(EventCategories c, EventTypes t) const noexcept
    {
        return IsType(t) && IsCategory(c);
    }
    [[nodiscard]] bool IsType(EventTypes t) const noexcept { return t == Type; }
    [[nodiscard]] bool IsCategory(EventCategories t) const noexcept { return t == Category; }
};

/**
 * @brief A callback function should return true if the event should not be propagated further,
 * i.e if the following callbacks in the list should not be called after this one.
 */
using EventFunction = std::function<bool(Event* e)>;
}    // namespace Nilai::Events
#endif

#endif    // NILAI_EVENTS_GENERICEVENT_H
