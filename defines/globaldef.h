/**
 * @addtogroup Defines
 * {
 * @file   globaldef.h
 * @author Pascal-Emmanuel Lachance
 * @p      https://www.github.com/Raesangur/
 * @date   2019/07/07, 20:01
 *
 * @defgroup    DAUGHTERBOARD   Daughterboard
 * @defgroup    MOTHERBOARD     Motherboard
 */
#ifndef GLOBALDEF_H
#define GLOBALDEF_H
/*****************************************************************************/
/* Includes ---------------------------------------------------------------- */

/* C standard library files */
#include <stdbool.h> /* Boolean types */
#include <stddef.h>  /* size_t type, offsetof and NULL macro */
#include <stdint.h>  /* Specific width integers */
#include <limits.h>  /* Implementation of variable size */

#include "defines/compilerDefines.h"

#include "main.h"

/* Error handling */
//#include "defines/errorCodes.h"
//#include "services/debug.h"
//#include "services/errorHandler.h"

/*****************************************************************************/
/* Defines ----------------------------------------------------------------- */

/* Generic definitions */
#define ON 1
#define OFF 0
#define INPUT 1
#define OUTPUT 0

#define HIGH true
#define LOW false
#define YES true
#define NO false
#define COMPLETED true
#define NOT_COMPLETED false
#define UPDATED true
#define NOT_UPDATED false
#define BUSY true
#define CLEAR false
#define SUCCESS true
#define FAILURE false
#define ACTIVE true
#define INACTIVE false
#define ENABLED true
#define DISABLED false

/*------------------------------------*/
/* Preconditional compilation */
#define IN_PROGRESS                                                            \
  1                 /* Temporary define, to be                                 \
                     * removed in production                                   \
                     */
#define ALLOW_DEBUG /* Allow the debug functions */
#define FOLD

#if MASTERBOARD
#define BOARD "MASTERBOARD"
#define VERSION "m_0.01"
#elif DRIVEBOARD

#define BOARD "DRIVEBOARD"
#define VERSION "d_0.01"
#else
#define BOARD "UNKNOWN BOARD"
#endif

/*****************************************************************************/
/* Macros ------------------------------------------------------------------ */
#define sizeof_array(x) (size_t)(sizeof(x) / sizeof((x)[0]))

#define IS_POINTER_NOT_NULL(ptr) (ptr != NULL)

#define ENABLE_MODULE(...) isModuleEnabled = true
#define DISABLE_MODULE(...) isModuleEnabled = false

/*****************************************************************************/
#endif /* GLOBALDEF_H */
/**
 * @}
 */

/* Have a wonderful day! :) */
/****** END OF FILE ******/
