/**
 * @addtogroup pwmModule.h
 * @{
 *******************************************************************************
 * @file	pwmModule.h
 * @author	Samuel Martel
 * @brief
 * Created on: Nov. 6, 2020
 *******************************************************************************
 */

#ifndef NILAI_PWMMODULE_H_
#define NILAI_PWMMODULE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_PWM)
#    if defined(NILAI_TEST)
#    else
#        include "../defines/internal_config.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_TIM_MODULE_ENABLED)
#            include "../defines/macros.h"
#            include "../defines/misc.h"
#            include "../defines/module.h"

#            include "PWM/enums.h"

#            include <string>
#            include <vector>

namespace Nilai::Drivers
{
class PwmModule : public Module
{
public:
    PwmModule(TIM_HandleTypeDef* timer, PWM::Channels channel, std::string label);

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    void               Enable();
    void               Disable();
    [[nodiscard]] bool IsEnabled() const { return m_isActive; }

    void                   SetFrequency(uint64_t hz);
    [[nodiscard]] uint64_t GetFrequency() const { return m_activeFreq; }

    void                   SetDutyCycle(uint32_t percent);
    [[nodiscard]] uint32_t GetDutyCycle() const { return m_activeDutyCycle; }

private:
    TIM_HandleTypeDef* m_timer   = nullptr;
    uint32_t           m_channel = 0;
    std::string        m_label;

    uint64_t m_activeFreq      = 0;
    uint32_t m_activeDutyCycle = 0;
    bool     m_isActive        = false;
};
}    // namespace Nilai::Drivers
#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO PWM Module enabled, but HAL_TIM_MODULE_ENABLED is not defined!
#            endif
#        endif
#    endif

/***********************************************/
/* Defines */

/***********************************************/
/* Function declarations */

/**
 * @}
 */
#endif
/* END OF FILE */
#endif /* NILAI_PWMMODULE_H_ */
