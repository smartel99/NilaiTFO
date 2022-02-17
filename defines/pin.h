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
    // Use GPIOA as a default port to avoid crashes with default-constructed pins.
    GPIO_TypeDef* port = GPIOA;
    uint16_t      pin  = 0;

    void Set(bool state) const;

    [[nodiscard]] bool Get() const;

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
