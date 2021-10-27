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

include(GoogleTest)
enable_testing()

get_filename_component(ROOT_DIR ${CMAKE_SOURCE_DIR}/.. ABSOLUTE)
#get_filename_component(SERVICE_DIR ${ROOT_DIR}/services ABSOLUTE)
#get_filename_component(PROCESS_DIR ${ROOT_DIR}/processes ABSOLUTE)
#get_filename_component(INTERFACE_DIR ${ROOT_DIR}/interfaces ABSOLUTE)
include_directories(${ROOT_DIR})

add_compile_options(-DNILAI_TEST)

# Pin
# The executable that will do the tests.
add_executable(
        Pin_test
        Mocks/GPIO/gpio.cpp
        Pin/Test.cpp
)

target_link_libraries(
        Pin_test
        gtest_main
)

gtest_discover_tests(Pin_test)


# Bit manipulation
add_executable(
        BitManipulation_test
        BitManipulations/swap.cpp
        BitManipulations/combine.cpp
        BitManipulations/arraynificator.cpp
)

target_link_libraries(
        BitManipulation_test
        gtest_main
)

gtest_discover_tests(BitManipulation_test)