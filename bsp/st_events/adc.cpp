/**
 * @file    adc.cpp
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
 * not, see https://www.gnu.org/licenses/.
 */
#if defined(NILAI_USE_EXPERIMENTAL)
#    if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_ADC_EVENTS)
#        include "../../defines/internal_config.h"
#        include NILAI_HAL_HEADER

#        include "../../defines/events/events.h"
#        include "../../processes/application.h"

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
    {
        Nilai::Events::AdcEvent e {hadc, Nilai::Events::EventTypes::ADC_ConvCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
    {
        Nilai::Events::AdcEvent e {hadc, Nilai::Events::EventTypes::ADC_ConvHalfCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
    {
        Nilai::Events::AdcEvent e {hadc, Nilai::Events::EventTypes::ADC_LevelOutOfWindow};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc)
    {
        Nilai::Events::AdcEvent e {hadc, Nilai::Events::EventTypes::ADC_Error};
        Nilai::Application::Get()->DispatchEvent(&e);
    }
}

#    endif
#endif
