/**
 * @file    Tim.cpp
 * @author  Samuel Martel
 * @date    2022-03-15
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_TIMER_EVENTS)
#    include "../../defines/Events/Events.h"
#    include "../../processes/application.hpp"

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    cep::Events::TimEvent e(htim, cep::Events::EventTypes::Tim_PeriodElapsed);
    cep::Application::Get()->DispatchEvent(&e);
}

extern "C" void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim)
{
    cep::Events::TimEvent e(htim, cep::Events::EventTypes::Tim_OC_DelayElapsed);
    cep::Application::Get()->DispatchEvent(&e);
}

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
    cep::Events::TimEvent e(htim, cep::Events::EventTypes::Tim_IC_Capture);
    cep::Application::Get()->DispatchEvent(&e);
}

extern "C" void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim)
{
    cep::Events::TimEvent e(htim, cep::Events::EventTypes::Tim_PWM_PulseFinished);
    cep::Application::Get()->DispatchEvent(&e);
}

extern "C" void HAL_TIM_TriggerCallback(TIM_HandleTypeDef* htim)
{
    cep::Events::TimEvent e(htim, cep::Events::EventTypes::Tim_Trigger);
    cep::Application::Get()->DispatchEvent(&e);
}

extern "C" void HAL_TIM_ErrorCallback(TIM_HandleTypeDef* htim)
{
    cep::Events::TimEvent e(htim, cep::Events::EventTypes::Tim_Error);
    cep::Application::Get()->DispatchEvent(&e);
}
#endif