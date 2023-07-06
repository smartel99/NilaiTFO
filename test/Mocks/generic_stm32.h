/**
 * @file    generic_stm32.h
 * @author  Samuel Martel
 * @date    2022-10-13
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */

#ifndef GUARD_NILAI_TEST_MOCK_GENERIC_STM32_H
#define GUARD_NILAI_TEST_MOCK_GENERIC_STM32_H
#include "defines/circular_buffer.h"

typedef enum
{
    HAL_OK      = 0x00,
    HAL_ERROR   = 0x01,
    HAL_BUSY    = 0x02,
    HAL_TIMEOUT = 0x03
} HAL_StatusTypeDef;


template<typename T, size_t N>
struct IoBuffer
{
    using type                     = T;
    static constexpr size_t s_size = N;
    using value_type               = Nilai::CircularBuffer<type, s_size>;

    value_type tx;
    value_type rx;

    IoBuffer() = default;
};

#endif    // GUARD_NILAI_TEST_MOCK_GENERIC_STM32_H
