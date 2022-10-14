add_subdirectory(uart)

set(NILAI_TEST_NAME nilai_drivers_test)

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES $<TARGET_PROPERTY:nilai_uart_test,SOURCES>
            )
endif ()
