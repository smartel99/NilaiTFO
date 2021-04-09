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

#ifndef NILAITFOCONFIG_H_
#define NILAITFOCONFIG_H_

// Path of the main application header file.
#define APPLICATION_HEADER "processes/MasterApplication.h"

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

/******************************************************************************/
/* [SECTION]: Module Activation                                               */
/******************************************************************************/
/**
 * Uncomment a define to use that module, or comment it to disable it
 */
// Drivers
//#define NILAI_USE_ADC
//#define NILAI_USE_CAN
//#define NILAI_USE_I2C
//#define NILAI_USE_SPI
//#define NILAI_USE_UART
//#define NILAI_USE_PWM
//#define NILAI_USE_RTC

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
//#define NILAI_USE_SD
//#define NILAI_USE_TLP3545

// Services
//#define NILAI_USE_UMO
//#define NILAI_USE_SYSTEM
//#define NILAI_USE_LOGGER
//#define NILAI_USE_FILE_LOGGER

/**
 * @}
 */
/* END OF FILE */
#endif /* NILAITFOCONFIG_H_ */
