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

add_subdirectory(bit_manipulations)
add_subdirectory(pin)
# add_subdirectory(processes)
# add_subdirectory(interfaces)
add_subdirectory(drivers)
add_subdirectory(services)
add_subdirectory(circular_buffer)


if (NILAI_SINGLE_TEST_EXE STREQUAL "true")
    set(NILAI_OUTPUT_FILENAME NilaiTFO_Test)

    set(NILAI_TEST_SOURCES
            $<TARGET_PROPERTY:nilai_drivers_test,SOURCES>
            #            $<TARGET_PROPERTY:nilai_interfaces_test,SOURCES>
            #            $<TARGET_PROPERTY:nilai_processes_test,SOURCES>
            $<TARGET_PROPERTY:nilai_bit_manipulation_test,SOURCES>
            $<TARGET_PROPERTY:nilai_pin_test,SOURCES>
            $<TARGET_PROPERTY:nilai_services_test,SOURCES>
            $<TARGET_PROPERTY:nilai_circular_buffer_test,SOURCES>
            )

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
