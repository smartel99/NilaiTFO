#pragma once
#include "defines/circular_buffer.h"

struct IoBuffer {
    Nilai::CircularBuffer<uint8_t> tx; 
    Nilai::CircularBuffer<uint8_t> rx; 
};
