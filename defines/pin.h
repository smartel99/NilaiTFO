/**
 ******************************************************************************
 * @addtogroup pin
 * @{
 * @file    pin
 * @author  Samuel Martel
 * @brief   Header for the pin module.
 *
 * @date 9/11/2020 3:04:43 PM
 *
 ******************************************************************************
 */
#ifndef _pin
#define _pin

/*****************************************************************************/
/* Includes */
#include "Core/Inc/gpio.h"

#include <cstdint>

/*****************************************************************************/
/* Exported defines */


/*****************************************************************************/
/* Exported macro */


/*****************************************************************************/
/* Exported types */
struct Pin
{
    GPIO_TypeDef* port = nullptr;
    uint16_t      pin  = 0;

    void Set(bool state) { HAL_GPIO_WritePin(port, pin, (state ? GPIO_PIN_SET : GPIO_PIN_RESET)); }

    bool Get() const { return HAL_GPIO_ReadPin(port, pin); }


    bool operator==(const Pin& other) const
    {
        return ((this->port == other.port) && (this->pin == other.pin));
    }
};

/*****************************************************************************/
/* Exported functions */


/* Have a wonderful day :) */
#endif /* _pin */
/**
 * @}
 */
/****** END OF FILE ******/
