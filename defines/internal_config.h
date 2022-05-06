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
#ifndef NILAI_INTERNALCONFIG_H_
#define NILAI_INTERNALCONFIG_H_

/***********************************************/
/* Includes */

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
#elif defined(NILAI_TEST)
#    define NILAI_HAL_HEADER          ""
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


#define NILAI_USE_ADC    //! Enables the Analog-to-Digital Converter module.
//#define NILAI_USE_CAN    //! Enables the Controller Area Network module.
#define NILAI_USE_I2C    //! Enables the Inter-Integrated Circuit module.
//#define NILAI_USE_I2S    //! Enables the Inter-IC Sound module.
#define NILAI_USE_PWM     //! Enables the Pulse-Width Modulation module.
#define NILAI_USE_RTC     //! Enables the Real-Time Clock module.
#define NILAI_USE_SAI     //! Enables the Serial Audio Interface module.
#define NILAI_USE_SPI     //! Enables the Serial Peripheral Interface module.
#define NILAI_USE_UART    //! Enables the Universal Asynchronous Receiver-Transmitter module.

/***********************************************/
/* Function declarations */

/**
 * @}
 */
/* ----- !!!!! DO NOT MODIFY THIS FILE !!!!! ----- */
/* END OF FILE */
#endif /* NILAI_INTERNALCONFIG_H_ */
