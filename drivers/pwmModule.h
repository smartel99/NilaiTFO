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
#    include "../defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_TIM_MODULE_ENABLED)
#        include "../defines/macros.hpp"
#        include "../defines/misc.hpp"
#        include "../defines/module.hpp"

#        include "PWM/Enums.h"

#        if defined(NILAI_USE_EVENTS)
#            include "../defines/Events/Events.h"
#        endif

#        include <string>
#        include <vector>


class PwmModule : public cep::Module
{
public:
    PwmModule(TIM_HandleTypeDef* timer, cep::PWM::Channels channel, std::string label);

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

#        if defined(NILAI_USE_EVENTS)
    bool OnEvent(cep::Events::Event* event) override;
#        endif

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

#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO PWM Module enabled, but HAL_TIM_MODULE_ENABLED is not defined!
#        endif
#    endif
#endif

/***********************************************/
/* Defines */

/***********************************************/
/* Function declarations */

/**
 * @}
 */
/* END OF FILE */
#endif /* NILAI_PWMMODULE_H_ */
