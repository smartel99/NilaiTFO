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

set(CMAKE_CXX_STANDARD 17)

get_filename_component(ROOT_DIR ${CMAKE_SOURCE_DIR}/.. ABSOLUTE)
#get_filename_component(SERVICE_DIR ${ROOT_DIR}/services ABSOLUTE)
#get_filename_component(PROCESS_DIR ${ROOT_DIR}/processes ABSOLUTE)
#get_filename_component(INTERFACE_DIR ${ROOT_DIR}/interfaces ABSOLUTE)
include_directories(${ROOT_DIR})

add_compile_options(-DNILAI_TEST)

add_subdirectory(bit_manipulations)
add_subdirectory(pin)
add_subdirectory(processes)
add_subdirectory(interfaces/TAS5707)
add_subdirectory(interfaces/TAS5760)


