/**
 * @file    Uart.cpp
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
#if defined(NILAI_USE_EXPERIMENTAL)
#    if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_UART_EVENTS)
#        include "../../defines/internal_config.h"
#        include NILAI_HAL_HEADER
#        include "../../processes/application.h"

#        include "../../defines/events/events.h"

extern "C"
{
    void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_TxHalfCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_TxCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_RxHalfCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_RxCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_Error};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_AbortCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_AbortCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_AbortTxCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef* huart)
    {
        Nilai::Events::UartEvent e {huart, Nilai::Events::EventTypes::UART_AbortRxCplt};
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
    {
        Nilai::Events::UartRxEvent e {huart, Size};
        Nilai::Application::Get()->DispatchEvent(&e);
    }
}

#    endif
#endif
