/**
 * @addtogroup  defines
 * @{
 * @addtogroup  logger
 * @{
 *
 * @file        Logger.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/06/29 - 11:30
 */
#pragma once

/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */


/**
 * @defgroup    LOGGING     Logging Macros
 *
 * For examples and details on to how to use these macros, refer to the README file in the
 * cep20/Tornatest repository.
 *
 * @note The usage of the `##` symbol preceding `__VA_ARGS__` is *non-standard*, and is an
 *       extension of the language as of C++17.                                                  \n
 *       This extension is however supported by gcc, llvm/clang and msbuild.
 *
 * @{
 */
/** Get the current program location information */
#define LOG_CURRENT_LOCATION

#ifdef LOG_DEBUG /* Quick fix because `LOG_DEBUG` is defined as empty in `macros.hpp` to */
#undef LOG_DEBUG /* avoid warnings. */
#endif
/** Prints the lowest-priority message */
#define LOG_DEBUG(msg, ...)

/** Prints a low-priority message */
#define LOG_INFO(msg, ...)

/** Prints a medium-priority message */
#define LOG_WARNING(msg, ...)

/** Prints a high-priority message */
#define LOG_ERROR(msg, ...)

/** Prints the highest-priority message */
#define LOG_CRITICAL(msg, ...)
