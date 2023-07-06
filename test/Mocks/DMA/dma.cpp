#include "dma.h"

namespace Nilai::Test::Internal
{
void DmaSetCapacity(DMA_HandleTypeDef* handle, size_t capacity)
{
    handle->capacity = capacity;
    handle->counter  = static_cast<int>(capacity);
}

uint32_t DmaGetCounter(DMA_HandleTypeDef* handle)
{
    return handle->counter;
}

void DmaSetCounter(DMA_HandleTypeDef* handle, size_t count)
{
    handle->counter = static_cast<int>(count);
}

void DmaDecCounter(DMA_HandleTypeDef* handle, size_t count)
{
    handle->counter -= static_cast<int>(count);
    while (handle->counter <= 0)
    {
        handle->counter += static_cast<int>(handle->capacity);
    }
}
}    // namespace Nilai::Test::Internal
