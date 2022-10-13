/**
 * @file    command_interface.h
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

#ifndef GUARD_NILAI_INTERFACES_COMMANDS_COMMAND_INTERFACE_H
#define GUARD_NILAI_INTERFACES_COMMANDS_COMMAND_INTERFACE_H

#if defined(NILAI_USE_COMMAND_INTERFACE)

#    if !defined(NILAI_USE_EVENTS)
#        error Command Interface requires events to work!
#    endif

#    include "command.h"
#    include "command_interface_device.h"
#    include "event.h"

#    include "../../defines/events/events.h"
#    include "../../processes/application.h"

#    include "../../services/deserializer.h"
#    include "../../services/serializer.h"

#    include <array>
#    include <variant>

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
/**
 * @brief Concept describing a valid communication interface usable by the command interface.
 *
 * A valid communication interface is a class that supports the features described bellow.
 * <ul>
 *  <li>Registering the command interface to be able to dispatch a received command. The
 *  specifications of what a start of frame is is left to the interface.</li>
 *  <li>Sending a start of frame, indicating the beginning of a command</li>
 *  <li>Sending a single byte, used to send the command's ID</li>
 *  <li>Sending a packet of bytes under the form of a vector of uint8_t, used to send the command's
 *  payload</li>
 *  <li>Sending an end of frame, indicating the end of a command. The specifications of what an
 *  end of frame is is left to the interface.
 *  </li>
 *  <li>Waiting for a response to a command that was sent.</li>
 * </ul>
 *
 * These functionalities can be implemented in any desirable way, either static or non-static, for
 * as long as they comply with these function signatures:
 * <ul>
 *  <li>@code bool T::RegisterCommandInterface(const std::function<void(T&, const
 *      std::vector<uint8_t>&>& func) @endcode
 *      This method should keep @c func and invoke it whenever a command is received.
 *      It should return true if successful, false otherwise.
 *  </li>
 *  <li>@code bool T::WriteByte(uint8_t b) @endcode
 *      This method should transmit @c b in a way that is received by its destination.
 *      It should return true if successful, false otherwise.
 *  </li>
 *  <li>@code bool T::WriteDate(const std::vector<uint8_t>& data) @endcode
 *      This method should transmit @c data in a way that is received by its destination.
 *      It should return true if successful, false otherwise.
 *  </li>
 *  <li>@code bool T::SendSof() @endcode
 *      This method should transmit a signal that warns the destination of the beginning of a
 *      command. This method isn't required to actually do anything, but should still be present,
 *      even if it does not do anything.
 *      It should return true if successful, false otherwise.
 *  <li>
 *  <li>@code bool T::SendEof() @endcode
 *      This method should transmit a signal that warns the destination of the end of a command.
 *      This method isn't required to actually do anything, but should still be present, even if
 *      it does not do anything.
 *      It should return true if successful, false otherwise.
 *  </li>
 *  <li>@code std::vector<uint8_t> T::WaitForResponse(size_t expected) @endcode
 *      This method should wait for the destination to send a response to the command that has
 *      just been sent, then return that response.
 *  </li>
 * </ul>
 * @tparam T The communication interface type.
 *
 * @example command_interface.cpp
 *
 * @note The syntax @code static_assert
 * (Nilai::Interfaces::CheckIfCommandInterfaceDeviceIsValid<T>()) @endcode can be used to
 * validate your command interface device.
 */
template<typename T>
concept CommandInterfaceDevice = CommandInterfaceDeviceIsValid<T>();

/**
 *
 *
 * Command structure:
 * <ul>
 *  <li>Start of Frame (handled uniquely by the interface)</li>
 *  <li>Destination Address (4 bytes, optional)</li>
 *  <li>Packet ID (4 bytes)</li>
 *  <li>Command ID (1 byte)</li>
 *  <li>Command Payload (optional)</li>
 *  <li>End of Frame (handled uniquely by the interface)</li>
 * </ul>
 * @tparam Interfaces
 */
template<CommandInterfaceDevice... Interfaces>
class CommandInterface
{
public:
    explicit CommandInterface(Interfaces&... interfaces)
    {
        (interfaces.RegisterDispatcher(
           [this](decltype(interfaces) interface, const std::vector<uint8_t>& data)
           { ReceiveFrom(interface, data); }),
         ...);

        m_interfaces = {std::ref(interfaces)...};
    }

    /**
     * Sends the command to all interfaces that matches @c Interface
     * @tparam Interface
     * @tparam Cmd
     * @param cmd
     * @return
     */
    template<CommandInterfaceDevice Interface, Command Cmd>
    auto SendCommand(const Cmd& cmd)
    {
        using InterfaceRef = std::reference_wrapper<Interface>;
        for (auto&& interface : m_interfaces)
        {
            if (std::holds_alternative<InterfaceRef>(interface))
            {
                SendCommand(std::get<InterfaceRef>(interface), cmd);
            }
        }
    }

    template<CommandInterfaceDevice Interface, Command Cmd>
    auto SendCommand(Interface& interface, const Cmd& cmd)
        requires((std::same_as<std::remove_cvref_t<Interface>, Interfaces>) || ...)
    {
        interface.SendSoF();

        if constexpr (CommandHasAddress<Cmd>())
        {
            interface.WriteData(Serialize(cmd.address));
        }

        interface.WriteData(Serialize(m_atCommandId));
        m_atCommandId++;

        interface.WriteByte(cmd.id);

        if constexpr (CommandHasPayload<Cmd>())
        {
            interface.WriteData(Serialize(cmd.payload));
        }

        interface.SendEoF();

        if constexpr (CommandNeedsResponse<Cmd>())
        {
            using response_type = typename Cmd::response_type;
            return Deserialize<response_type>(interface.WaitForResponse());
        }
    }

    template<typename T>
    void ReceiveFrom(T& interface, const std::vector<uint8_t>& data)
        requires((std::same_as<std::remove_cvref_t<T>, Interfaces>) || ...)
    {
        CommandEvent cmd = CommandEvent(
          [this, interface](const std::vector<uint8_t>& d) { RespondTo(interface, d); }, data);

        Nilai::Application::Get().DispatchEvent(&cmd);
    }

    template<typename T>
    void RespondTo(T& interface, const std::vector<uint8_t>& data)
    {
        interface.WriteData(data);
    }

private:
    std::array<Internal::UniqueVariant_t<std::variant<Interfaces...>>, sizeof...(Interfaces)>
      m_interfaces;

    uint32_t m_atCommandId = 0;
};
}    // namespace Nilai::Interfaces
//!@}
//!@}
//!@}
#endif

#endif    // GUARD_NILAI_INTERFACES_COMMANDS_COMMAND_INTERFACE_H
