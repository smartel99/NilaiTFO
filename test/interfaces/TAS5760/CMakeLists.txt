add_compile_definitions(NILAI_USE_SW_TAS5760)

add_executable(
        TAS5760_test
        struct_tests.cpp
)

target_link_libraries(
        TAS5760_test
        gtest_main
)

gtest_discover_tests(TAS5760_test)