/**
 ******************************************************************************
 * @file    pin.h
 * @author  Samuel Martel
 * @brief   Header for the pin module.
 *
 * @date 9/11/2020 3:04:43 PM
 *
 ******************************************************************************
 */
#ifndef NILAI_PIN_H
#    define NILAI_PIN_H

/*****************************************************************************/
/* Includes */
#    if !defined(NILAI_TEST)
#        include "Core/Inc/gpio.h"
#    else
#        include "../test/Mocks/GPIO/gpio.h"
#        include "../test/Mocks/HALMocks/stm32_mock_hal.h"
#    endif

#    include <bit>
#    include <cstdint>

namespace Nilai
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

    GPIO_TypeDef* port = nullptr;
    uint16_t      pin  = s_defaultPin;

    /**
     * @brief Sets the pin in the desired state.
     * @param state True to set the pin to HIGH, false to set it to LOW.
     *
     * @note The pin must be configured in output mode for this to do anything.
     */
    void Set(bool state) const;
    void Toggle() const;

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

    constexpr Pin(GPIO_TypeDef* _port, uint16_t _pin) : port(_port), pin(_pin) {}
    constexpr Pin() = default;
};

}    // namespace Nilai
/* Have a wonderful day :) */
#endif /* NILAI_PIN_H */
/****** END OF FILE ******/
