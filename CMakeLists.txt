cmake_minimum_required(VERSION 3.14)
project(NilaiTFO C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (NILAI_TEST)
    message(STATUS "Building Nilai in Test Mode")
    add_definitions(-DNILAI_TEST)
    add_subdirectory(test)
else ()
    message(STATUS "Building Nilai in Normal Mode")
endif ()

#------------------------------------------------------------------------------
# Options
if (NOT DEFINED NILAI_MCU)
    set(NILAI_MCU "" CACHE STRING "MCU to build for")
    set_property(CACHE NILAI_MCU PROPERTY STRINGS L452 F405)
endif ()

if (NILAI_MCU STREQUAL "")
    message(STATUS "${NILAI_MCU}")
    message(FATAL_ERROR "NILAI_MCU not defined!")
else ()
    message(STATUS "Building Nilai for ${NILAI_MCU}")
    add_definitions(-D${NILAI_MCU})
endif ()

if (NOT DEFINED NILAI_DRIVER_DIR)
    set(NILAI_DRIVER_DIR "" CACHE STRING "Path to the HAL and CMSIS drivers")
endif ()

if (NILAI_DRIVER_DIR STREQUAL "")
else ()
    include_directories(${NILAI_DRIVER_DIR})
endif ()

#set(NILAI_DRIVER_SRC "" CACHE STRING "Driver source files")
#file(GLOB_RECURSE DRIVER_SOURCES ${NILAI_DRIVER_DIR})

if (NOT DEFINED NILAI_CONFIG_FILE)
    set(NILAI_CONFIG_FILE "" CACHE STRING "Path of the configuration file of NilaiTFO")
endif ()

if (NILAI_CONFIG_FILE STREQUAL "")
    message(FATAL_ERROR "NILAI_CONFIG_FILE not defined!")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${NILAI_CONFIG_FILE}")
endif ()

#------------------------------------------------------------------------------
if (NOT NILAI_TEST)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    set(CMAKE_AR arm-none-eabi-ar)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP arm-none-eabi-objdump)
    set(SIZE arm-none-eabi-size)

    add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
    add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
    add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

    add_compile_options(-mcpu=cortex-m4 -mthumb)
    add_compile_options(--specs=nano.specs)

    # Enable assembler files preprocessing
    add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

    add_link_options(-Wl,-gc-sections,--print-memory-usage)
    add_link_options(-mcpu=cortex-m4 -mthumb)
    add_link_options(-nolibc -Bsymbolic --specs=nano.specs -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group -Wl,-nmagic)
endif ()


# Gets the absolute path and puts it in a variable called `ROOT_DIR`
#get_filename_component(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${ROOT_DIR})
message(STATUS "Nilai's ROOT_DIR: ${ROOT_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${ROOT_DIR}/defines/compiler_defines.h")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif ()

# Disable warnings for C code.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
# Enable desired C++ warnings.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum")
# Disable warnings that are known and understood in C++.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register -Wno-volatile")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Enable useful error messages with concepts.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts-diagnostics-depth=2")
endif ()
# To reduce binary file output size.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fnothrow-opt -fno-enforce-eh-specs -fstrict-enums -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti")
add_compile_options(--no-exceptions -fno-exceptions)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

file(GLOB_RECURSE SOURCES
        "${ROOT_DIR}/*.*"
        )

list(FILTER SOURCES EXCLUDE REGEX "${ROOT_DIR}/test")
list(FILTER SOURCES EXCLUDE REGEX "${ROOT_DIR}/example")
list(FILTER SOURCES EXCLUDE REGEX "${ROOT_DIR}/vendor")
set(SOURCES ${SOURCES} "${ROOT_DIR}/vendor/inih/ini.c")

if (NILAI_TEST)
    include_directories(${ROOT_DIR}/test/Mocks)

    file(GLOB_RECURSE MOCK_SOURCES "${ROOT_DIR}/test/Mocks/*.*")

    if ("${NILAI_MCU}" STREQUAL "L452")
        list(FILTER MOCK_SOURCES EXCLUDE REGEX "${ROOT_DIR}/test/Mocks/STM32F405")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test/Mocks/STM32L452)
    elseif ("${NILAI_MCU}" STREQUAL "F405")
        list(FILTER MOCK_SOURCES EXCLUDE REGEX "${ROOT_DIR}/test/Mocks/STM32L452")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test/Mocks/STM32F405)
    else ()
        message(FATAL_ERROR "A MCU must be specified!")
    endif ()

    set(SOURCES ${SOURCES} ${MOCK_SOURCES})
endif ()

add_library(NilaiTFO OBJECT
        ${SOURCES}
        )
