# Bit manipulation
add_executable(
        BitManipulation_test
        swap.cpp
        combine.cpp
        arraynificator.cpp
)

target_link_libraries(
        BitManipulation_test
        gtest_main
)

gtest_discover_tests(BitManipulation_test)