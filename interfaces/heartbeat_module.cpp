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
#include "heartbeat_module.h"

#if defined(NILAI_USE_HEARTBEAT)
#    include "../services/logger.h"

#    include "../services/time.h"

namespace Nilai::Interfaces
{
HeartbeatModule::HeartbeatModule(const Pin& pin) : m_led(pin)
{
}

bool HeartbeatModule::DoPost()
{
    LOG_INFO("[HB]: POST OK");
    return true;
}

void HeartbeatModule::Run()
{
    static uint32_t nextChange   = 0;
    static bool     currentState = false;

    if (GetTime() >= nextChange)
    {
        if (currentState)
        {
            // Turn the LED off.
            m_led.Set(false);
            nextChange   = GetTime() + m_defaultPattern.timeOff;
            currentState = false;
        }
        else
        {
            // Turn the LED on.
            m_led.Set(true);
            nextChange   = GetTime() + m_defaultPattern.timeOn;
            currentState = true;
        }
    }
}
}    // namespace Nilai::Interfaces
#endif
