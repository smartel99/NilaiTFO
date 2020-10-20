/**
 * @addtogroup NilaiTFOConfig.h
 * @{
 *******************************************************************************
 * @file	NilaiTFOConfig.h
 * @author	Samuel Martel
 * @brief
 * Created on: Oct 6, 2020
 *******************************************************************************
 */

#ifndef NILAITFOCONFIG_H_
#define NILAITFOCONFIG_H_

// Path of the main application header file
#define APPLICATION_HEADER "application.hpp"

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
// #define NILAI_USE_SPI
//#define NILAI_USE_UART

// Interfaces
//#define NILAI_USE_ADS
//#define NILAI_USE_HEARTBEAT
//#define NILAI_USE_MAX14763
//#define NILAI_USE_MAX14778
//#define NILAI_USE_MAX323
//#define NILAI_USE_PCA9505
//#define NILAI_USE_RELAY

// Services
//#define NILAI_USE_UMO
//#define NILAI_USE_SYSTEM

/******************************************************************************/
/* [SECTION]: Configuration                                                   */
/******************************************************************************/

/**
 * Warn if a module is included but the STM32 drivers are missing.
 * Set to 1 to enable, 0 to disable
 */
#define WARN_MISSING_STM_DRIVERS 1

/**
 * @}
 */
/* END OF FILE */
#endif /* NILAITFOCONFIG_H_ */
