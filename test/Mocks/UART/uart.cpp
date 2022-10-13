#include "uart.h"
#include <algorithm>
#include <utility>

size_t                      s_uart_buffers_id_count;
std::map<size_t, IoBuffer*> s_uart_buffers;

void Nilai_UART_Init(UART_HandleTypeDef* handle, size_t txl, size_t rxl)
{
    using T = typename decltype(std::declval<IoBuffer>().rx)::value_type;
    s_uart_buffers[s_uart_buffers_id_count] =
      new IoBuffer {{static_cast<T>(txl)}, {static_cast<T>(rxl)}};
    handle->id     = s_uart_buffers_id_count++;
    handle->hdmarx = new DMA_HandleTypeDef;
    __NILAI_DMA_SET_CAPACITY(handle->hdmarx, rxl);
}

HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef* handle)
{
    delete s_uart_buffers[handle->id];
    s_uart_buffers[handle->id] = nullptr;

    delete handle->hdmarx;
    handle->hdmarx = nullptr;

    s_uart_buffers.erase(handle->id);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef* handle, uint8_t* buff, size_t len)
{
    s_uart_buffers[handle->id]->rx = decltype(s_uart_buffers[handle->id]->rx)(len);
    
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef* handle, uint8_t* buff, size_t len)
{
    s_uart_buffers[handle->id]->tx.Push(buff, len);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef*)
{
    return HAL_OK;
}

void Nilai_UART_Inject_DMA(UART_HandleTypeDef* handle, const uint8_t* buff, size_t len)
{
    size_t pushed = s_uart_buffers[handle->id]->rx.Push(buff, len);
    s_uart_buffers[handle->id]->rx.Pop(pushed);
    __NILAI_DMA_DEC_COUNTER(handle->hdmarx, len);
}

void Nilai_UART_Inject_DMA(UART_HandleTypeDef* handle, const std::string& msg)
{
    Nilai_UART_Inject_DMA(handle, reinterpret_cast<const uint8_t*>(msg.data()), msg.length());
}

void Nilai_UART_Inject_DMA(UART_HandleTypeDef* handle, const std::vector<uint8_t>& buff)
{
    Nilai_UART_Inject_DMA(handle, buff.data(), buff.size());
}

IoBuffer* Nilai_UART_Get_Buffer(UART_HandleTypeDef* handle)
{
    return s_uart_buffers[handle->id];
}
