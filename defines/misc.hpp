/**
 * @addtogroup misc
 * @{
 * @file    misc.hpp
 * @author  Pascal-Emmanuel Lachance
 * @p       https://www.github.com/Raesangur
 * @date    2019/08/27, 14:22
 *
 * @brief   Header file for small miscelleanous functions and features.
 *
 * @version 2020/08/10 - Switch to C++17
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "shared/defines/pch.hpp"

namespace cep
{
/*************************************************************************************************/
/* Definitions --------------------------------------------------------------------------------- */
#define NULLCHAR '\0' /* Null termination character */

/*************************************************************************************************/
/* Function-like macros ------------------------------------------------------------------------ */

/*------------------------------------*/
#pragma region Array macros
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
 * @def     COPY_ARRAY
 * @brief   Copy array `src` into array `dest`.
 *          This macro expands into a simple `for` loop.
 *
 * @param   source : The array to copy from
 * @param   destination : The array to copy data into
 *
 * @warning This macro does <strong> NOT </strong> check if the sizes of the
 *          arrays match. Moreover, it bases its number of iterations on the
 *          size of the `src` array <small>(in order to allow it to be used to
 *          copy an array into an output parameter array)</small>, which means
 *          that if array `src` is bigger and `dest`, it will keep writing
 *          beyond `dest`'s bounds.
 */
#define COPY_ARRAY(src, dest)                                                                      \
    do                                                                                             \
    {                                                                                              \
        for (uint8_t i = 0; i < sizeof_array(src); i++)                                            \
        {                                                                                          \
            dest[i] = src[i];                                                                      \
        }                                                                                          \
    } while (false)
#pragma endregion

/*------------------------------------*/
#pragma region Math and comparison macros
/**
 * @def     REPLACE_MAXIMUMS
 *
 * @brief   Replace a value with another value if the second value is biggest
 *          than the first.
 *          (if x > y, y = x)
 *
 * @param   value :   The value to be compared, will replace 'maximum'.
 * @param   maximum : The value to compare to, that will be replaced if `value`
 *                    is bigger than `maximum`.
 */
#define REPLACE_MAXIMUMS(value, maximum)                                                           \
    do                                                                                             \
    {                                                                                              \
        if ((value) > (maximum))                                                                   \
        {                                                                                          \
            (maximum) = (value);                                                                   \
        }                                                                                          \
    } while (false)

/**
 * @def     REPLACE_MINIMUMS
 *
 * @brief   Replace a value with another value if the second value is smaller
 *          than the first.
 *          (if x < y, y = x)
 *
 * @param   value :   The value to be compared, will replace 'maximum'.
 * @param   minimum : The value to compare to, that will be replaced if `value`
 *                    is smaller than `maximum`.
 */
#define REPLACE_MINIMUMS(value, minimum)                                                           \
    do                                                                                             \
    {                                                                                              \
        if ((value) < (minimum))                                                                   \
        {                                                                                          \
            (minimum) = (value);                                                                   \
        }                                                                                          \
    } while (false)
#pragma endregion

/*------------------------------------*/
/**
 * @def     TENSION_DIVIDER
 * @brief   Calculate the tension value at the output of a tension divider
 *          characterized with resistances values `R1` and `R2`.             \n
 *          This uses the following formula:                                 \n
 *          Vout = __Vs_*_R2__                                               \n
 *                  (R1 + R2)                                                \n
 *          Where:
 *          (#) `Vs` is the voltage source (V)
 *          (#) `R1` is the resistance of the 1st resistor (Ohms)
 *          (#) `R2` is the resistance of the 2nd resistor (Ohms)
 *          (#) `Vout` is the output voltage (V)
 *
 *          Use the following diagram as reference:
 *          Vs|----/\/\/\/--/-----Vout
 *                    R1    \
 *                          /  R2
 *                          \
 *                          /
 *                         GND
 *
 * @param   Vs : Source voltage (V)
 * @param   R1 : Value of the 1st resistance (Ohms)
 * @param   R2 : Value of the 2nd resistance (Ohms)
 * @retval  Vout : Tension at the output of the voltage divider
 */
#define TENSION_DIVIDER(Vs, R1, R2)                                                                \
    (((double)(Vs) * (R2)) / ((((R1) + (R2)) == 0.0) ? 1.0 : ((R1) + (R2))))

/**
 * @def     PARALLEL_RESISTANCES
 * @brief   Calculates the resistance value of multiple resistances in parallel.
 *
 *          This uses the following formula :                                \n
 *          Rtotal = _____1_____                                             \n
 *                   __1_ + _1__                                             \n
 *                    R1     R2                                              \n
 *          Where:
 *          (#) `R1` is the value of the first resistance  (Ohms)
 *          (#) `R2` is the value of the second resistance (Ohms)
 *
 *          Use the following diagram as reference:
 *          __________________             _________
 *            /         /                      /
 *            \         \                      \  _____1_____
 *            / R1      / R2       ===         /  __1_ + _1__
 *            \         \                      \   R1     R2
 *            /         /                      /
 *           GND       GND                    GND
 *
 * @param   R1 : Value of the first resistance (Ohms)
 * @param   R2 : Value of the second resistance (Ohms)
 * @retval  Rtotal : Value of the equivalent parallel resistance (Ohms)
 *
 * @note    Values of 0ohms will be automatically converted to 1ohm to prevent
 *          divisions by 0.
 */
#define PARALLEL_RESISTANCES(R1, R2)                                                               \
    (1.0 / (1.0 / (((R1) == 0.0) ? 1.0 : (R1)) + (1.0 / (((R2) == 0.0) ? 1.0 : (R2)))))

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

/*************************************************************************************************/
/* Have a wonderful day! :) */
}    // namespace cep
/**
 * @}
 * @}
 */
/****** END OF FILE ******/
