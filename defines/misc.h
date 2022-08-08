/**
 * @file    misc.h
 * @author  Pascal-Emmanuel Lachance
 * @author  Samuel Martel
 * @date    2019/08/27, 14:22
 *
 * @brief   Header file for small miscellaneous functions and features.
 *
 * @version 2020/08/10 - Switch to C++17
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */

#include "macros.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace Nilai
{
/*************************************************************************************************/
/* Definitions --------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Function-like macros ------------------------------------------------------------------------ */

/*------------------------------------*/

/*************************************************************************************************/
/* Function declarations ----------------------------------------------------------------------- */
void forceNullTerminationCharacter(char* string, size_t size);
void clearArray(void* array, size_t length);

size_t countOfOnesInBytesInator(const uint8_t* bytes, uint8_t len);

bool plus_minus(int32_t value, int32_t compare, int32_t margin);
bool plus_minus(double value, double compare, double margin);

size_t FindStringInVector(const std::string& str, const std::vector<uint8_t>& vec);

constexpr uint64_t Hash(const char* str)
{
    uint64_t result = 0;
    while (*str != 0)
    {
        result = result * 31 + *str++;
    }
    return result;
}

std::string IntToHex(size_t i);

template<typename T>
std::vector<uint8_t> ValToVector(const T& val)
{
    std::vector<uint8_t> v;
    v.reserve(sizeof(T));

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&val);
    for (size_t i = 0; i < sizeof(T); i++)
    {
        v.push_back(*ptr);
        ptr++;
    }

    return v;
}

template<bool, typename T>
std::vector<uint8_t> ValToVector(const T& val)
{
    std::vector<uint8_t> v;
    v.reserve(sizeof(T));

    int off = (sizeof(T) * 8) - 8;    // Amount of bits to shift right.
    for (int i = off; i >= 0; i -= 8)
    {
        v.push_back((uint8_t)(val >> i));
    }

    return v;
}

template<typename T>
T VectorToVal(const std::vector<uint8_t>& vec)
{
    return (*reinterpret_cast<const T*>(vec.data()));
}

template<typename T, bool>
T VectorToVal(const std::vector<uint8_t>& vec)
{
    T      t   = {};
    size_t off = (sizeof(T) * 8) - 8;    // Amount of bits to shift left.
    for (const auto& v : vec)
    {
        t |= v << off;
        off -= 8;
    }

    return t;
}

template<typename T, size_t N>
T VectorToVal(const std::array<uint8_t, N>& arr)
{
    return (*reinterpret_cast<const T*>(arr.data()));
}

template<size_t N>
std::string VectorToVal(const std::array<uint8_t, N>& arr)
{
    return std::string((const char*)arr.data(), arr.size());
}

std::vector<uint8_t> StrToVec(const std::string& str);
std::vector<uint8_t> StrToVec(const std::string& str, size_t maxSize);

template<std::integral T, bool, size_t>
constexpr T PackImpl()
{
    return 0;
}

template<std::integral T,
         bool          bigEndian = false,
         size_t        shiftBy   = bigEndian ? (sizeof(T) * 8) - 8 : 0,
         std::convertible_to<uint8_t>... Args>
constexpr T PackImpl(uint8_t v, Args... args)
{
    return (static_cast<T>(v) << shiftBy) | PackImpl < T, bigEndian,
           bigEndian ? shiftBy - 8 : shiftBy + 8 > (args...);
}

template<std::integral T,
         bool          bigEndian = false,
         size_t        shiftBy   = bigEndian ? (sizeof(T) * 8) - 8 : 0,
         std::convertible_to<uint8_t>... Args>
constexpr T Pack(Args... args)
{
    static_assert(sizeof...(args) == sizeof(T), "Invalid amount of data provided!");

    return PackImpl<T, bigEndian, shiftBy>(std::forward<Args>(args)...);
}

/*************************************************************************************************/
/* Have a wonderful day! :) */
}    // namespace Nilai
/****** END OF FILE ******/
