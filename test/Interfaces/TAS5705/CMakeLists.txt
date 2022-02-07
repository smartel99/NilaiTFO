add_compile_definitions(NILAI_USE_TAS5705)

add_executable(
        TAS5705_test
        EnumTests.cpp
)

target_link_libraries(
        TAS5705_test
        gtest_main
)

gtest_discover_tests(TAS5705_test)