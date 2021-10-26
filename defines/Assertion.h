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

#ifndef ASSERTION_H_
#define ASSERTION_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C"
#endif
    void
    AssertFailed(const uint8_t* file, uint32_t line, uint8_t shouldPrint);

/**
 * @}
 */
/* END OF FILE */
#endif /* ASSERTION_H_ */
