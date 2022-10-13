/**
 * @file    event.h
 * @author  Samuel Martel
 * @date    2022-10-11
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

#ifndef GUARD_NILAI_INTERFACES_COMMANDS_EVENT_H
#define GUARD_NILAI_INTERFACES_COMMANDS_EVENT_H
#if defined(NILAI_USE_COMMAND_INTERFACE)

#    include "../../defines/events/generic_event.h"
#    include "../../defines/macros.h"

#    include "command.h"

#    include <functional>
#    include <utility>

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup Commands
 * @{
 */

namespace Nilai::Interfaces
{
struct CommandEvent : public Nilai::Events::Event
{
    std::function<void(const std::vector<uint8_t>&)> Respond;
    std::vector<uint8_t>                             Data;

    explicit CommandEvent(std::function<void(const std::vector<uint8_t>&)> f,
                          std::vector<uint8_t>                             data)
    : Event(Events::EventTypes::CommandEvent, Events::EventCategories::Command),
      Respond(std::move(f)),
      Data(std::move(data))
    {
    }

    template<Command Cmd>
    [[nodiscard]] bool Is() const
    {
        if (!HasEnoughDataForCmd<Cmd>())
        {
            return false;
        }

        constexpr size_t idIdx = CommandHasAddress<Cmd>() ? sizeof(Cmd::address) : 0;
        return Data[idIdx] == Cmd::id;
    }

    template<Command Cmd>
    [[nodiscard]] Cmd As() const
    {
        if (!HasEnoughDataForCmd<Cmd>())
        {
            return Cmd {};
        }

        return Cmd {Data};
    }

private:
    template<Command Cmd>
    [[nodiscard]] bool HasEnoughDataForCmd() const noexcept
    {
        constexpr size_t expectedSize =
          ((CommandHasAddress<Cmd>() ? sizeof(Cmd::address) : 0) + sizeof(Cmd::id)) +
          Cmd::payload_size;
        if (Data.size() == expectedSize)
        {
            return true;
        }
        else
        {
            NILAI_ASSERT(false, "Exp: %d, got %d", expectedSize, Data.size());
            return false;
        }
    }
};
}    // namespace Nilai::Interfaces
//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_COMMANDS_EVENT_H
