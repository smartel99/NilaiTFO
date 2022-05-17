/**
 * @addtogroup Assertion.cpp
 * @{
 *******************************************************************************
 * @file	Assertion.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: 13 juill. 2021
 *******************************************************************************
 */
#include "assertion.h"

#if defined(NILAI_USE_LOGGER)
#    include "../services/logger.h"
#endif

#include "system.h"

// For our C friends:
#if defined(__cplusplus)
extern "C"
{
#endif
    [[noreturn]] void AssertFailed(const uint8_t* file, uint32_t line, uint8_t shouldPrint)
    {
#if defined(NILAI_USE_LOGGER)
        if (shouldPrint == 1)
        {
            LOG_CRITICAL("An assertion failed: line %i, %s", line, file);
        }
#else
    NILAI_UNUSED(file);
    NILAI_UNUSED(line);
    NILAI_UNUSED(shouldPrint);
#endif

        if (Nilai::System::IsDebuggerConnected())
        {
            NILAI_BREAKPOINT;
        }

        while (true)
        {
        }
    }
#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
