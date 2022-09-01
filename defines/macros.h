/*
 * @file        macros.h
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

#    if defined(DEBUG) || defined(NILAI_TEST)
#        include "../defines/assertion.h"
#        include "../services/logger.h"
#    endif

#    include <string_view>
/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */

/*
 * @brief Suppresses unused variable warnings.
 */
#    define NILAI_UNUSED(x) static_cast<void>(x)

/*
 * @brief   Check if a boolean condition `x` is true, if false:                                  \n
 *          - Print an error message
 *
 * @param   x   : Boolean evaluation
 * @param   msg : Error message to print in case of failed assertion
 * @param   ... : Additional parameters to be passed (following sprintf's formatting in the `msg`)
 */
#    if defined(DEBUG)
#        define NILAI_ASSERT(x, msg, ...)                                                          \
            do                                                                                     \
            {                                                                                      \
                if (!static_cast<bool>(x))                                                         \
                {                                                                                  \
                    LOG_CRITICAL("Assert Failed at line %i %s: " msg,                              \
                                 __LINE__,                                                         \
                                 __FILE__ __VA_OPT__(, ) __VA_ARGS__);                             \
                    AssertFailed(reinterpret_cast<const uint8_t*>(__FILE__), __LINE__, 0);         \
                }                                                                                  \
            } while (false)
#    elif defined(NILAI_TEST)
#        include <cassert>
#        define NILAI_ASSERT(x, msg, ...)                                                          \
            do                                                                                     \
            {                                                                                      \
                if (!static_cast<bool>(x))                                                         \
                {                                                                                  \
                    LOG_CRITICAL("Assert Failed at line %i %s: " msg,                              \
                                 __LINE__,                                                         \
                                 __FILE__ __VA_OPT__(, ) __VA_ARGS__);                             \
                    assert(false);                                                                 \
                }                                                                                  \
            } while (false)
#    else
#        define NILAI_ASSERT(x, msg, ...)
#    endif

/*************************************************************************************************/
#endif
/* ----- END OF FILE ----- */
