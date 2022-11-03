/**
 * @file    command_interface.cpp
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
#if defined(NILAI_USE_COMMAND_INTERFACE)
#    include "command_interface.h"

#    include <functional>

namespace Nilai::Interfaces
{
class ValidInterface
{
public:
    bool RegisterCommandInterface(
      [[maybe_unused]] const std::function<void(ValidInterface&, const std::vector<uint8_t>&)>&
        func)
    {
        return true;
    }

    bool WriteByte([[maybe_unused]] uint8_t b) { return true; }
    bool WriteData([[maybe_unused]] const std::vector<uint8_t>& data) { return true; }
    bool SendSoF() { return true; }
    bool SendEoF() { return true; }

    std::vector<uint8_t> WaitForResponse([[maybe_unused]] size_t len, [[maybe_unused]] size_t t) {
        return {}; }
};
static_assert(CommandInterfaceDevice<ValidInterface>);
static_assert(CheckIfCommandInterfaceDeviceIsValid<ValidInterface>());
static_assert(CommandInterfaceDeviceHasRegisterCommandInterface<ValidInterface>());
static_assert(CommandInterfaceDeviceHasWriteByte<ValidInterface>());
static_assert(CommandInterfaceDeviceHasWriteData<ValidInterface>());
static_assert(CommandInterfaceDeviceHasSendSoF<ValidInterface>());
static_assert(CommandInterfaceDeviceHasSendEoF<ValidInterface>());
static_assert(CommandInterfaceDeviceHasWaitForResponse<ValidInterface>());

//--------------------------------------------------------------------------------------------------
class BadVisibility
{
    bool RegisterCommandInterface(
      [[maybe_unused]] const std::function<void(BadVisibility&, const std::vector<uint8_t>&)>& func)
    {
        return true;
    }

    bool WriteByte([[maybe_unused]] uint8_t b) { return true; }
    bool WriteData([[maybe_unused]] const std::vector<uint8_t>& data) { return true; }
    bool SendSoF() { return true; }
    bool SendEoF() { return true; }

    std::vector<uint8_t> WaitForResponse() { return {}; }
};
static_assert(!CommandInterfaceDeviceHasRegisterCommandInterface<BadVisibility>());
static_assert(!CommandInterfaceDeviceHasWriteByte<BadVisibility>());
static_assert(!CommandInterfaceDeviceHasWriteData<BadVisibility>());
static_assert(!CommandInterfaceDeviceHasSendSoF<BadVisibility>());
static_assert(!CommandInterfaceDeviceHasSendEoF<BadVisibility>());
static_assert(!CommandInterfaceDeviceHasWaitForResponse<BadVisibility>());

//--------------------------------------------------------------------------------------------------
struct BadRegisterCommandInterfaceRet
{
    void RegisterCommandInterface(
      [[maybe_unused]] const std::function<void(BadRegisterCommandInterfaceRet&,
                                                const std::vector<uint8_t>&)>& func)
    {
    }
};
static_assert(!CommandInterfaceDeviceHasRegisterCommandInterface<BadRegisterCommandInterfaceRet>());

struct BadRegisterCommandInterfaceParam
{
    bool RegisterCommandInterface(
      [[maybe_unused]] const std::function<void(ValidInterface&, const std::vector<uint8_t>&)>&
        func)
    {
        return true;
    }
};
static_assert(
  !CommandInterfaceDeviceHasRegisterCommandInterface<BadRegisterCommandInterfaceParam>());

struct NoRegisterCommandInterface
{
};
static_assert(!CommandInterfaceDeviceHasRegisterCommandInterface<NoRegisterCommandInterface>());

//--------------------------------------------------------------------------------------------------
struct BadWriteByteRet
{
    void WriteByte([[maybe_unused]] uint8_t b) {}
};
static_assert(!CommandInterfaceDeviceHasWriteByte<BadWriteByteRet>());

struct BadWriteByteParam
{
    bool WriteByte() { return true; }
};
static_assert(!CommandInterfaceDeviceHasWriteByte<BadWriteByteParam>());

struct NoWriteByte
{
};
static_assert(!CommandInterfaceDeviceHasWriteByte<NoWriteByte>());

//--------------------------------------------------------------------------------------------------
struct BadWriteDataRet
{
    void WriteData([[maybe_unused]] const std::vector<uint8_t>& d) {}
};
static_assert(!CommandInterfaceDeviceHasWriteData<BadWriteDataRet>());

struct BadWriteDataParam
{
    bool WriteData([[maybe_unused]] const std::vector<int>& d) { return false; }
};
static_assert(!CommandInterfaceDeviceHasWriteData<BadWriteDataParam>());

struct NoWriteData
{
};
static_assert(!CommandInterfaceDeviceHasWriteData<NoWriteData>());

//--------------------------------------------------------------------------------------------------
struct BadSendSoFRet
{
    void SendSoF() {}
};
static_assert(!CommandInterfaceDeviceHasSendSoF<BadSendSoFRet>());

struct BadSendSoFParam
{
    bool SendSoF([[maybe_unused]] std::string s) { return true; }
};
static_assert(!CommandInterfaceDeviceHasSendSoF<BadSendSoFParam>());

struct NoSendSoF
{
};
static_assert(!CommandInterfaceDeviceHasSendSoF<NoSendSoF>());

//--------------------------------------------------------------------------------------------------
struct BadSendEoFRet
{
    void SendEoF() {}
};
static_assert(!CommandInterfaceDeviceHasSendEoF<BadSendEoFRet>());

struct BadSendEoFParam
{
    bool SendEoF([[maybe_unused]] std::string s) { return true; }
};
static_assert(!CommandInterfaceDeviceHasSendEoF<BadSendEoFParam>());

struct NoSendEoF
{
};
static_assert(!CommandInterfaceDeviceHasSendEoF<NoSendEoF>());

}    // namespace Nilai::Interfaces
#endif
