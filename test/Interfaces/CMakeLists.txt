add_subdirectory(TAS5707)
add_subdirectory(TAS5760)

set(NILAI_TEST_NAME nilai_interfaces_test)

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES
            $<TARGET_PROPERTY:nilai_tas5707_test,SOURCES>
            $<TARGET_PROPERTY:nilai_tas5760_test,SOURCES>
            )
endif ()
