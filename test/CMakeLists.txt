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

if ("${NILAI_MCU}" STREQUAL "L452")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Mocks/STM32L452)
elseif ("${NILAI_MCU}" STREQUAL "F405")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Mocks/STM32F405)
else ()
    message(FATAL_ERROR "A MCU must be specified!")
endif ()

add_compile_options(-DNILAI_TEST)

add_subdirectory(bit_manipulations)
add_subdirectory(pin)
# add_subdirectory(processes)
# add_subdirectory(interfaces/TAS5707)
# add_subdirectory(interfaces/TAS5760)
add_subdirectory(drivers)
add_subdirectory(services)
add_subdirectory(circular_buffer)
