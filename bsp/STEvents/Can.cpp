/**
 * @file    Can.cpp
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
#if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_CAN_EVENTS)
#    include "../../defines/Events/Events.h"
#    include "../../processes/application.hpp"

extern "C"
{
    void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_TxMailbox0Cplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_TxMailbox1Cplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_TxMailbox2Cplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_TxMailbox0AbortCplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_TxMailbox1AbortCplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_TxMailbox2AbortCplt};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_RxFifo0MsgPending};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_RxFifo0Full};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_RxFifo1MsgPending};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_RxFifo1Full};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_SleepCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_Sleep};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_WakeUpFromRx};
        cep::Application::Get()->DispatchEvent(&e);
    }

    void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan)
    {
        cep::Events::CanEvent e {hcan, cep::Events::EventTypes::CAN_Error};
        cep::Application::Get()->DispatchEvent(&e);
    }
}
#endif
