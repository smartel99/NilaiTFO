/**
 * @addtogroup pwmModule.cpp
 * @{
 *******************************************************************************
 * @file	pwmModule.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Nov 6, 2020
 *******************************************************************************
 */

#include "pwmModule.h"
#if defined(NILAI_USE_PWM) && defined(HAL_TIM_MODULE_ENABLED)
#include "services/logger.hpp"

PwmModule::PwmModule(TIM_HandleTypeDef* timer, PWM::Channels channel, const std::string& label)
: m_timer(timer),
  m_channel((uint32_t)channel),
  m_label(label),
  m_activeFreq(0),
  m_activeDutyCycle(0),
  m_isActive(false)
{
    CEP_ASSERT(timer != nullptr, "[PWM]: TIM Handle is NULL!");

    LOG_INFO("[PWM]: Initialized");
}

/**
 * If the initialization passed, the POST passes.
 */
bool PwmModule::DoPost()
{
    LOG_INFO("[%s]: POST OK", m_label.c_str());
    return true;
}

void PwmModule::Run()
{
}

void PwmModule::Enable()
{
    LOG_DEBUG("[PWM]: Starting PWM generation");
    HAL_TIM_PWM_Start(m_timer, m_channel);
    m_isActive = true;
}

void PwmModule::Disable()
{
    LOG_DEBUG("[PWM]: Stopping PWM generation");
    HAL_TIM_PWM_Stop(m_timer, m_channel);
    m_isActive = false;
}

void PwmModule::SetFrequency(uint64_t hz)
{
    if (hz == 0)
    {
        Disable();
        return;
    }

    uint32_t clk = HAL_RCC_GetPCLK1Freq();

    // If the PWM is currently active:
    if (m_isActive)
    {
        // Stop it.
        HAL_TIM_PWM_Stop(m_timer, m_channel);
    }

    TIM_Base_InitTypeDef init = m_timer->Init;
    init.Prescaler            = 0;

    uint64_t regVal = 0;

    // Try to find a valid prescaler/period combo for desired freq.
    while (init.Prescaler < 0xFFFF)
    {
        regVal = ((clk / (hz * (init.Prescaler + 1))) - 1) * 2;

        // If regVal <= 0xFFFF, good value.
        if (regVal <= 0xFFFF)
        {
            m_activeFreq  = hz;
            init.Period   = (uint32_t)regVal;
            m_timer->Init = init;
            HAL_TIM_Base_DeInit(m_timer);
            HAL_TIM_Base_Init(m_timer);
            LOG_DEBUG("[PWM]: Setting frequency to %d%Hz (psc: 0x%08X, per: 0x%08X, clk: %dHz)",
                      hz,
                      m_timer->Init.Prescaler,
                      regVal,
                      clk);

            // Recompute duty cycle since we changed the frequency.
            SetDutyCycle(m_activeDutyCycle);

            if (m_isActive)
            {
                // Restart PWM if it was active.
                HAL_TIM_PWM_Start(m_timer, m_channel);
            }
            return;
        }
        else
        {
            // regVal is not valid, try another prescaler value.
            init.Prescaler++;
        }
    }

    // If we get here, no valid prescaler/period combo found.
    // We don't restart the PWM if it was active.
    CEP_ASSERT(false, "Invalid frequency requested!");
}

void PwmModule::SetDutyCycle(uint32_t percent)
{
    // TODO: Fix this function so that we can set the duty cycle.
    // Clip percent to 100, we can't have more than a 100% duty cycle.
    //    percent           = (percent < 100) ? percent : 100;
    //    float requestedDC = (float)percent / 100.0f;

    // If the PWM is currently active, disable it.
    if (m_isActive)
    {
        HAL_TIM_PWM_Stop(m_timer, m_channel);
    }

    m_activeDutyCycle = percent;
    m_timer->Instance->CCR1 =
      m_timer->Instance->ARR / 2;    //(uint32_t)((float)m_activeFreq * requestedDC);

    LOG_DEBUG("[PWM]: Setting duty cycle to %d%% (CCR1: 0x%08X)", percent, m_timer->Instance->CCR1);
    // If the PWM was active, re-enable it.
    if (m_isActive)
    {
        HAL_TIM_PWM_Start(m_timer, m_channel);
    }
}

#endif
