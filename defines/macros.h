/*
 * @addtogroup  defines
 * @{
 * @addtogroup  macros
 * @{
 *
 * @file        Macros.h
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 * @date        2020/07/01 - 11:30
 *
 * @note History:
 * - 2020/07/01: Initial - PEL
 * - 2022/05/04: Rename to macros.h, add UNUSED macro
 */
/*************************************************************************************************/
#ifndef NILAI_MACROS_H
#    define NILAI_MACROS_H

#    if defined(DEBUG)
#        include "../defines/assertion.h"
#        include "../services/logger.h"
#    endif

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */

/**
 * @brief   Returns the number of elements in a C-style array (instead of the size in byte)
 *
 * @param   x : The array to calculate the size
 */
#    define SIZEOF_ARRAY(x) (size_t)(sizeof(x) / sizeof((x)[0]))

/**
 * @brief Suppresses unused variable warnings.
 */
#    define NILAI_UNUSED(x) ((void)(x))

/**
 * @brief   Check if a boolean condition `x` is true, if false:                                  \n
 *          - Print an error message
 *
 * @param   x   : Boolean evaluation
 * @param   msg : Error message to print in case of failed assertion
 * @param   ... : Additional parameters to be passed (following sprintf's formatting in the `msg`)
 *
 * @note    The macro is embedded within a `do while` loop to allow for a `;` to be inserted at the
 *          end of the macro without warnings.
 */
#    if defined(DEBUG)
#        define NILAI_ASSERT(x, msg, ...)                                                          \
            do                                                                                     \
            {                                                                                      \
                if (!(x))                                                                          \
                {                                                                                  \
                    LOG_DEBUG(                                                                     \
                      "Assert Failed at line %i %s: " msg, __LINE__, __FILE__, ##__VA_ARGS__);     \
                    AssertFailed((uint8_t*)__FILE__, __LINE__, 0);                                 \
                }                                                                                  \
            } while (false)
#    else
#        define NILAI_ASSERT(x, msg, ...)
#    endif

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
