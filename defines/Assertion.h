/**
 * @addtogroup Assertion.h
 * @{
 *******************************************************************************
 * @file	Assertion.h
 * @author	Samuel Martel
 * @brief
 * Created on: 13 juill. 2021
 *******************************************************************************
 */

#ifndef GUARD_ASSERTION_H
#define GUARD_ASSERTION_H

#include <cstdint>

#if defined(__cplusplus)
extern "C"
#endif
  [[noreturn]] void
  AssertFailed(const uint8_t* file, uint32_t line, uint8_t shouldPrint);

/**
 * @}
 */
/* END OF FILE */
#endif /* GUARD_ASSERTION_H */
