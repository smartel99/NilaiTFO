/**
 * @file    time.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#ifndef NILAI_TIME_H
#define NILAI_TIME_H

#if !defined(NILAI_TEST)
#    include "../defines/internal_config.h"
#    include NILAI_HAL_HEADER
#else
#    include <chrono>
#    include <thread>
#endif

namespace Nilai
{
using time_t = uint32_t;

inline static time_t GetTime()
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

inline static time_t GetTicks()
{
#if !defined(NILAI_TEST)
    return DWT->CYCCNT;    // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
#else
    return GetTime();
#endif
}

inline static void Delay(time_t ms)
{
#ifndef NILAI_TEST
    HAL_Delay(ms);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}
}    // namespace Nilai
#endif    // NILAI_TIME_H
