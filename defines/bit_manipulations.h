/**
 * @file    bit_manipulations.h
 * @author  Pascal-Emmanuel Lachance
 * @date    2019/11/24  -  20:14
 *
 * @brief   Header file for small miscelleanous bit manipulation functions and
 *          features.
 *          Includes the following inline functions:
 *          -   swap, swaps the first and last half of an integer type.
 *          -   combine, combines two values into a greater value by
 *              splicing the values together.
 *          -   arraynificator, takes a big number and splits it into an array
 *              of bytes.
 *
 * @version 2020/07/02 - Switch to C++17
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "defines/macros.h"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Nilai
{

/*************************************************************************************************/
/* Function declarations ----------------------------------------------------------------------- */
template<typename T>
constexpr T swap(T input);

static uint64_t combine(const uint8_t* array, uint8_t length);

static constexpr void arraynificator(void* src, size_t len, void* dest);

/*************************************************************************************************/
/* Bit definitions ----------------------------------------------------------------------------- */
#define NILAI_BIT0   0x01U
#define NILAI_BIT1   0x02U
#define NILAI_BIT2   0x04U
#define NILAI_BIT3   0x08U
#define NILAI_BIT4   0x10U
#define NILAI_BIT5   0x20U
#define NILAI_BIT6   0x40U
#define NILAI_BIT7   0x80U
#define NILAI_BIT8   0x0100U
#define NILAI_BIT9   0x0200U
#define NILAI_BIT10  0x0400U
#define NILAI_BIT11  0x0800U
#define NILAI_BIT12  0x1000U
#define NILAI_BIT13  0x2000U
#define NILAI_BIT14  0x4000U
#define NILAI_BIT15  0x8000U
#define NILAI_BIT(x) (1 << (x))

#define BIT0(x)   ((x)&NILAI_BIT0)
#define BIT1(x)   ((x)&NILAI_BIT1)
#define BIT2(x)   ((x)&NILAI_BIT2)
#define BIT3(x)   ((x)&NILAI_BIT3)
#define BIT4(x)   ((x)&NILAI_BIT4)
#define BIT5(x)   ((x)&NILAI_BIT5)
#define BIT6(x)   ((x)&NILAI_BIT6)
#define BIT7(x)   ((x)&NILAI_BIT7)
#define BIT8(x)   ((x)&NILAI_BIT8)
#define BIT9(x)   ((x)&NILAI_BIT9)
#define BIT10(x)  ((x)&NILAI_BIT10)
#define BIT11(x)  ((x)&NILAI_BIT11)
#define BIT12(x)  ((x)&NILAI_BIT12)
#define BIT13(x)  ((x)&NILAI_BIT13)
#define BIT14(x)  ((x)&NILAI_BIT14)
#define BIT15(x)  ((x)&NILAI_BIT15)
#define BIT(y, x) ((x)&NILAI_BIT##(y))

/*************************************************************************************************/
/* Inline functions definitions ---------------------------------------------------------------- */
/**
 * @brief   Swaps the first and last halves of an integral variable.
 * @param   input: the value to swap
 * @retval  value swapped
 */
template<typename T>
constexpr T swap(T input)
{
    static_assert(std::is_integral<T>::value,
                  "value to swap must be an integral type (uint8_t, int, size_t, ect...)");
    static_assert(std::is_unsigned<T>::value, "value to swap must be an unsigned integer.");

    constexpr size_t size = 8 * sizeof(T);

    T output = input << (size / 2);
    output |= input >> (size / 2);

    return output;
}

/**
 * @brief   Combines `length` 8-bits values into a single value, up to 64-bits
 *          in length.
 * @param   array: A pointer to an array of bytes to be combined in a single
 *                 value.
 * @param   length: The number of bytes to combine, up to 8.
 *
 * @retval  uint64_t: combined value
 */
[[maybe_unused]] uint64_t combine(const uint8_t* array, uint8_t length)
{
    if (array == nullptr)
    {
        return 0;
    }
    NILAI_ASSERT(length <= sizeof(uint64_t), "Invalid array length");

    uint64_t output    = 0;
    uint8_t* outputPtr = reinterpret_cast<uint8_t*>(&output);
    for (uint8_t i = 0; i < length; i++)
    {
        outputPtr[length - (i + 1)] = array[i];
    }

    return output;
}

/**
 * @brief   Takes a pointer to a value and its length, and write the bytes
 *          sequentially in a destination array.
 *
 * @param   src:  Pointer to a source value
 * @param   len:  Number of bytes to copy
 * @param   dest: Destination array to copy the values into
 */
constexpr void arraynificator(void* src, size_t len, void* dest)
{
    if (src == nullptr || dest == nullptr)
    {
        return;
    }

    for (size_t i = 0; i < len; i++)
    {
        (static_cast<uint8_t*>(dest))[i] = (static_cast<uint8_t*>(src))[i];
    }
}


/*************************************************************************************************/
}    // namespace Nilai
/* ----- END OF FILE ----- */
