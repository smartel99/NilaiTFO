/**
 * @file    NilaiTFO_config_Template.h
 * @author  Samuel Martel
 * @brief
 * Created on: 2020/10/06
 * Updated on: 2021/04/09
 *******************************************************************************
 */
/**
 * @addtogroup Nilai
 * @{
 */
/**
 * @addtogroup Configuration
 * @{
 * @brief Configuration of NilaiTFO.
 *
 * These settings should be made available to every translation units.
 */
#error asd
#ifndef NILAI_NILAITFOCONFIG_H_a
#    define NILAI_NILAITFOCONFIG_H

/**
 * @defgroup nilai_global_cfg Global configuration
 * @{
 * @brief Global options for Nilai.
 */

/**
 * @addtogroup APPLICATION_HEADER
 * @{
 * @brief Path of the main application's header file.
 */
#    define APPLICATION_HEADER "processes/MasterApplication.h"
//!@}

/**
 * @addtogroup NILAI_PATH
 * @{
 * @brief Path of Nilai relative to the root of the project.
 */
#    define NILAI_PATH "NilaiTFO/"
//!@}
//!@}

/**
 * @addtogroup Profiling
 * @{
 */

/**
 * @addtogroup NILAI_ENABLE_PROFILING
 * @{
 * @brief Toggles the profiling of code.
 */
#    define NILAI_ENABLE_PROFILING
//!@}

/**
 * @addtogroup NILAI_MAX_PROFILE_EVENTS
 * @{
 * @brief Sets the maximum number of events that can be profiled simultaneously.
 */
#    define NILAI_MAX_PROFILE_EVENTS 20
//!@}
//!@}

/**
 * @defgroup nilai_app_cfg Application Settings
 * @{
 * @brief Low-level settings to configure the back-end of Nilai.
 */
/**
 * @addtogroup WARN_MISSING_STM_DRIVERS
 * @{
 * @brief Warn if a module is included but the STM32 drivers are missing.
 * Set to 1 to enable, 0 to disable
 */
#    define WARN_MISSING_STM_DRIVERS 1
//!@}

/**
 * @addtogroup nilai_mcu_selection MCU Selection
 * @{
 * @brief Selects the MCU you are using.
 *
 * This is extremely important for the activated modules that depends on HAL functions.
 * Obviously, don't try enabling more than one at the same time, that won't work.
 */
/**
 * @addtogroup NILAI_USES_STM32F4xx
 * @{
 * @brief Make Nilai use the backend for the STM32F4xx.
 */
//#define NILAI_USES_STM32F4xx
//!@}

/**
 * @addtogroup NILAI_USES_STM32L4xx
 * @{
 * @brief Make Nilai use the backend for the STM32L4xx.
 */
//#define NILAI_USES_STM32L4xx
//!@}
//!@}

/**
 * @addtogroup NILAI_MAX_MODULE_AMOUNT
 * @{
 * @brief The maximum amount of modules that can operate simultaneously in the application.
 *
 * Default: 16
 */
#    define NILAI_MAX_MODULE_AMOUNT 16
//!@}

/**
 * @addtogroup nilai_log_opts Logging Options
 * @{
 */
/**
 * @addtogroup NILAI_LOGGER_USE_RTC
 * @{
 * @brief Select if the logger should use the system time or the RTC time.
 *
 * Uncomment to use the RTC, comment to use the system clock.
 *
 * @attention To use the RTC, the RTC module must be enabled.
 */
#    define NILAI_LOGGER_USE_RTC
//!@}

/**
 * @defgroup nilai_log_opt_log_levels Log Levels
 * @{
 * @brief Controls which types of log messages should be printed.
 */
/**
 * @addtogroup NILAI_LOG_ENABLE_DEBUG
 * @{
 * @brief Toggles Debug messages.
 */
#    define NILAI_LOG_ENABLE_DEBUG
//!@}

/**
 * @addtogroup NILAI_LOG_ENABLE_INFO
 * @{
 * @brief Toggles Info messages.
 */
#    define NILAI_LOG_ENABLE_INFO
//!@}

/**
 * @addtogroup NILAI_LOG_ENABLE_WARNING
 * @{
 * @brief Toggles Warning messages.
 */
#    define NILAI_LOG_ENABLE_WARNING
//!@}

/**
 * @addtogroup NILAI_LOG_ENABLE_ERROR
 * @{
 * @brief Toggles Error messages.
 */
#    define NILAI_LOG_ENABLE_ERROR
//!@}

/**
 * @addtogroup NILAI_LOG_ENABLE_CRITICAL
 * @{
 * @brief Toggles Critical messages.
 */
#    define NILAI_LOG_ENABLE_CRITICAL
//!@}
//!@}
//!@}

/**
 * @addtogroup nilai_app_opt_umo Umo Settings
 * @{
 * @brief Defines the hardware layer used by Umo.
 *
 * Selects if the hardware layer should be UART or CAN.
 *
 * @attention Can only have one at a time!
 */
/**
 * @addtogroup NILAI_UMO_USE_CAN
 * @{
 * @brief If defined, the Umo module will use the CAN peripheral.
 */
//#define NILAI_UMO_USE_CAN
//!@}

/**
 * @addtogroup NILAI_UMO_USE_UART
 * @{
 * @brief If defined, the Umo module will use the UART peripheral.
 */
#    define NILAI_UMO_USE_UART
//!@}
//!@}

/**
 * @addtogroup nilai_event_opts Event Options
 * @{
 * @brief Options related to the Event subsystem.
 */
/**
 * @addtogroup NILAI_USE_EVENTS
 * @{
 * @brief If defined, enables the event subsystem.
 */
//#    define NILAI_USE_EVENTS
//! @}

#    if defined(NILAI_USE_EVENTS)
/**
 * @addtogroup NILAI_EVENTS_MAX_CALLBACKS
 * @{
 * @brief Set the number of callbacks that can be assigned to an event (Default: 1).
 */
#        define NILAI_EVENTS_MAX_CALLBACKS 1
//!@}

/**
 * @addtogroup NILAI_USE_ADC_EVENTS
 * @{
 * @brief If defined, ADC events will be enabled and used.
 */
//#    define NILAI_USE_ADC_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_CAN_EVENTS
 * @{
 * @brief If defined, CAN events will be enabled and used.
 */
//#    define NILAI_USE_CAN_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_I2C_EVENTS
 * @{
 * @brief If defined, I2C events will be enabled and used.
 */
//#    define NILAI_USE_I2C_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_I2S_EVENTS
 * @{
 * @brief If defined, I2S events will be enabled and used.
 */
//#    define NILAI_USE_I2S_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_RTC_EVENTS
 * @{
 * @brief If defined, RTC events will be enabled and used.
 */
//#    define NILAI_USE_RTC_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_SAI_EVENTS
 * @{
 * @brief If defined, SAI events will be enabled and used.
 */
//#    define NILAI_USE_SAI_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_SPI_EVENTS
 * @{
 * @brief If defined, SPI events will be enabled and used.
 */
//#    define NILAI_USE_SPI_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_TIMER_EVENTS
 * @{
 * @brief If defined, TIMER events will be enabled and used.
 */
//#    define NILAI_USE_TIMER_EVENTS
//!@}

/**
 * @addtogroup NILAI_USE_UART_EVENTS
 * @{
 * @brief If defined, UART events will be enabled and used.
 */
//#    define NILAI_USE_UART_EVENTS
//!@}
#    endif
//!@}
//!@}

/**
 * @addtogroup nilai_module_selection Module Selection
 * @{
 * @brief Use these preprocessor definitions to enable/disable the modules of Nilai.
 */

/**
 * @addtogroup nilai_driver_selection Drivers
 * @{
 * @brief Low-level driver module offering a direct with the hardware peripherals.
 */

/**
 * @addtogroup NILAI_USE_ADC
 * @{
 * @brief If defined, enables the ADC module.
 */
//#define NILAI_USE_ADC     //! Enables the Analog-to-Digital Converter module.
//!@}

/**
 * @addtogroup NILAI_USE_CAN
 * @{
 * @brief If defined, enables the CAN module.
 */
//#define NILAI_USE_CAN     //! Enables the Controller Area Network module.
//!@}

/**
 * @addtogroup NILAI_USE_I2C
 * @{
 * @brief If defined, enables the I2C module.
 */
//#define NILAI_USE_I2C     //! Enables the Inter-Integrated Circuit module.
//!@}

/**
 * @addtogroup NILAI_USE_I2S
 * @{
 * @brief If defined, enables the I2S module.
 */
//#define NILAI_USE_I2S     //! Enables the Inter-IC Sound module.
//!@}

/**
 * @addtogroup NILAI_USE_PWM
 * @{
 * @brief If defined, enables the PWM module.
 */
//#define NILAI_USE_PWM     //! Enables the Pulse-Width Modulation module.
//!@}

/**
 * @addtogroup NILAI_USE_RTC
 * @{
 * @brief If defined, enables the RTC module.
 */
//#define NILAI_USE_RTC     //! Enables the Real-Time Clock module.
//!@}

/**
 * @addtogroup NILAI_USE_SAI
 * @{
 * @brief If defined, enables the SAI module.
 */
//#define NILAI_USE_SAI     //! Enables the Serial Audio Interface module.
//!@}

/**
 * @addtogroup NILAI_USE_SPI
 * @{
 * @brief If defined, enables the SPI module.
 */
//#define NILAI_USE_SPI     //! Enables the Serial Peripheral Interface module.
//!@}

/**
 * @addtogroup NILAI_USE_UART
 * @{
 * @brief If defined, enables the UART module.
 */
//#define NILAI_USE_UART    //! Enables the Universal Asynchronous Receiver-Transmitter module.
//!@}
//!@}

/**
 * @addtogroup nilai_interface_selection Interfaces
 * @{
 * @brief Modules utilizing @ref Drivers to interface with integrated circuits connected to the
 * MCU.
 */

/**
 * @addtogroup NILAI_USE_ADS
 * @{
 * @brief If defined, enables the <a
 * href=https://www.ti.com/lit/ds/symlink/ads131a04.pdf?ts=1659994576841&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS131A04>ADS131</a>
 * (ADC) module.
 */
//#define NILAI_USE_ADS
//!@}

/**
 * @addtogroup NILAI_USE_AT24QT2120
 * @{
 * @brief If defined, enables the <a
 * href=https://ww1.microchip.com/downloads/en/DeviceDoc/doc9634.pdf>AT24QT2120</a> (Capacitive
 * touch sensor) module.
 */
//#define NILAI_USE_AT24QT2120
//!@}

/**
 * @addtogroup NILAI_USE_HEARTBEAT
 * @{
 * @brief If defined, enables the heartbeat module, which toggles a pin at a steady rate.
 */
//#define NILAI_USE_HEARTBEAT
//!@}

/**
 * @addtogroup NILAI_USE_LTC2498
 * @{
 * @brief If defined, enables the <a
 * href=https://www.analog.com/media/en/technical-documentation/data-sheets/2498fg.pdf>LTC2498</a>
 * (ADC) module.
 */
//#define NILAI_USE_LTC2498
//!@}

/**
 * @addtogroup NILAI_USE_MAX14763
 * @{
 * @brief If defined, enables the <a
 * href=https://datasheets.maximintegrated.com/en/ds/MAX14759-MAX14763.pdf>MAX14763</a> (2:1
 * analog switch) module.
 */
//#define NILAI_USE_MAX14763
//!@}

/**
 * @addtogroup NILAI_USE_MAX14778
 * @{
 * @brief If defined, enables the <a
 * href=https://datasheets.maximintegrated.com/en/ds/MAX14778.pdf>MAX14778</a> (4:1 analog
 * multiplexer) module.
 */
//#define NILAI_USE_MAX14778
//!@}

/**
 * @addtogroup NILAI_USE_MAX323
 * @{
 * @brief If defined, enables the <a
 * href=https://datasheets.maximintegrated.com/en/ds/MAX323-MAX325.pdf>MAX323</a> (Dual SPST
 * analog switch) module.
 */
//#define NILAI_USE_MAX323
//!@}

/**
 * @addtogroup NILAI_USE_PCA9505
 * @{
 * @brief If defined, enables the <a
 * href=https://www.nxp.com/docs/en/data-sheet/PCA9505_9506.pdf>PCA9505</a> (40-bit I2C I/O
 * extender) module.
 */
//#define NILAI_USE_PCA9505
//!@}

/**
 * @addtogroup NILAI_USE_AT24QT2120
 * @{
 * @brief If defined, enables the relay module.
 */
//#define NILAI_USE_RELAY
//!@}

/**
 * @addtogroup NILAI_USE_RN2903
 * @{
 * @brief If defined, enables the <a
 * href=http://ww1.microchip.com/downloads/en/DeviceDoc/50002390E.pdf>RN2903</a> (LoRa
 * Transceiver) module.
 */
//#define NILAI_USE_RN2903
//!@}

/**
 * @addtogroup NILAI_USE_TLP3545
 * @{
 * @brief If defined, enables the <a
 * href=https://toshiba.semicon-storage.com/eu/semiconductor/product/isolators-solid-state-relays/photorelay-mosfet-output/detail.TLP3545.html>TLP3545</a>
 * (Photorelay) module.
 */
//#define NILAI_USE_TLP3545
//!@}

/**
 * @addtogroup NILAI_USE_TAS5707
 * @{
 * @brief If defined, enables the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5707.pdf?ts=1660064616030&ref_url=https%253A%252F%252Fwww.google.com%252F>TAS5707</a>
 * (20-W stereo digital audio power amplifier) module.
 */
//#define NILAI_USE_TAS5707
//!@}

/**
 * @addtogroup NILAI_USE_HW_TAS5760
 * @{
 * @brief If defined, enables the <a
 * href=https://www.ti.com/document-viewer/TAS5760MD/datasheet>TAS5760</a> (General purpose I2S
 * Class-D audio amplifier) module in hardware mode.
 */
//#define NILAI_USE_HW_TAS5760
//!@}

/**
 * @addtogroup NILAI_USE_SW_TAS5760
 * @{
 * @brief If defined, enables the <a
 * href=https://www.ti.com/document-viewer/TAS5760MD/datasheet>TAS5760</a> (General purpose I2S
 * Class-D audio amplifier) module in software mode.
 */
//#define NILAI_USE_SW_TAS5760
//!@}
//!@}

/**
 * @addtogroup nilai_services_selection Services
 * @{
 * @brief Miscellaneous modules offered by Nilai.
 */

/**
 * @addtogroup NILAI_USE_UMO
 * @{
 * @brief If defined, enables the Universe Management module.
 */
//#define NILAI_USE_UMO
//!@}

/**
 * @addtogroup NILAI_USE_SYSTEM
 * @{
 * @brief If defined, enables the System module.
 */
//#define NILAI_USE_SYSTEM
//!@}

/**
 * @addtogroup NILAI_USE_LOGGER
 * @{
 * @brief If defined, enables the Logging module.
 */
//#define NILAI_USE_LOGGER
//!@}

/**
 * @addtogroup NILAI_USE_FILE_LOGGER
 * @{
 * @brief If defined, enables the file logging module.
 */
//#define NILAI_USE_FILE_LOGGER
//!@}

/**
 * @addtogroup NILAI_USE_FILESYSTEM
 * @{
 * @brief If defined, enables the filesystem module.
 *
 * @attention FATFS must be included in the project to use the filesystem module
 */
//#define NILAI_USE_FILESYSTEM
//!@}

/**
 * @addtogroup NILAI_USE_INI_PARSER
 * @{
 * @brief If defined, enables the Ini file parsing module.
 */
//#define NILAI_USE_INI_PARSER
//!@}
//!@}
//!@}

/**
 * @addtogroup nilai_module_specific_settings Module Specific Settings
 * @{
 * @brief Per-module configuration options.
 */

/**
 * @addtogroup tas5760_options TAS5760 Settings
 * @{
 * @brief Options for the TAS5760 module.
 *
 * @note These are only effective if @ref NILAI_USE_SW_TAS5760 or @ref NILAI_USE_HW_TAS5760 is
 * defined.
 */
#    if defined(NILAI_USE_SW_TAS5760) || defined(NILAI_USE_HW_TAS5760)
//--- TAS5760.

/*
 * Controls if the PWM outputs should be disabled (none), in mono (just one) or in stereo (two
 * outputs).
 */

/**
 * @addtogroup NILAI_TAS5760_PWM_NONE
 * @{
 * @brief Disables the PWM outputs of the TAS5760.
 */
#        define NILAI_TAS5760_PWM_NONE 0
//!@}

/**
 * @addtogroup NILAI_TAS5760_PWM_MONO
 * @{
 * @brief Puts the TAS5760 into Mono mode (only 1 PWM output enabled).
 */
#        define NILAI_TAS5760_PWM_MONO 1
//!@}

/**
 * @addtogroup NILAI_TAS5760_PWM_STEREO
 * @{
 * @brief Puts the TAS5760 into Stereo mode (both PWM output enabled).
 */
#        define NILAI_TAS5760_PWM_STEREO 2
//!@}

/**
 * @addtogroup NILAI_TAS5760_PWM_MODE
 * @{
 * @brief The PWM mode that must be used by the TAS5760.
 *
 * Define it using one of the following values, based on your use case:
 * - @ref NILAI_TAS5760_PWM_NONE (default)
 * - @ref NILAI_TAS5760_PWM_MONO
 * - @ref NILAI_TAS5760_PWM_STEREO
 */
#        define NILAI_TAS5760_PWM_MODE NILAI_TAS5760_PWM_NONE
//!@}

/**
 * @addtogroup NILAI_TAS5760_VERIFY_WRITE
 * @{
 * @brief If defined, verifies every I2C transactions done with the TAS5760.
 */
#        define NILAI_TAS5760_VERIFY_WRITE
//!@}
//!@}
#    endif


#    if defined(NILAI_USE_RTC)
/**
 * @addtogroup nilai_rtc_settings RTC Settings
 * @{
 * @brief Options for the Real-Time Clock module.
 */
/**
 * @addtogroup NILAI_RTC_USE_STL
 * @{
 * @brief If defined, enables the use of the standard library to convert epoch time to timestamps
 * and
 * vice-versa.
 *
 * @note This adds significant bloat to the binary file (mktime taking 820 bytes just by itself).
 */
//#define NILAI_RTC_USE_STL
//!@}
//!@}
#    endif

#    if defined(NILAI_USE_FILESYSTEM)
/**
 * @addtogroup nilai_filesystem_options Filesystem Settings
 * @{
 * @brief Options for the filesystem module.
 */

/**
 * @addtogroup NILAI_FS_STATUS_STRING
 * @{
 * @brief If defined, enables the interpretation of error codes.
 */
//#define NILAI_FS_STATUS_STRING
//!@}
//!@}
#    endif

#    if defined(NILAI_USE_ADC)
/**
 * @addtogroup nilai_adc_options ADC Settings
 * @{
 * @brief Options for the ADC module.
 */

/**
 * @addtogroup NILAI_ADC_STATUS_STRING
 * @{
 * @brief If defined, enables the interpretation of error codes for the ADC.
 */
#        define NILAI_ADC_STATUS_STRING
//!@}
//!@}
#    endif
//!@}
/* END OF FILE */
#endif /* NILAI_NILAITFOCONFIG_H */
/**
 * @}
 */
/**
 * @}
 */
