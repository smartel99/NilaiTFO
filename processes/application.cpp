/**
 * @addtogroup application.cpp
 * @{
 *******************************************************************************
 * @file	application.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: May 19, 2021
 *******************************************************************************
 */
#include "application.hpp"

[[noreturn]] void cep::AbortionHandler(int signal)
{
    (void)signal;
    // This function will in most cases be called by the STL, in place of the standard _exit().
    AssertFailed((const uint8_t*)__FILE__, __LINE__, 1);
    while (1)
    {
    }
}
