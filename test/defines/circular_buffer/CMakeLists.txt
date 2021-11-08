include_directories(${ROOT_DIR})

add_executable(
    CircularBuffer_test
    test.cpp
)

target_link_libraries(
    CircularBuffer_test
    gtest_main
)

gtest_discover_tests(CircularBuffer_test)