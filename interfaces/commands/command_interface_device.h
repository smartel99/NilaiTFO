/**
 * @file    command_interface_device.h
 * @author  Samuel Martel
 * @date    2022-10-12
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

#ifndef GUARD_NILAI_INTERFACES_COMMANDS_COMMAND_INTERFACE_DEVICE_H
#define GUARD_NILAI_INTERFACES_COMMANDS_COMMAND_INTERFACE_DEVICE_H

#if defined(NILAI_USE_COMMAND_INTERFACE)
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

#    include <variant>

namespace Nilai::Interfaces
{
template<typename T>
consteval bool CommandInterfaceDeviceHasRegisterCommandInterface()
{
    return requires(T t) {
               {
                   t.RegisterCommandInterface([](T&, const std::vector<uint8_t>&) {})
                   } -> std::same_as<bool>;
           };
};


template<typename T>
consteval bool CommandInterfaceDeviceHasWriteByte()
{
    return requires(T t) {
               {
                   t.WriteByte(static_cast<uint8_t>(0))
                   } -> std::same_as<bool>;
           };
}

template<typename T>
consteval bool CommandInterfaceDeviceHasWriteData()
{
    return requires(T t) {
               {
                   t.WriteData(std::vector<uint8_t> {})
                   } -> std::same_as<bool>;
           };
};

template<typename T>
consteval bool CommandInterfaceDeviceHasSendSoF()
{
    return requires(T t) {
               {
                   t.SendSoF()
                   } -> std::same_as<bool>;
           };
}

template<typename T>
consteval bool CommandInterfaceDeviceHasSendEoF()
{
    return requires(T t) {
               {
                   t.SendEoF()
                   } -> std::same_as<bool>;
           };
}

template<typename T>
consteval bool CommandInterfaceDeviceHasWaitForResponse()
{
    return requires(T t) {
               {
                   t.WaitForResponse(static_cast<size_t>(0))
                   } -> std::same_as<std::vector<uint8_t>>;
           };
}

template<typename T>
consteval bool CommandInterfaceDeviceIsValid()
{
    static_assert(CommandInterfaceDeviceHasRegisterCommandInterface<T>(),
                  "Interface does not have "
                  "a function with the signature 'bool T::RegisterCommandInterface(const "
                  "std::function<void(T&, const std::vector<uint8_t>&)>&)'");

    static_assert(CommandInterfaceDeviceHasWriteByte<T>(),
                  "Interface does not have a function "
                  "with the signature 'bool T::WriteByte(uint8_t)'");

    static_assert(CommandInterfaceDeviceHasWriteData<T>(),
                  "Interface does not have a function "
                  "with the signature 'bool T::WriteData(const std::vector<uint8_t>&)'");

    static_assert(CommandInterfaceDeviceHasSendSoF<T>(),
                  "Interface does not have a function with"
                  " the signature 'bool T::SendSoF()'");

    static_assert(CommandInterfaceDeviceHasSendEoF<T>(),
                  "Interface does not have a function with"
                  " the signature 'bool T::SendEoF()'");

    static_assert(CommandInterfaceDeviceHasWaitForResponse<T>(),
                  "Interface does not have a function with the signature 'std::vector<uint8_t> "
                  "T::WaitForResponse(size_t)'");

    return true;
}

template<typename T>
consteval bool CheckIfCommandInterfaceDeviceIsValid()
{
    return CommandInterfaceDeviceHasRegisterCommandInterface<T>() &&
           CommandInterfaceDeviceHasWriteByte<T>() && CommandInterfaceDeviceHasWriteData<T>() &&
           CommandInterfaceDeviceHasSendSoF<T>() && CommandInterfaceDeviceHasSendEoF<T>();
}

namespace Internal
{
template<typename T, typename... Ts>
struct FilterDuplicates
{
    using type = T;
};

template<template<typename...> class C, typename... Ts, typename U, typename... Us>
struct FilterDuplicates<C<Ts...>, U, Us...>
: std::conditional_t<(std::is_same_v<U, Ts> || ...),
                     FilterDuplicates<C<Ts...>, Us...>,
                     FilterDuplicates<C<Ts..., U>, Us...>>
{
};

template<typename T>
struct UniqueVariant;

template<typename... Ts>
struct UniqueVariant<std::variant<Ts...>>
: FilterDuplicates<std::variant<>, std::monostate, std::reference_wrapper<Ts>...>
{
};

template<typename T>
using UniqueVariant_t = typename UniqueVariant<T>::type;
}    // namespace Internal
}    // namespace Nilai::Interfaces

//!@}
//!@}
//!@}
#endif

#endif    // GUARD_NILAI_INTERFACES_COMMANDS_COMMAND_INTERFACE_DEVICE_H
