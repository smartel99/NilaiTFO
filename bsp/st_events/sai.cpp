/**
 * @file    Sai.cpp
 * @author  Samuel Martel
 * @date    2022-05-03
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
#if defined(NILAI_USE_EXPERIMENTAL)
#    if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_SAI_EVENTS)
#        include "../../defines/events/events.h"
#        include "../../processes/application.h"

#        include "../../defines/internal_config.h"
#        include NILAI_HAL_HEADER

extern "C"
{
    void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef* hsai)
    {
        Nilai::Events::SaiEvent e {hsai, Nilai::Events::EventTypes::SAI_TxHalfCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef* hsai)
    {
        Nilai::Events::SaiEvent e {hsai, Nilai::Events::EventTypes::SAI_TxCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* hsai)
    {
        Nilai::Events::SaiEvent e {hsai, Nilai::Events::EventTypes::SAI_RxHalfCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* hsai)
    {
        Nilai::Events::SaiEvent e {hsai, Nilai::Events::EventTypes::SAI_RxCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SAI_ErrorCallback(SAI_HandleTypeDef* hsai)
    {
        Nilai::Events::SaiEvent e {hsai, Nilai::Events::EventTypes::SAI_Error};
        Nilai::Application::Get()->DispatchEvent(&e);
    }
}
#    endif
#endif