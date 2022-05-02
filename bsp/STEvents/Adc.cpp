/**
 * @file    Adc.cpp
 * @author  Samuel Martel
 * @date    2022-05-02
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
#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_ADC_EVENTS)
#    include "../../defines/internalConfig.h"
#    include NILAI_HAL_HEADER

#    include "../../defines/Events/Events.h"
#    include "../../processes/application.hpp"

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
    {
        cep::Events::AdcEvent e {hadc, cep::Events::EventTypes::ADC_ConvCplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
    {
        cep::Events::AdcEvent e {hadc, cep::Events::EventTypes::ADC_ConvHalfCplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
    {
        cep::Events::AdcEvent e {hadc, cep::Events::EventTypes::ADC_LevelOutOfWindow};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc)
    {
        cep::Events::AdcEvent e {hadc, cep::Events::EventTypes::ADC_Error};
        cep::Application::Get()->DispatchEvent(&e);
    }
}

#endif