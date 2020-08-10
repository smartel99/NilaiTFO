/**
 * @addtogroup defines
 * @{
 * @addtogroup bitManipulations
 * @{
 * @file    bitManipulations.hpp
 * @author  Pascal-Emmanuel Lachance
 * @p       https://www.github.com/Raesangur
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
#include "defines/_pch.hpp"


namespace cep
{


/*************************************************************************************************/
/* Function declarations ----------------------------------------------------------------------- */
template<typename T>
constexpr T swap(T input);

template<typename T>
constexpr typename cep::width2<T>::type combine(T msb, T lsb);
std::uint64_t                           combine(std::uint8_t* array, std::uint8_t length);

constexpr void arraynificator(void* src, std::size_t len, void* dest);
constexpr void uint16_arraynificator(std::uint16_t val, std::uint8_t output[2]);
constexpr void uint24_arraynificator(std::uint32_t val, std::uint8_t output[3]);
constexpr void uint32_arraynificator(std::uint32_t val, std::uint8_t output[4]);

void  float_arraynificator(float val, std::uint8_t output[4]);
void  double_arraynificator(double val, std::uint8_t output[8]);
float floatinificator(uint8_t input[4]);


/*************************************************************************************************/
/* Bit definitions ----------------------------------------------------------------------------- */
#define _BIT0   0x01U
#define _BIT1   0x02U
#define _BIT2   0x04U
#define _BIT3   0x08U
#define _BIT4   0x10U
#define _BIT5   0x20U
#define _BIT6   0x40U
#define _BIT7   0x80U
#define _BIT8   0x0100U
#define _BIT9   0x0200U
#define _BIT10  0x0400U
#define _BIT11  0x0800U
#define _BIT12  0x1000U
#define _BIT13  0x2000U
#define _BIT14  0x4000U
#define _BIT15  0x8000U
#define _BIT(x) (1 << (x))

#define BIT0(x)   ((x)&_BIT0)
#define BIT1(x)   ((x)&_BIT1)
#define BIT2(x)   ((x)&_BIT2)
#define BIT3(x)   ((x)&_BIT3)
#define BIT4(x)   ((x)&_BIT4)
#define BIT5(x)   ((x)&_BIT5)
#define BIT6(x)   ((x)&_BIT6)
#define BIT7(x)   ((x)&_BIT7)
#define BIT8(x)   ((x)&_BIT8)
#define BIT9(x)   ((x)&_BIT9)
#define BIT10(x)  ((x)&_BIT10)
#define BIT11(x)  ((x)&_BIT11)
#define BIT12(x)  ((x)&_BIT12)
#define BIT13(x)  ((x)&_BIT13)
#define BIT14(x)  ((x)&_BIT14)
#define BIT15(x)  ((x)&_BIT15)
#define BIT(y, x) ((x)&_BIT##(y))


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
    
    constexpr std::size_t size = 8 * sizeof(T);

    T output = input << (size / 2);
    output |= input >> (size / 2);

    return output;
}


/**
 * @brief   Combines two integral values into a single value twice as big
 * @param   msb: The first value, that will be the most significant half of
 *               the returned value;
 * @param   lsb: The second value, that will be the least significant half of
 *               the returned value;
 *
 * @retval  combined value
 */
template<typename T>
constexpr typename cep::width2<T>::type combine(T msb, T lsb)
{
    static_assert(std::is_integral<T>::value,
                  "values to combine must be an integral type (uint8_t, int, size_t, ect...)");
    static_assert(sizeof(T) < sizeof(std::uint64_t), "two 64-bits values cannot be combined");
    
    constexpr std::size_t size = 8 * sizeof(msb);

    typename cep::width2<T>::type output = static_cast<typename cep::width2<T>::type>(msb << size);
    output |= lsb;

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
std::uint64_t combine(std::uint8_t* array, std::uint8_t length)
{
    CEP_ASSERT_NULL(array);
    CEP_ASSERT(length <= sizeof(uint64_t), "Invalid array length");

    std::uint64_t output    = 0;
    std::uint8_t* outputPtr = reinterpret_cast<std::uint8_t*>(&output);
    for (std::uint8_t i = 0; i < length; i++)
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
 * @retval  None
 */
constexpr void arraynificator(void* src, std::size_t len, void* dest)
{
    for (std::size_t i = 0; i < len; i++)
    {
        ((std::uint8_t*)dest)[i] = ((std::uint8_t*)src)[i];
    }
}

/**
 * @brief   Takes a single uint16_t value and splits it into two uint8_t values
 *
 *          Example:
 *              val = 0xAA55
 *              output = { 0xAA, 0x55 }
 *
 * @param   val: The value to split
 * @param   output : The desired output destination
 * @retval  None
 */
constexpr void uint16_arraynificator(std::uint16_t val, std::uint8_t output[2])
{
    output[0] = val >> 8;
    output[1] = (std::uint8_t)val;
}

/**
 * @brief   Takes a single uint32_t value and splits it into three uint8_t values
 *
 *          Example:
 *              val = 0x00223344
 *              output = { 0x22, 0x33, 0x44 }
 *
 * @param   val: The value to split
 * @param   output : The desired output destination
 * @retval  None
 */
constexpr void uint24_arraynificator(std::uint32_t val, std::uint8_t output[3])
{
    output[0] = (std::uint8_t)(val >> 16);
    output[1] = (std::uint8_t)(val >> 8);
    output[2] = (std::uint8_t)(val);
}

/**
 * @brief   Takes a single uint32_t value and splits it into four uint8_t values
 *
 *          Example:
 *              val = 0x11223344
 *              output = { 0x11, 0x22, 0x33, 0x44 }
 *
 * @param   val: The value to split
 * @param   output : The desired output destination
 * @retval  None
 */
constexpr void uint32_arraynificator(std::uint32_t val, uint8_t output[4])
{
    output[0] = (std::uint8_t)(val >> 24);
    output[1] = (std::uint8_t)(val >> 16);
    output[2] = (std::uint8_t)(val >> 8);
    output[3] = (std::uint8_t)(val);
}

/**
 * @brief   Takes a single float value and splits it into four uint8_t values
 *
 * @param   val: The value to split
 * @param   output : The desired output destination
 * @retval  None
 *
 * @note    This function swaps the endianness of the float
 */
void float_arraynificator(float val, std::uint8_t output[4])
{
    std::uint32_t value = *reinterpret_cast<std::uint32_t*>(&val);
    output[3]           = (std::uint8_t)(value >> 24);
    output[2]           = (std::uint8_t)(value >> 16);
    output[1]           = (std::uint8_t)(value >> 8);
    output[0]           = (std::uint8_t)(value);
}

/**
 * @brief   Takes a `double` value and splits it into 8 bytes.
 *
 * @param   val: The double to split
 * @param   output: The desired output destination
 * @retval  None
 */
void double_arraynificator(double val, std::uint8_t output[8])
{
    std::uint64_t temp = reinterpret_cast<std::uint64_t>(&val);

    output[0] = (std::uint8_t)(temp >> 56);
    output[1] = (std::uint8_t)(temp >> 48);
    output[2] = (std::uint8_t)(temp >> 40);
    output[3] = (std::uint8_t)(temp >> 32);
    output[4] = (std::uint8_t)(temp >> 24);
    output[5] = (std::uint8_t)(temp >> 16);
    output[6] = (std::uint8_t)(temp >> 8);
    output[7] = (std::uint8_t)(temp);
}

/**
 * @brief   Takes an array of 4 raw bytes, and converts them into a float.
 *
 * @param   input: The value to combine into a float
 * @retval  constructed float
 *
 * @note    This function swaps the endianness of the float
 */
float floatinificator(std::uint8_t input[4])
{
    float         val = 0.0f;
    std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(&val);

    ptr[0] = input[3];
    ptr[1] = input[2];
    ptr[2] = input[1];
    ptr[3] = input[0];

    return val;
}


/*************************************************************************************************/
}    // namespace cep
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
