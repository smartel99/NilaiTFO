/**
 * @file    Tas5760Module.h
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
#ifndef NILAI_TAS5760_TAS5760MODULE_H
#define NILAI_TAS5760_TAS5760MODULE_H

#include "../../defines/internal_config.h"
#define NILAI_USE_TAS5760
#if defined(NILAI_USE_TAS5760)

#    if !defined(NILAI_USE_I2S) && !defined(NILAI_USE_SAI)
#        error An audio device must be enabled to use the TAS5760 module!
#    endif

#    include "enums.h"

#    include <concepts>
#    include <string>

namespace Nilai::Interfaces
{
template<typename T>
concept CommunicationDevice = requires(T t) {
                                  t.GetHandle();
                                  t.StartClock();
                                  t.StopClock();
                                  {
                                      t.Stream(nullptr, 0)
                                      } -> std::same_as<bool>;
                                  {
                                      t.PauseStream()
                                      } -> std::same_as<bool>;
                                  {
                                      t.ResumeStream()
                                      } -> std::same_as<bool>;
                                  {
                                      t.StopStream()
                                      } -> std::same_as<bool>;
                              };


template<CommunicationDevice Device>
class Tas5760Module : public Device
{
protected:
    using Handle = typename std::result_of<decltype (&Device::GetHandle)(Device)>::type;

public:
    template<typename... Args>
    Tas5760Module(Handle handle, const std::string& label, Args... args)
    : Device(handle, label, args...)
    {
    }

    virtual void               ToggleSleep([[maybe_unused]] bool s) {}
    [[nodiscard]] virtual bool IsAsleep() const { return true; }

    virtual void               ToggleShutdown([[maybe_unused]] bool s) {}
    [[nodiscard]] virtual bool IsShutdown() const { return true; }
};
}    // namespace Nilai::Interfaces
#endif

#endif    // NILAI_TAS5760_TAS5760MODULE_H
