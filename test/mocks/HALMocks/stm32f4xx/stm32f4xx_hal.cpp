/**
 ******************************************************************************
 * @file    stm32f4xx.c
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-12-01
 *
 ******************************************************************************
 */

#include "stm32f4xx_hal.h"

#include <chrono>
#include <thread>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#    define NILAI_OS_WINDOWS
#    include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#    define NILAI_OS_LINUX
#    include <sys/sysinfo.h>
#else
#    error unsupported OS
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    void     HAL_Delay(uint32_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
    uint32_t HAL_GetTick(void)
    {
#ifdef NILAI_OS_WINDOWS
        return std::chrono::milliseconds(GetTickCount()).count();
#elif defined(NILAI_OS_LINUX)
        std::chrono::milliseconds uptime(0u);
        struct sysinfo x;
        if (sysinfo(&x) == 0)
        {
            uptime = std::chrono::milliseconds(
              static_cast<unsigned long long>(x.uptime)*1000ULL
            );
        }
        return uptime.count();
#else
    return 0;
#endif
    }

#ifdef __cplusplus
}
#endif

#undef NILAI_OS_WINDOWS
#undef NILAI_OS_LINUX
