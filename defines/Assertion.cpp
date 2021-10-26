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

// For our C friends:
#if defined(NILAI_USE_LOGGER)
#    include "services/logger.hpp"
#endif

#if defined(__cplusplus)
extern "C"
{
#endif
    void AssertFailed(const uint8_t* file, uint32_t line, uint8_t shouldPrint)
    {
#if defined(NILAI_USE_LOGGER)
        if (shouldPrint == 1)
        {
            LOG_CRITICAL("An assertion failed: line %i, %s", line, file);
        }
#endif
        while (true)
        {
        }
    }
#if defined(__cplusplus)
}
#endif
