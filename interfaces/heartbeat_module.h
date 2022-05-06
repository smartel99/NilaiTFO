/**
 ******************************************************************************
 * @addtogroup heartbeatModule
 * @{
 * @file    heartbeatModule
 * @author  Samuel Martel
 * @brief   Header for the heartbeatModule module.
 *
 * @date 9/23/2020 1:45:10 PM
 *
 ******************************************************************************
 */
#ifndef GUARD_HEARTBEATMODULE_H
#    define GUARD_HEARTBEATMODULE_H

#    if defined(NILAI_USE_HEARTBEAT)
/*****************************************************************************/
/* Includes */
#        include "../defines/led_pattern.h"
#        include "../defines/module.h"
#        include "../defines/pin.h"

#        include <string>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
namespace Nilai::Interfaces
{
class HeartbeatModule : public Nilai::Module
{
public:
    HeartbeatModule(const Nilai::Pin& pin);
    ~HeartbeatModule() override = default;

    bool DoPost() override;
    void Run() override;

private:
    Nilai::Pin m_led;

    Nilai::LedPattern m_defaultPattern {500, 500, -1};
};
}    // namespace Nilai::Interfaces
/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#    endif /* GUARD_HEARTBEATMODULE_H */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
