set(NILAI_TEST_SOURCES
        test.cpp
        )

set(NILAI_TEST_NAME nilai_circular_buffer_test)

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES ${NILAI_TEST_SOURCES}
            )
else ()
    add_executable(${NILAI_TEST_NAME}
            ${NILAI_TEST_SOURCES}
            )

    target_link_libraries(
            ${NILAI_TEST_NAME}
            gtest_main
    )

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set_target_properties(${NILAI_TEST_NAME}
                PROPERTIES SUFFIX .exe)
        gtest_discover_tests(${NILAI_TEST_NAME})
    else ()
        gtest_discover_tests(${NILAI_TEST_NAME})
    endif ()
endif ()
