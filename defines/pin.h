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
#ifndef GUARD_PIN_H
#    define GUARD_PIN_H

/*****************************************************************************/
/* Includes */
#    if !defined(NILAI_TEST)
#        include "Core/Inc/gpio.h"
#    else
#        include "test/Mocks/GPIO/gpio.h"
#    endif

#    include <cstdint>

namespace cep
{
/*****************************************************************************/
/* Exported types */
struct Pin
{
    GPIO_TypeDef* port = nullptr;
    uint16_t      pin  = 0;

    void Set(bool state) const
    {
        HAL_GPIO_WritePin(port, pin, (state ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }

    [[nodiscard]] bool Get() const { return (HAL_GPIO_ReadPin(port, pin) != 0u); }

    bool operator==(const Pin& other) const
    {
        return ((this->port == other.port) && (this->pin == other.pin));
    }

    bool operator!=(const Pin& other) const { return !(*this == other); }

    explicit operator bool() const { return Get(); }
};

}    // namespace cep
/* Have a wonderful day :) */
#endif /* GUARD_PIN_H */
/**
 * @}
 */
/****** END OF FILE ******/
