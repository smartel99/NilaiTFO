/*
 * @addtogroup  defines
 * @{
 * @addtogroup  macros
 * @{
 *
 * @file        Macros.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/07/01 - 11:30
 */
/*************************************************************************************************/
#ifndef _MACROS_HPP_H_
#define _MACROS_HPP_H_

#include "shared/processes/application.hpp"

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/* This is a fix to remove intellisense warnings when parsing files - Pascal 2020/07/02 */
#ifndef LOG_DEBUG
#define LOG_DEBUG(x) (void)(x)
#endif


/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */

/**
 * @brief   Returns the number of elements in a C-style array (instead of the size in byte)
 *
 * @param   x : The array to calculate the size
 */
#define sizeof_array(x) (size_t)(sizeof(x) / sizeof((x)[0]))

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
#if defined(DEBUG)
#define CEP_ASSERT(x, msg, ...)                                                                    \
    do                                                                                             \
    {                                                                                              \
        if ((x) == false)                                                                          \
        {                                                                                          \
            LOG_DEBUG(msg, ##__VA_ARGS__);                                                         \
            cep::Application::AssertFailed();                                                      \
        }                                                                                          \
    } while (false)
#else
#define CEP_ASSERT(x, msg, ...)
#endif

/**
 * @brief   This macro is an alternative to the @def `CEP_ASSERT` macro, specifically for checking
 *          the state of pointers.                                                               \n
 *          It simply calls the `CEP_ASSERT` macro with a custom message.                        \n
 *          It also *stringifies* the pointer, and concatenates with with a default error message.
 *
 * @param   ptr : Pointer to evaluate its equality with `NULL`
 *
 * @example (Will print "data was null" and infinetly loop if `data` was `NULL`.)
 * ```cpp
 *          CEP_ASSERT_NULL(data);
 * ```
 */
#if defined(DEBUG)
#define CEP_ASSERT_NULL(ptr)                                                                       \
    do                                                                                             \
    {                                                                                              \
        CEP_ASSERT((ptr != nullptr), #ptr " was null");                                            \
    } while (false)
#else
#define CEP_ASSERT_NULL(ptr)
#endif

/**
 * @brief   Checks if the current module is enabled before entering a module's function's
 *          implementation.                                                                      \n
 *          This macro exits the function if the module is not enabled.                          \n
 *          For functions with a non-void return type, this macro can be provided with a default
 *          return value.
 *
 * @param   ret: Default return value that will be used if the module is not enabled.
 *
 * @example (Will return with default value `0` if the module is not yet enabled.)
 * ```cpp
 *      int foo()
 *      {
 *          CHECK_ENABLED(0);
 *          // ...
 *      }
 * ```
 */
#define CHECK_ENABLED(ret)                                                                         \
    do                                                                                             \
    {                                                                                              \
        if (GetInstance()->IsEnabled() == false)                                                   \
        {                                                                                          \
            return ret;                                                                            \
        }                                                                                          \
    } while (false)


#define GETTER  [[nodiscard]] ALWAYS_INLINE


#ifndef UNUSED
#define UNUSED(X) (void)X /* To avoid warnings */
#endif


/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
