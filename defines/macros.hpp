/*
 * @addtogroup  defines
 * @{
 * @addtogroup  macros
 * @{
 *
 * @file        Macros.hpp
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/07/01 - 11:30
 */
/*************************************************************************************************/
#ifndef _MACROS_HPP_H_
#    define _MACROS_HPP_H_

#    include "processes/application.hpp"
#    include "services/logger.hpp"

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */

/**
 * @brief   Returns the number of elements in a C-style array (instead of the size in byte)
 *
 * @param   x : The array to calculate the size
 */
#    define sizeof_array(x) (size_t)(sizeof(x) / sizeof((x)[0]))

/**
 * @brief   Check if a boolean condition `x` is true, if false:                                  \n
 *          - Print an error message
 *
 * @param   x   : Boolean evaluation
 * @param   msg : Error message to print in case of failed assertion
 * @param   ... : Additionnal parameters to be passed (following libfmt's formatting in the `msg`)
 *
 * @note    The macro is embedded within a `do while` loop to allow for a `;` to be inserted at the
 *          end of the macro without warnings.
 */
#    if defined(NILAI_TEST)
#        include <cstdlib>
#include <iostream>
#        define CEP_ASSERT(x, msg, ...)                                                            \
            do                                                                                     \
            {                                                                                      \
                if (!(x))                                                                          \
                {                                                                                  \
                    LOG_ERROR(                                                                     \
                      "Assert failed at line %i in %s: " msg, __LINE__, __FILE__, ##__VA_ARGS__);  \
                    std::cout << std::endl;                                                        \
                    abort();                                                                       \
                }                                                                                  \
            } while (0)
#    elif defined(DEBUG)
#        define CEP_ASSERT(x, msg, ...)                                                            \
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
#        define CEP_ASSERT(x, msg, ...)
#    endif

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
