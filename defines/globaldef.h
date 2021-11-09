/**
 * @addtogroup Defines
 * {
 * @file   globaldef.h
 * @author Pascal-Emmanuel Lachance
 * @date   2019/07/07, 20:01
 *
 */
#ifndef GLOBALDEF_H
#    define GLOBALDEF_H
/*****************************************************************************/
/* Includes ---------------------------------------------------------------- */
#    include "stdbool.h"
#    include "stddef.h"
#    include "stdint.h"
/*****************************************************************************/
/* Defines ----------------------------------------------------------------- */

/* Generic definitions */

/*------------------------------------*/
/* Preconditional compilation */
#    if defined(DEBUG)
#        define IN_PROGRESS 1 /* Temporary define, to be removed in production */
#        define ALLOW_DEBUG   /* Allow the debug functions */
#    else
#        define IN_PROGRESS 0
#    endif
#    define FOLD

/*****************************************************************************/
/* Macros ------------------------------------------------------------------ */
#    define sizeof_array(x) (size_t)(sizeof(x) / sizeof((x)[0]))

#    define IS_POINTER_NOT_NULL(ptr) (ptr != NULL)

/*****************************************************************************/
#endif /* GLOBALDEF_H */
/**
 * @}
 */

/* Have a wonderful day! :) */
/****** END OF FILE ******/
