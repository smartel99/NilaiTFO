#ifndef NILAI_TEST_MOCK_UART_H
#define NILAI_TEST_MOCK_UART_H

#include "../DMA/dma.h"

#include "../generic_stm32.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

using HAL_UART_StateTypeDef = uint32_t;
using UartIoBuffer          = IoBuffer<uint8_t>;


#define HAL_UART_STATE_READY                                                                       \
    0x00000020U /*!< Peripheral Initialized and ready for use Value is allowed for gState and      \
                   RxState */

struct UART_HandleTypeDef
{
    size_t                id;
    HAL_UART_StateTypeDef gState;
    DMA_HandleTypeDef*    hdmarx;

    uint8_t* data;
    size_t len;
};

extern std::map<size_t, std::unique_ptr<UartIoBuffer>> s_uart_buffers;

HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef*);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef*, uint8_t*, size_t);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef*, uint8_t*, size_t);

void              Nilai_UART_Init(UART_HandleTypeDef*, size_t = 512, size_t = 512);
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef*);
void              Nilai_UART_Inject_DMA(UART_HandleTypeDef*, const uint8_t*, size_t);
void              Nilai_UART_Inject_DMA(UART_HandleTypeDef*, const std::string&);
void              Nilai_UART_Inject_DMA(UART_HandleTypeDef*, const std::vector<uint8_t>&);
UartIoBuffer&     Nilai_UART_Get_Buffer(UART_HandleTypeDef*);
#endif
