/**
 * @file	internal_config.h
 * @author	Samuel Martel
 * @brief	Contains the internal configuration options for Nilai.
 * Created on: 2021/04/09
 *******************************************************************************
 */

/* ----- !!!!! DO NOT MODIFY THIS FILE !!!!! ----- */
#ifndef NILAI_INTERNALCONFIG_H_
#define NILAI_INTERNALCONFIG_H_

/***********************************************/
/* Includes */


// #if defined(NILAI_TEST)
// #    define NILAI_HAL_HEADER          "test/Mocks/HALMocks/stm32_mock_hal.h"
// #    define NILAI_UART_DATA_REG       RDR
// #    define NILAI_UART_IRQ_STATUS_REG ISR

// #    define NILAI_GPIO_BSRR_REG BSRR
// #    define NILAI_GPIO_IDR_REG  IDR
// #elif defined(NILAI_USES_STM32F4xx)
#if defined(NILAI_USES_STM32F4xx)
#    define NILAI_HAL_HEADER          "stm32f4xx_hal.h"
#    define NILAI_UART_DATA_REG       DR
#    define NILAI_UART_IRQ_STATUS_REG SR

#    define NILAI_GPIO_BSRR_REG BSRR
#    define NILAI_GPIO_IDR_REG  IDR
#elif defined(NILAI_USES_STM32F7xx)
#    define NILAI_HAL_HEADER          "stm32f7xx_hal.h"
#    define NILAI_UART_DATA_REG       RDR
#    define NILAI_UART_IRQ_STATUS_REG ISR

#    define NILAI_GPIO_BSRR_REG BSRR
#    define NILAI_GPIO_IDR_REG  IDR
#elif defined(NILAI_USES_STM32L4xx)
#    define NILAI_HAL_HEADER          "stm32l4xx_hal.h"
#    define NILAI_UART_DATA_REG       RDR
#    define NILAI_UART_IRQ_STATUS_REG ISR

#    define NILAI_GPIO_BSRR_REG BSRR
#    define NILAI_GPIO_IDR_REG  IDR
#else
#    define NILAI_HAL_HEADER ""
#    error No STM32 were defined in NilaiTFOConfig!

#    define NILAI_UART_DATA_REG       RDR
#    define NILAI_UART_IRQ_STATUS_REG ISR

#    define NILAI_GPIO_BSRR_REG BSRR
#    define NILAI_GPIO_IDR_REG  IDR
#endif

#if defined(NILAI_USE_UMO)
#    if defined(NILAI_UMO_USE_CAN)
#        if defined(NILAI_UMO_USE_UART)
#            error Cant use Umo with UART when it already uses CAN!
#        endif
#    elif defined(NILAI_UMO_USE_UART)
#        if defined(NILAI_UMO_USE_CAN)
#            error Cant use Umo with CAN when it already uses UART!
#        endif
#    else
#        error A communication protocol must be chosen for UMO!
#    endif
#endif

/***********************************************/
/* Defines */
#if defined(NILAI_USE_HW_TAS5760) || defined(NILAI_USE_SW_TAS5760)
#    define NILAI_USE_TAS5760
#endif

#if defined(NILAI_TEST)
#    define NILAI_NOP()
#else
#    define NILAI_NOP() asm("NOP")
#endif

/***********************************************/
/* Function declarations */

/* ----- !!!!! DO NOT MODIFY THIS FILE !!!!! ----- */
/* END OF FILE */
#endif /* NILAI_INTERNALCONFIG_H_ */
