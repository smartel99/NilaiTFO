add_subdirectory(events)

set(NILAI_TEST_NAME nilai_application_test)

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES
            $<TARGET_PROPERTY:nilai_events_test,SOURCES>
            )
endif ()
