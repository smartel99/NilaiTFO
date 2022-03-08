add_compile_definitions(NILAI_USE_EVENTS)
add_compile_definitions(NILAI_EVENTS_MAX_CALLBACKS=5)

add_executable(
        ApplicationEvents_test
        Tests.cpp
        ${ROOT_DIR}/processes/application.cpp
        ${ROOT_DIR}/defines/Assertion.cpp
)

target_link_libraries(
        ApplicationEvents_test
        gtest_main
)

gtest_discover_tests(ApplicationEvents_test)