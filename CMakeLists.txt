cmake_minimum_required(VERSION 3.11)
# Only make this a project if it's not part of one.
if("${CMAKE_PROJECT_NAME}" STREQUAL "")
    project(NilaiTFO 
        LANGUAGES C CXX
        HOMEPAGE_URL https://github.com/smartel99/NilaiTFO)
    set(CMAKE_CXX_STANDARD 17)
else()
    if("${CMAKE_CXX_STANDARD}" LESS 17)
        message(FATAL_ERROR "Minimum supported C++ version is C++17")
    endif()

    if("${CMAKE_C_STANDARD}" LESS 11)
        message(FATAL_ERROR "Minimum supported C version is C11")
    endif()
endif()

#------------------------------------------------------------------------------
# Options

set(NILAI_APPLICATION_HEADER_PATH "" CACHE PATH "Path to the application header, i.e. where cep::Application is inherited")

set(NILAI_DRIVERS_PATH "" CACHE PATH "Path to a file including the drivers to be used by Nilai")

# Various MCUs supported by Nilai.
set(NILAI_MCU MOCK CACHE STRING "Selects the microcontroller to use")
set_property(CACHE NILAI_MCU PROPERTY STRINGS 
    MOCK
    STM32F4XX
    STM32F7XX
    STM32L4XX
    GD32F4XX)

option(NILAI_MOCK_RANDOM_DATA "If enabled and if NILAI_MCU is set to MOCK, the mocks will generate random data. If disabled, the mocks will always return fixed data" OFF)

option(NILAI_WARN_MISSING_DRIVERS "Warn if a module is included but the corresponding drivers are missing" ON)

option(NILAI_LOGGER_USE_RTC "Choose the timebase used by the logger. If true, the real-time clock will be used. Otherwise, the system's timebase will be used. The real-time clock must be enabled to be able to be used by the logger." OFF)

# Log levels.
option(NILAI_LOG_ENABLE_DEBUG "Enables debug-level logging messages" ON)
option(NILAI_LOG_ENABLE_INFO "Enables info-level logging messages" ON)
option(NILAI_LOG_ENABLE_WARNING "Enables warning-level logging messages" ON)
option(NILAI_LOG_ENABLE_ERROR "Enables error-level logging messages" ON)
option(NILAI_LOG_ENABLE_CRITICAL "Enables critical-level logging messages" ON)

set(NILAI_UMO_PROTOCOL UART CACHE STRING "Selects the hardware protocol used by the Umo module")
set_property(CACHE NILAI_UMO_PROTOCOL PROPERTY STRINGS
    UART
    CAN)

# Modules - Drivers
option(NILAI_USE_ADC "Enables the ADC driver" OFF)
option(NILAI_USE_CAN "Enables the CAN driver" OFF)
option(NILAI_USE_I2C "Enables the I2C driver" OFF)
option(NILAI_USE_SPI "Enables the SPI driver" OFF)
option(NILAI_USE_UART "Enables the UART driver" OFF)
option(NILAI_USE_PWM "Enables the PWM driver" OFF)
option(NILAI_USE_RTC "Enables the RTC driver" OFF)

# Modules - Interfaces
option(NILAI_USE_ADS "Enables the ADS131 interface" OFF)
option(NILAI_USE_DS2484 "Enables the DS2484 interface" OFF)
option(NILAI_USE_ESP32 "Enables the ESP32 interface" OFF)
option(NILAI_USE_HEARTBEAT "Enables the Heartbeat interface" OFF)
option(NILAI_USE_LTC2498 "Enables the LTC2498 interface" OFF)
option(NILAI_USE_MAX14763 "Enables the MAX14763 interface" OFF)
option(NILAI_USE_MAX14778 "Enables the MAX14778 interface" OFF)
option(NILAI_USE_MAX323 "Enables the MAX323 interface" OFF)
option(NILAI_USE_PCA9505 "Enables the PCA9505 interface" OFF)
option(NILAI_USE_RELAY "Enables the Relay interface" OFF)
option(NILAI_USE_RN2903 "Enables the RN2903 interface" OFF)
option(NILAI_USE_TLP3545 "Enables the TLP3545 interface" OFF)

# Modules - Services
option(NILAI_USE_UMO "Enables the Umo service" OFF)
option(NILAI_USE_SYSTEM "Enables the System service" OFF)
option(NILAI_USE_LOGGER "Enables the Logger service" OFF)
option(NILAI_USE_FILE_LOGGER "Enables the File Logger service" OFF)
option(NILAI_USE_FILESYSTEM "Enables the Filesystem service (requires FATFS)" OFF)

#------------------------------------------------------------------------------
# Option verification
if("${NILAI_APPLICATION_HEADER_PATH}" STREQUAL "")
    message(FATAL_ERROR "NILAI_APPLICATION_HEADER_PATH was not defined")
elseif(NOT EXISTS "${NILAI_APPLICATION_HEADER_PATH}")
    message(FATAL_ERROR "Unable to find application header at '${NILAI_APPLICATION_HEADER_PATH}'")
endif()

if("${NILAI_DRIVERS_PATH}" STREQUAL "")
    message(FATAL_ERROR "NILAI_DRIVERS_PATH was not defined")
elseif(NOT EXISTS "${NILAI_DRIVERS_PATH}")
    message(FATAL_ERROR "Unable to find application header at '${NILAI_DRIVERS_PATH}'")
endif()

if((NOT "${NILAI_MCU}" STREQUAL "MOCK") AND ("${NILAI_MOCK_RANDOM_DATA}" STREQUAL "ON"))
    message(WARNING "NILAI_MOCK_RANDOM_DATA is enabled, but MCU type is not MOCK")
endif()


# Disable -Wregister, which warns of the use of the register keyword in C++17 and later.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")
