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
#    include <optional>
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
    std::function<void(const CommandEvent& cmd, const std::vector<uint8_t>&)> Respond;
    std::vector<uint8_t>                                                      Data;

    uint32_t PacketId = 0;
    uint8_t  Id       = 0;

    explicit CommandEvent(
      std::function<void(const CommandEvent& cmd, const std::vector<uint8_t>&)> f,
      std::vector<uint8_t>                                                      data)
    : Event(Events::EventTypes::CommandEvent, Events::EventCategories::Command),
      Respond(std::move(f))
    {
        if (data.size() >= s_headerSize)
        {
            PacketId = (static_cast<uint32_t>(data[0]) << 24) |
                       (static_cast<uint32_t>(data[1]) << 16) |
                       (static_cast<uint32_t>(data[2]) << 8) | (static_cast<uint32_t>(data[3]));

            Id   = data[4];
            Data = std::vector<uint8_t> {data.begin() + s_headerSize, data.end()};
        }
    }

    template<Command Cmd>
    [[nodiscard]] bool Is() noexcept
    {
        return Id == Cmd::id && HasEnoughDataForCmd<Cmd>();
    }

    template<Command Cmd>
    [[nodiscard]] std::optional<Cmd> As() noexcept
    {
        if (!Is<Cmd>())
        {
            return std::nullopt;
        }

        return Cmd {Data};
    }

private:
    template<Command Cmd>
    [[nodiscard]] bool HasEnoughDataForCmd() const noexcept
    {
        size_t expectedSize = 0;
        if constexpr (CommandHasPayload<Cmd>())
        {
            expectedSize += Cmd::payload_size;
        }

        if (Data.size() >= expectedSize)
        {
            return true;
        }
        else
        {
            LOG_ERROR("Exp: %d, got %d", expectedSize, Data.size());
            return false;
        }
    }

    static constexpr size_t s_headerSize = sizeof(PacketId) + sizeof(Id);
};
}    // namespace Nilai::Interfaces
//!@}
//!@}
//!@}
#endif
#endif    // GUARD_NILAI_INTERFACES_COMMANDS_EVENT_H
