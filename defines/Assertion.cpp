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
#include "Assertion.h"

#if defined(NILAI_USE_LOGGER)
#    include "../services/logger.hpp"
#endif

#if defined(__cplusplus)
#define NO_RETURN [[noreturn]]
#else
#define NO_RETURN noreturn
#endif

// For our C friends:
#if defined(__cplusplus)
extern "C"
{
#endif
    NO_RETURN void AssertFailed(const uint8_t* file, uint32_t line, uint8_t shouldPrint)
    {
#if defined(NILAI_USE_LOGGER)
        if (shouldPrint == 1)
        {
            LOG_CRITICAL("An assertion failed: line %i, %s", line, file);
        }
#else
        (void)file;
        (void)line;
        (void)shouldPrint;
#endif
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
