/**
 * @file    I2c.cpp
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
#    if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_I2C_EVENTS)
#        include "../../defines/events/events.h"
#        include "../../processes/application.h"

#        include NILAI_HAL_HEADER

extern "C"
{
    void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_MasterTxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_MasterRxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_SlaveTxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_SlaveRxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c,
                              uint8_t            TransferDirection,
                              uint16_t           AddrMatchCode)
    {
        Nilai::Events::I2cAddrEvent e(hi2c, TransferDirection, AddrMatchCode);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_ListenCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_MemTxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_MemRxCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_Error);
        Nilai::Application::Get()->DispatchEvent(&e);
    }

    void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef* hi2c)
    {
        Nilai::Events::I2cEvent e(hi2c, Nilai::Events::EventTypes::I2C_AbortCplt);
        Nilai::Application::Get()->DispatchEvent(&e);
    }
}
#    endif
#endif