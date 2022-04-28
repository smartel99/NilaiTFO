/**
 * @file    Time.h
 * @author  Samuel Martel
 * @date    2022-04-21
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
#ifndef NILAI_TIME_H
#define NILAI_TIME_H

#ifndef NILAI_TEST
#    include "defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#else
#    if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#        define NILAI_OS_WINDOWS
#        include <windows.h>
#    elif defined(unix) || defined(__unix) || defined(__unix__)
#        define NILAI_OS_LINUX
#        include <sys/sysinfo.h>
#    else
#        error unsupported OS
#    endif
#    include <chrono>
#    include <thread>
#endif

namespace cep
{
inline static uint32_t GetTime()
{
#ifndef NILAI_TEST
    return HAL_GetTick();
#else
#    ifdef NILAI_OS_WINDOWS
    return std::chrono::milliseconds(GetTickCount()).count();
#    elif defined(NILAI_OS_LINUX)
    std::chrono::milliseconds uptime(0u);
    struct sysinfo            x;
    if (sysinfo(&x) == 0)
    {
        uptime = std::chrono::milliseconds(static_cast<unsigned long long>(x.uptime) * 1000ULL);
    }
    return uptime.count();
#    else
    return 0;
#    endif
#endif
}

inline static void Delay(uint32_t ms)
{
#ifndef NILAI_TEST
    HAL_Delay(ms);
#else
#    ifdef NILAI_OS_WINDOWS
    Sleep(ms);
#    else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#    endif
#endif
}
}    // namespace cep
#endif    // NILAI_TIME_H
