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

#ifndef PWMMODULE_H_
#define PWMMODULE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_PWM)
#    include "stm32f4xx_hal.h"
#    if defined(HAL_TIM_MODULE_ENABLED)
#        include "defines/macros.hpp"
#        include "defines/misc.hpp"
#        include "defines/module.hpp"

#        include <string>
#        include <vector>

namespace PWM
{
enum class Channels
{
    CH1 = 0,
    CH2 = 1,
    CH3 = 2,
    CH4 = 3
};
}

class PwmModule : public cep::Module
{
public:
    PwmModule(TIM_HandleTypeDef* timer, PWM::Channels channel, const std::string& label);

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    void Enable( );
    void Disable( );
    bool IsEnabled( ) const { return m_isActive; }

    void     SetFrequency(uint64_t hz);
    uint64_t GetFrequency( ) const { return m_activeFreq; }

    void     SetDutyCycle(uint32_t percent);
    uint32_t GetDutyCycle( ) const { return m_activeDutyCycle; }

private:
    TIM_HandleTypeDef* m_timer   = nullptr;
    uint32_t           m_channel = 0;
    std::string        m_label   = "";

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
#endif /* PWMMODULE_H_ */
