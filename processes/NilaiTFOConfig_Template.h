/**
 * @addtogroup NilaiTFOConfig_Template.h
 * @{
 *******************************************************************************
 * @file    NilaiTFOConfig_Template.h
 * @author  Samuel Martel
 * @brief
 * Created on: 2020/10/06
 * Updated on: 2021/04/09
 *******************************************************************************
 */

#ifndef NILAI_NILAITFOCONFIG_H
#define NILAI_NILAITFOCONFIG_H

// Path of the main application header file.
#define APPLICATION_HEADER "processes/MasterApplication.h"

// Path of NilaiTFO
#define NILAI_PATH "NilaiTFO/"

/******************************************************************************/
/* [SECTION]: Configuration                                                   */
/******************************************************************************/

/**
 * Warn if a module is included but the STM32 drivers are missing.
 * Set to 1 to enable, 0 to disable
 */
#define WARN_MISSING_STM_DRIVERS 1

/**
 * Select the STM32 you are using.
 * This is extremely important for the activated modules that depends on HAL functions.
 * Obviously, don't try enabling more than one at the same time, that won't work.
 */
//#define NILAI_USES_STM32F4xx
//#define NILAI_USES_STM32L4xx

/**
 * The maximum amount of modules that can operate simultaneously in the application.
 * Default: 16
 */
#define NILAI_MAX_MODULE_AMOUNT 16

/**
 * Select if the logger should use the system time or the RTC time.
 * To use the RTC, the RTC module must be enabled.
 * Uncomment to use the RTC, comment to use the system clock.
 */
#define NILAI_LOGGER_USE_RTC

/**
 * Toggle each level of logging.
 * To enable a level of logging, uncomment the definition, otherwise comment it.
 */
#define NILAI_LOG_ENABLE_DEBUG
#define NILAI_LOG_ENABLE_INFO
#define NILAI_LOG_ENABLE_WARNING
#define NILAI_LOG_ENABLE_ERROR
#define NILAI_LOG_ENABLE_CRITICAL

/**
 * Defines the hardware layer used by Umo.
 * Selects if the hardware layer should be UART or CAN.
 * Can only have one at a time!
 */
//#define NILAI_UMO_USE_CAN
#define NILAI_UMO_USE_UART

/**
 * Toggles the use of the event system.
 */
//#define NILAI_USE_EVENTS

#if defined(NILAI_USE_EVENTS)
/**
 * Set the number of callbacks that can be assigned to an event (Default: 1).
 */
#    define NILAI_EVENTS_MAX_CALLBACKS 1

#    define NILAI_USE_ADC_EVENTS
//#    define NILAI_USE_CAN_EVENTS
#    define NILAI_USE_I2C_EVENTS
#    define NILAI_USE_I2S_EVENTS
#    define NILAI_USE_RTC_EVENTS
#    define NILAI_USE_SAI_EVENTS
#    define NILAI_USE_SPI_EVENTS
#    define NILAI_USE_TIMER_EVENTS
#    define NILAI_USE_UART_EVENTS
#endif

/******************************************************************************/
/* [SECTION]: Module Activation                                               */
/******************************************************************************/
/**
 * Uncomment a define to use that module, or comment it to disable it
 */
// Drivers
//#define NILAI_USE_ADC     //! Enables the Analog-to-Digital Converter module.
//#define NILAI_USE_CAN     //! Enables the Controller Area Network module.
//#define NILAI_USE_I2C     //! Enables the Inter-Integrated Circuit module.
//#define NILAI_USE_I2S     //! Enables the Inter-IC Sound module.
//#define NILAI_USE_PWM     //! Enables the Pulse-Width Modulation module.
//#define NILAI_USE_RTC     //! Enables the Real-Time Clock module.
//#define NILAI_USE_SAI     //! Enables the Serial Audio Interface module.
//#define NILAI_USE_SPI     //! Enables the Serial Peripheral Interface module.
//#define NILAI_USE_UART    //! Enables the Universal Asynchronous Receiver-Transmitter module.

// Interfaces
//#define NILAI_USE_ADS
//#define NILAI_USE_DS2484
//#define NILAI_USE_ESP32
//#define NILAI_USE_HEARTBEAT
//#define NILAI_USE_LTC2498
//#define NILAI_USE_MAX14763
//#define NILAI_USE_MAX14778
//#define NILAI_USE_MAX323
//#define NILAI_USE_PCA9505
//#define NILAI_USE_RELAY
//#define NILAI_USE_RN2903
//#define NILAI_USE_TLP3545
//#define NILAI_USE_TAS5707
//#define NILAI_USE_HW_TAS5760
//#define NILAI_USE_SW_TAS5760

// Services
//#define NILAI_USE_UMO
//#define NILAI_USE_SYSTEM
//#define NILAI_USE_LOGGER
//#define NILAI_USE_FILE_LOGGER
//#define NILAI_USE_FILESYSTEM
//#define NILAI_USE_INI_PARSER

/******************************************************************************/
/* [SECTION]: Module-Specific Settings                                        */
/******************************************************************************/
#if defined(NILAI_USE_SW_TAS5760) || defined(NILAI_USE_HW_TAS5760)
//--- TAS5760.

/**
 * Controls if the PWM outputs should be disabled (none), in mono (just one) or in stereo (two
 * outputs).
 */
#    define NILAI_TAS5760_PWM_NONE   0
#    define NILAI_TAS5760_PWM_MONO   1
#    define NILAI_TAS5760_PWM_STEREO 2
#    define NILAI_TAS5760_PWM_MODE   NILAI_TAS5760_PWM_NONE

/**
 * Controls if I2C writes should be verified or not.
 */
#    define NILAI_TAS5760_VERIFY_WRITE
#endif

#if defined(NILAI_USE_RTC)
//--- Real-Time Clock settings.

/**
 * @brief Enables the use of the standard library to convert epoch time to timestamps and
 * vice-versa.
 * @note This adds significant bloat to the binary file (mktime taking 820 bytes just by itself).
 */
//#define NILAI_RTC_USE_STL
#endif

/**
 * @}
 */
/* END OF FILE */
#endif /* NILAI_NILAITFOCONFIG_H */
