/**
 * @addtogroup internalConfig.h
 * @{
 *******************************************************************************
 * @file	internalConfig.h
 * @author	Samuel Martel
 * @brief	Contains the internal configuration options for Nilai.
 * Created on: 2021/04/09
 *******************************************************************************
 */

/* ----- !!!!! DO NOT MODIFY THIS FILE !!!!! ----- */
#ifndef INTERNALCONFIG_H_
#define INTERNALCONFIG_H_

/***********************************************/
/* Includes */

#if defined(NILAI_USES_STM32F4xx)
#define NILAI_HAL_HEADER          "stm32f4xx_hal.h"
#define NILAI_UART_DATA_REG       DR
#define NILAI_UART_IRQ_STATUS_REG SR
#elif defined(NILAI_USES_STM32L4xx)
#define NILAI_HAL_HEADER          "stm32l4xx_hal.h"
#define NILAI_UART_DATA_REG       RDR
#define NILAI_UART_IRQ_STATUS_REG ISR
#else
#define NILAI_HAL_HEADER ""
#error No STM32 were defined in NilaiTFOConfig!
#endif




/***********************************************/
/* Defines */


/***********************************************/
/* Function declarations */



/**
 * @}
 */
/* ----- !!!!! DO NOT MODIFY THIS FILE !!!!! ----- */
/* END OF FILE */
#endif /* INTERNALCONFIG_H_ */
