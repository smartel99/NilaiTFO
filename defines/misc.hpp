/**
 * @addtogroup misc
 * @{
 * @file    misc.hpp
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

#include "macros.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace cep
{
/*************************************************************************************************/
/* Definitions --------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Function-like macros ------------------------------------------------------------------------ */

/*------------------------------------*/
/**
 * @def     CLEAR_ARRAY
 * @brief   Calls the @ref clearArray function, by also entering automatically
 *          the size of the specified array into the 2nd parameter field of the
 *          function.
 *
 * @param   arr : The name of an array to clear.
 */
#define CLEAR_ARRAY(arr) clearArray((arr), sizeof(arr));

/**
 * @def     LABEL_TO_STRING
 * @brief   Turn a label (such as a raw enum value) into a `const char*` string
 */
#define LABEL_TO_STRING(x) #x

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

template<typename T>
T VectorToVal(const std::vector<uint8_t>& vec)
{
    return (*reinterpret_cast<const T*>(vec.data()));
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

/*************************************************************************************************/
/* Have a wonderful day! :) */
}    // namespace cep
/**
 * @}
 * @}
 */
/****** END OF FILE ******/
