add_compile_definitions(NILAI_USE_TAS5707)

add_executable(
        TAS5707_test
        enum_tests.cpp
)

target_link_libraries(
        TAS5707_test
        gtest_main
)

gtest_discover_tests(TAS5707_test)