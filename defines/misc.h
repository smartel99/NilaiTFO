/**
 * @addtogroup misc
 * @{
 * @file    misc.hpp
 * @author  Pascal-Emmanuel Lachance
 * @date    2019/08/27, 14:22
 *
 * @brief   Header file for small miscelleanous functions and features.
 *
 * @version 2020/08/10 - Switch to C++17
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

namespace cep
{
/*************************************************************************************************/
/* Definitions --------------------------------------------------------------------------------- */
#define NULLCHAR '\0' /* Null termination character */

/*************************************************************************************************/
/* Function-like macros ------------------------------------------------------------------------ */

/**
 * @def     LABEL_TO_STRING
 * @brief   Turn a label (such as a raw enum value) into a `const char*` string
 */
#define LABEL_TO_STRING(x) #x

/*************************************************************************************************/
/* Function declarations ----------------------------------------------------------------------- */
void forceNullTerminationCharacter(char* string, std::size_t size);
void clearArray(void* array, std::size_t length);

std::uint8_t countOfOnesInBytesInator(std::uint8_t* bytes, std::uint8_t len);

bool plus_minus(std::int32_t value, std::int32_t compare, std::int32_t margin);
bool plus_minus(double value, double compare, double margin);

size_t FindStringInVector(const std::string& str, const std::vector<uint8_t>& vec);

uint64_t Hash(const std::string& str);

template<typename T>
std::string IntToHex(T i)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(sizeof(T)) << std::hex << i;
    return stream.str();
}

/*************************************************************************************************/
/* Have a wonderful day! :) */
}    // namespace cep
/**
 * @}
 * @}
 */
/****** END OF FILE ******/
