/**
 * @file    Spi.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#if defined(NILAI_USE_EXPERIMENTAL)
#    if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_SPI_EVENTS)
#        include "../../defines/events/events.h"
#        include "../../processes/application.h"

#        include "../../defines/internal_config.h"
#        include NILAI_HAL_HEADER

extern "C"
{
    void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_TxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_RxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_TxRxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_TxHalfCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_RxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_TxRxHalfCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_Error);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef* hspi)
    {
        Nilai::Events::SpiEvent e(hspi, Nilai::Events::EventTypes::SPI_AbortCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }
}

#    endif
#endif
