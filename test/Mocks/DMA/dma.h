#pragma once
#include <cstdint>
#include <cstddef>

struct DMA_HandleTypeDef {
    size_t id;
    int32_t counter;
    size_t capacity;
};

void __NILAI_DMA_SET_CAPACITY(DMA_HandleTypeDef*, size_t);
uint32_t __HAL_DMA_GET_COUNTER(DMA_HandleTypeDef*);
void __NILAI_DMA_SET_COUNTER(DMA_HandleTypeDef*, size_t);
void __NILAI_DMA_DEC_COUNTER(DMA_HandleTypeDef*, size_t = 1);