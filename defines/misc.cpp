/**
 * @addtogroup defines
 * @{
 * @addtogroup misc
 * @{
 * @file    misc.cpp
 * @author  Pascal-Emmanuel Lachance
 * @p       https://www.github.com/Raesangur
 * @date    2019/08/27, 14:22
 *
 * @brief   Miscelleanous functions including :
 *
 * @version 2020/08/10 - Switch to C++17
 */
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "shared/defines/misc.hpp"

#include <cmath>
#include <cstring>

/*************************************************************************************************/
/* Global variables ---------------------------------------------------------------------------- */
void* const g_NullPointer = nullptr;

/*************************************************************************************************/
/* Public functions definitions ---------------------------------------------------------------- */

/**
 * @brief   Forces a null termination character at the end of a character array
 * @param   string: The character array to force null-terminate
 * @param   size: The size in characters of the char array
 * @retval  None
 */
void forceNullTerminationCharacter(char* string, std::size_t size)
{
    /* Access last element of the array and clear it */
    string[size - 1] = NULLCHAR;
}

/**
 * @brief   Clears an array using memset.
 * @param   array: the address of the array to clear
 * @param   length: the number of bytes to clear
 * @retval  None
 */
void clearArray(void* array, std::size_t length)
{
    memset(array, 0x00U, length);
}

/**
 * @brief   Count the number of 1s in the passed bytes.
 * @param   bytes: A pointer to the bytes to check
 * @param   len:   The number of bytes to check
 * @retval  The number of 1s
 */
size_t countOfOnesInBytesInator(uint8_t* bytes, uint8_t len)
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
    return count;
#endif
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
bool plus_minus(std::int32_t value, std::int32_t compare, std::int32_t margin)
{
    /* Take the absolute values */
    value   = std::abs(value);
    compare = std::abs(compare);
    margin  = std::abs(margin);

    const std::int32_t upperRange = compare + margin;
    const std::int32_t lowerRange = (margin <= compare) ? compare - margin : 0;

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
bool plus_minus(double value, double compare, double margin)
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

/*************************************************************************************************/
/**
 * @}
 * @}
 */

/* Have a wonderful day! :) */
/****** END OF FILE ******/

size_t cep::FindStringInVector(const std::string& str, const std::vector<uint8_t>& vec)
{
    size_t strLen = str.size();
    size_t vecLen = vec.size();

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
}
