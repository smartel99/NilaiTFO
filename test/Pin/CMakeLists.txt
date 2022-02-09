# Pin

add_compile_definitions(NILAI_USE_STM32F4xx)

# The executable that will do the tests.
add_executable(
        Pin_test
        ../../defines/pin.cpp
        ../Mocks/GPIO/gpio.cpp
        Test.cpp
)

target_link_libraries(
        Pin_test
        gtest_main
)

gtest_discover_tests(Pin_test)