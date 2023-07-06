/**
 * @file	assertion.h
 * @author	Samuel Martel
 * @brief
 * Created on: 13 juill. 2021
 *******************************************************************************
 */

#ifndef NILAI_ASSERTION_H
#define NILAI_ASSERTION_H

#include <cstdint>

#if defined(__cplusplus)
extern "C"
#endif
  [[noreturn]] void
  AssertFailed(const uint8_t* file, uint32_t line, uint8_t shouldPrint);

/* END OF FILE */
#endif /* NILAI_ASSERTION_H */
