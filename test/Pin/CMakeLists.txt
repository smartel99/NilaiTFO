# Pin
# The executable that will do the tests.
add_executable(
        Pin_test
        ../Mocks/GPIO/gpio.cpp
        Test.cpp
)

target_link_libraries(
        Pin_test
        gtest_main
)

gtest_discover_tests(Pin_test)