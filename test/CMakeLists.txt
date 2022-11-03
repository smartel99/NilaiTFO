cmake_minimum_required(VERSION 3.14)
project(NilaiTFO_Test)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_compile_definitions(GTEST)

include(GoogleTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -pedantic)
add_compile_options(-fconcepts-diagnostics-depth=3)

#get_filename_component(ROOT_DIR ${CMAKE_SOURCE_DIR}/.. ABSOLUTE)
#get_filename_component(SERVICE_DIR ${ROOT_DIR}/services ABSOLUTE)
#get_filename_component(PROCESS_DIR ${ROOT_DIR}/processes ABSOLUTE)
#get_filename_component(INTERFACE_DIR ${ROOT_DIR}/interfaces ABSOLUTE)
set(NILAI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${NILAI_DIR})
message(STATUS "Nilai Tests's NILAI_DIR: ${NILAI_DIR}")

# Added for the intellisense.
include_directories(${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Mocks/fatfs)

if ("${NILAI_MCU}" STREQUAL "L452")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Mocks/STM32L452)
    add_definitions(-DNILAI_USES_STM32L4xx)
elseif ("${NILAI_MCU}" STREQUAL "F405")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Mocks/STM32F405)
    add_definitions(-DNILAI_USES_STM32F4xx)
else ()
    message(FATAL_ERROR "A MCU must be specified!")
endif ()

add_compile_options(-DNILAI_TEST)

option(NILAI_TEST_ALL "Enable all tests" OFF)
if (NILAI_TEST_ALL)
    set(NILAI_TEST_BIT_MANIPULATION ON CACHE BOOL "Enable testing for bit manipulation" FORCE)
    set(NILAI_TEST_PIN ON CACHE BOOL "Enable testing for pins" FORCE)
    set(NILAI_TEST_CIRCULAR_BUFFER ON CACHE BOOL "Enable testing for circular buffer" FORCE)
    set(NILAI_TEST_SWAP_BUFFER ON CACHE BOOL "Enable testing for swap buffer" FORCE)

    set(NILAI_TEST_DRIVERS ON CACHE BOOL "Enable testing for drivers" FORCE)
    set(NILAI_TEST_ALL_DRIVERS ON CACHE BOOL "Enable testing for all drivers" FORCE)

    set(NILAI_TEST_INTERFACES ON CACHE BOOL "Enable testing for interfaces" FORCE)
    set(NILAI_TEST_ALL_INTERFACES ON CACHE BOOL "Enable testing for all interfaces" FORCE)

    set(NILAI_TEST_SERVICES ON CACHE BOOL "Enable testing for services" FORCE)
    set(NILAI_TEST_ALL_SERVICES ON CACHE BOOL "Enable testing for all services" FORCE)

    set(NILAI_TEST_PROCESSES ON CACHE BOOL "Enable testing for processes" FORCE)
    set(NILAI_TEST_ALL_PROCESSES ON CACHE BOOL "Enable testing for all processes" FORCE)
endif ()

set(NILAI_TEST_SOURCES)

option(NILAI_TEST_BIT_MANIPULATION "Enable testing for bit manipulation" OFF)
if (NILAI_TEST_BIT_MANIPULATION)
    add_subdirectory(bit_manipulations)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_bit_manipulation_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_PIN "Enable testing for pins" OFF)
if (NILAI_TEST_PIN)
    add_subdirectory(pin)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_pin_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_CIRCULAR_BUFFER "Enable testing for circular buffer" OFF)
if (NILAI_TEST_CIRCULAR_BUFFER)
    add_subdirectory(circular_buffer)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_circular_buffer_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_SWAP_BUFFER "Enable testing for swap buffer" OFF)
if (NILAI_TEST_SWAP_BUFFER)
    add_subdirectory(swap_buffer)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_swap_buffer_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_DRIVERS "Enable testing for drivers" OFF)
if (NILAI_TEST_DRIVERS)
    add_subdirectory(drivers)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_drivers_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_INTERFACES "Enable testing for interfaces" OFF)
if (NILAI_TEST_INTERFACES)
    add_subdirectory(interfaces)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_interfaces_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_SERVICES "Enable testing for services" OFF)
if (NILAI_TEST_SERVICES)
    add_subdirectory(services)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_services_test,SOURCES>)
    endif ()
endif ()

option(NILAI_TEST_PROCESSES "Enable testing for processes" OFF)
if (NILAI_TEST_PROCESSES)
    add_subdirectory(processes)
    if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
        set(NILAI_TEST_SOURCES ${NILAI_TEST_SOURCES} $<TARGET_PROPERTY:nilai_processes_test,SOURCES>)
    endif ()
endif ()

if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
    set(NILAI_OUTPUT_FILENAME NilaiTFO_Test)

    if (NILAI_MAKE_TEST_AS_LIB STREQUAL "true")
        message(STATUS "Making NilaiTFO_Test as a library")
        add_library(
                ${NILAI_OUTPUT_FILENAME} OBJECT
                ${NILAI_TEST_SOURCES}
        )

    else ()
        message(STATUS "Making NilaiTFO_Test as an executable")
        add_executable(
                ${NILAI_OUTPUT_FILENAME}
                ${NILAI_TEST_SOURCES}
        )

        if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
            set_target_properties(${NILAI_OUTPUT_FILENAME}
                    PROPERTIES SUFFIX .exe)
            gtest_discover_tests(${NILAI_OUTPUT_FILENAME})
        else ()
            gtest_discover_tests(${NILAI_OUTPUT_FILENAME})
        endif ()
    endif ()

    target_link_libraries(
            ${NILAI_OUTPUT_FILENAME}
            gtest_main
    )
    #    gtest_discover_tests(${NILAI_OUTPUT_FILENAME})
endif ()
