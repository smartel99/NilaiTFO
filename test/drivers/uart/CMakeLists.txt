include_directories(${NILAI_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
        Uart_test
        test.cpp
        ${NILAI_DIR}/drivers/uart_module.cpp
        ${NILAI_DIR}/test/Mocks/UART/uart.cpp
        ${NILAI_DIR}/test/Mocks/DMA/dma.cpp
)

target_link_libraries(
        Uart_test
        gtest_main
)

gtest_discover_tests(Uart_test)
