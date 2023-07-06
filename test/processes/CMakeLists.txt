
add_subdirectory(application)

set(NILAI_TEST_NAME nilai_processes_test)
message(STATUS "Building ${NILAI_TEST_NAME}")

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME} SOURCES
            $<TARGET_PROPERTY:nilai_application_test,SOURCES>
            )

    target_link_libraries(
            ${NILAI_TEST_NAME}
            gtest_main
    )
endif ()
