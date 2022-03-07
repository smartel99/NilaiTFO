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

/**
 * @brief Structure representing a physical pin.
 */
struct Pin
{
    //! The default pin, representing "no pin".
    static constexpr uint16_t s_defaultPin = 0;

    // Use GPIOA as a default port to avoid crashes with default-constructed pins.
    GPIO_TypeDef* port = GPIOA;
    uint16_t      pin  = s_defaultPin;

    /**
     * @brief Sets the pin in the desired state.
     * @param state True to set the pin to HIGH, false to set it to LOW.
     *
     * @note The pin must be configured in output mode for this to do anything.
     */
    void Set(bool state) const;

    /**
     * @brief Gets the current state of the pin.
     * @returns True if the pin is HIGH
     * @returns False if the pin is LOW
     *
     * @note If the pin is in output mode, this function returns the current *desired* state,
     * not the actual measured state.
     */
    [[nodiscard]] bool Get() const;

    bool operator==(const Pin& other) const
    {
        return ((this->port == other.port) && (this->pin == other.pin));
    }

    bool operator!=(const Pin& other) const { return !(*this == other); }

    explicit operator bool() const { return Get(); }

    /**
     * @brief Check if the pin is default-constructed.
     * @returns True if the pin is default-constructed.
     */
    [[nodiscard]] bool IsDefault() const { return pin == s_defaultPin; }
};

}    // namespace cep
/* Have a wonderful day :) */
#endif /* GUARD_PIN_H */
/**
 * @}
 */
/****** END OF FILE ******/
