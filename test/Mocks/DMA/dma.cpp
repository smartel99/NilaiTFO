#include "dma.h"

void __NILAI_DMA_SET_CAPACITY(DMA_HandleTypeDef* handle, size_t capacity) {
    handle->capacity = capacity;
    handle->counter = capacity;
}

uint32_t __HAL_DMA_GET_COUNTER(DMA_HandleTypeDef* handle) {
    return handle->counter;
}

void __NILAI_DMA_SET_COUNTER(DMA_HandleTypeDef* handle, size_t count) {
    handle->counter = count;
}

void __NILAI_DMA_DEC_COUNTER(DMA_HandleTypeDef* handle, size_t count) {
    handle->counter -= count;
    while(handle->counter <= 0) {
        handle->counter += handle->capacity;
    }
}