option(NILAI_TEST_ALL_DRIVERS "Enable testing for all drivers" OFF)
if (NILAI_TEST_ALL_DRIVERS)
    set(NILAI_TEST_DRIVER_UART ON CACHE BOOL "Enable testing for the UART driver")
endif ()

option(NILAI_TEST_DRIVER_UART "Enable testing for the UART driver" OFF)
if (NILAI_TEST_DRIVER_UART)
    add_subdirectory(uart)
endif ()

set(NILAI_TEST_NAME nilai_drivers_test)

if (DEFINED NILAI_SINGLE_TEST_EXE)
    add_custom_target(${NILAI_TEST_NAME}
            SOURCES $<TARGET_PROPERTY:nilai_uart_test,SOURCES>
            )
endif ()
