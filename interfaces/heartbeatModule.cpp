/**
 ******************************************************************************
 * @addtogroup heartbeatModule
 * @{
 * @file    heartbeatModule
 * @author  Samuel Martel
 * @brief   Sources for the heartbeatModule module.
 *
 * @date 9/23/2020 1:45:10 PM
 *
 ******************************************************************************
 */
#include "heartbeatModule.h"

#if defined(NILAI_USE_HEARTBEAT)
HeartbeatModule::HeartbeatModule(const Pin& pin, const std::string& label)
    : m_label(label), m_led(pin)
{
}

void HeartbeatModule::Run( )
{
    static uint32_t nextChange   = 0;
    static bool     currentState = false;

    if (HAL_GetTick( ) >= nextChange)
    {
        if (currentState)
        {
            // Turn the LED off.
            HAL_GPIO_WritePin(m_led.port, m_led.pin, GPIO_PIN_RESET);
            nextChange   = HAL_GetTick( ) + m_defaultPattern.timeOff;
            currentState = false;
        }
        else
        {
            // Turn the LED on.
            HAL_GPIO_WritePin(m_led.port, m_led.pin, GPIO_PIN_SET);
            nextChange   = HAL_GetTick( ) + m_defaultPattern.timeOn;
            currentState = true;
        }
    }
}
#endif
