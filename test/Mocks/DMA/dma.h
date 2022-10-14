#ifndef NILAI_TEST_MOCK_DMA_H
#define NILAI_TEST_MOCK_DMA_H
#include <cstddef>
#include <cstdint>

struct DMA_HandleTypeDef
{
    size_t  id;
    int32_t counter;
    size_t  capacity;
};

#define __HAL_DMA_GET_COUNTER(handle) ::Nilai::Test::Internal::DmaGetCounter((handle))

namespace Nilai::Test::Internal
{
void     DmaSetCapacity(DMA_HandleTypeDef*, size_t);
uint32_t DmaGetCounter(DMA_HandleTypeDef*);
void     DmaSetCounter(DMA_HandleTypeDef*, size_t);
void     DmaDecCounter(DMA_HandleTypeDef*, size_t = 1);
}    // namespace Nilai::Test::Internal
#endif
