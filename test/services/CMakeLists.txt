add_executable(
        Test
        serializer.cpp
        deserializer.cpp
)

target_link_libraries(
        Test
        gtest_main
)

gtest_discover_tests(Test)
