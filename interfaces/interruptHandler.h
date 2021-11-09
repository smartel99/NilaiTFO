/**
 ******************************************************************************
 * @addtogroup interruptHandler
 * @{
 * @file    interruptHandler
 * @author  Samuel Martel
 * @brief   Header for the interruptHandler module.
 *
 * @date 9/3/2020 2:58:37 PM
 *
 ******************************************************************************
 */
#ifndef _interruptHandler
#    define _interruptHandler

/*****************************************************************************/
/* Includes */
#    include "bsp/stm32.h"

#    include <functional>

namespace cep
{
/*****************************************************************************/
/* Exported defines */


/*****************************************************************************/
/* Exported macro */


/*****************************************************************************/
/* Exported types */


class InterruptHandler
{
public:
    InterruptHandler();

    void BindInterruptToFunction(Interrupts interrupt, std::function<void()> func);


private:
};

/*****************************************************************************/
/* Exported functions */


/* Have a wonderful day :) */
#endif /* _interruptHandler */
/**
 * @}
 */
}
/****** END OF FILE ******/
