/**
 * @addtogroup defines
 * @{
 * @addtogroup misc
 * @{
 * @file    misc.cpp
 * @author  Pascal-Emmanuel Lachance
 * @date    2019/08/27, 14:22
 *
 * @brief   Miscelleanous functions including :
 *
 * @version 2020/08/10 - Switch to C++17
 */
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "misc.h"

#include <algorithm>
#include <cmath>
#include <cstring>

/*************************************************************************************************/
/* Global variables ---------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Public functions definitions ---------------------------------------------------------------- */

/**
 * @brief   Forces a null termination character at the end of a character array
 * @param   string: The character array to force null-terminate
 * @param   size: The size in characters of the char array
 * @retval  None
 */
void Nilai::forceNullTerminationCharacter(char* string, size_t size)
{
    /* Access last element of the array and clear it */
    string[size - 1] = '\0';
}

/**
 * @brief   Clears an array using memset.
 * @param   array: the address of the array to clear
 * @param   length: the number of bytes to clear
 * @retval  None
 */
void Nilai::clearArray(void* array, size_t length)
{
    memset(array, 0x00U, length);
}

/**
 * @brief   Count the number of 1s in the passed bytes.
 * @param   bytes: A pointer to the bytes to check
 * @param   len:   The number of bytes to check
 * @retval  The number of 1s
 */
size_t Nilai::countOfOnesInBytesInator(const uint8_t* bytes, uint8_t len)
{
    size_t count = 0;
#if defined(__GCC__)
    for (std::size_t i = 0; i < len; i++)
    {
        count += __builtin_popcount(bytes[i]);
    }
#else

    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            count += (uint8_t)((bytes[i] >> j) & 0x01);
        }
    }
#endif
    return count;
}

/**
 * @brief   Check if a value is in a reasonable margin of another value.     \n
 *          If the value to be compared (x) is bigger than a specific value (y)
 *          plus a margin (z), and smaller than the specific value (y) minus
 *          a margin (z) return true. Otherwise, return false.               \n
 *          This returns true when the value to be compared (x) is in a certain
 *          range of a specific value (y).
 *          This range being set by a margin (z).                            \n
 *          if ((x < (y + z)) && (x > (y - z)))
 *
 * @param   value: First value we want to compare
 * @param   compare: Second value we want to compare
 * @param   margin: margin between the two values
 *
 * @retval  true: If the first value falls within the specified margin of the
 *                second value.
 *          false: If the two values are too far apart from each other
 */
bool Nilai::plus_minus(int32_t value, int32_t compare, int32_t margin)
{
    /* Take the absolute values */
    value   = std::abs(value);
    compare = std::abs(compare);
    margin  = std::abs(margin);

    const int32_t upperRange = compare + margin;
    const int32_t lowerRange = (margin <= compare) ? compare - margin : 0;

    if (value < upperRange && value > lowerRange)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief   Check if a value is in a reasonable margin of another value.     \n
 *          If the value to be compared (x) is bigger than a specific value (y)
 *          plus a margin (z), and smaller than the specific value (y) minus
 *          a margin (z) return true. Otherwise, return false.               \n
 *          This returns true when the value to be compared (x) is in a certain
 *          range of a specific value (y).
 *          This range being set by a margin (z).                            \n
 *          if ((x < (y + z)) && (x > (y - z)))
 *
 * @param   value: First value we want to compare
 * @param   compare: Second value we want to compare
 * @param   margin: margin between the two values
 *
 * @retval  true: If the first value falls within the specified margin of the
 *                second value.
 *          false: If the two values are too far apart from each other
 */
bool Nilai::plus_minus(double value, double compare, double margin)
{
    if ((value <= (compare + margin)) && (value > (compare - margin)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t Nilai::FindStringInVector(const std::string& str, const std::vector<uint8_t>& vec)
{
#if __cplusplus >= 201703L
    auto it = std::search(vec.begin(), vec.end(), str.begin(), str.end());
    if (it == vec.end())
    {
        return std::string::npos;
    }
    else
    {
        return (it - vec.begin());
    }
#else
    size_t strLen = str.size();
    size_t vecLen = vec.size();

    // If the string is bigger than the vector, we won't find the string in the vector ;)
    if (strLen > vecLen)
    {
        return std::string::npos;
    }
    for (size_t i = 0; i <= vecLen - strLen; ++i)
    {
        size_t j = 0;
        while (j < vecLen && vec[i + j] == str[j])
        {
            j++;
        }
        if (j == strLen)
        {
            // Match found.
            return i;
        }
    }

    return std::string::npos;
#endif
}

std::vector<uint8_t> Nilai::StrToVec(const std::string& str)
{
    std::vector<uint8_t> v;
    v.reserve(str.size());

    for (const auto& c : str)
    {
        v.push_back(c);
    }

    return v;
}

std::string Nilai::IntToHex(size_t i)
{
    char msg[9] = {0};

    sprintf(msg, "%08X", i);

    return std::string(msg);
}

std::vector<uint8_t> Nilai::StrToVec(const std::string& str, size_t maxSize)
{
    std::vector<uint8_t> v;
    v.reserve(maxSize);

    for (const auto& c : str)
    {
        v.push_back(c);
        if (v.size() >= maxSize)
        {
            break;
        }
    }

    return v;
}
/*************************************************************************************************/
/**
 * @}
 * @}
 */

/* Have a wonderful day! :) */
/****** END OF FILE ******/
