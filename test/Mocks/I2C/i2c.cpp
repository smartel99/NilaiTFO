/**
 * @file    i2c.cpp
 * @author  Samuel Martel
 * @date    2022-10-14
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
#include "i2c.h"



using Nilai::Test::Internal::I2CDevice;

size_t                                          s_i2cBuffersIdCount = 0;
std::map<size_t, std::map<uint16_t, I2CDevice>> s_i2cDevices;

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef*        i2c,
                                          uint16_t                  devAddr,
                                          const uint8_t*            data,
                                          uint16_t                  size,
                                          [[maybe_unused]] uint32_t timeout)
{
    if (HAL_I2C_IsDeviceReady(i2c, devAddr) == HAL_OK)
    {
        s_i2cDevices[i2c->id].at(devAddr).TxToDev(data, size);
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef*        hi2c,
                                         uint16_t                  DevAddress,
                                         uint8_t*                  pData,
                                         uint16_t                  Size,
                                         [[maybe_unused]] uint32_t Timeout)
{
    if (HAL_I2C_IsDeviceReady(hi2c, DevAddress) == HAL_OK)
    {
        s_i2cDevices[hi2c->id].at(DevAddress).RxFromDev(pData, Size);
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef*        hi2c,
                                    uint16_t                  DevAddress,
                                    uint16_t                  MemAddress,
                                    [[maybe_unused]] uint16_t MemAddSize,
                                    const uint8_t*            pData,
                                    uint16_t                  Size,
                                    [[maybe_unused]] uint32_t Timeout)
{
    if (HAL_I2C_IsDeviceReady(hi2c, DevAddress) == HAL_OK)
    {
        s_i2cDevices[hi2c->id].at(DevAddress).TxToRegister(MemAddress, pData, Size);
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef*        hi2c,
                                   uint16_t                  DevAddress,
                                   uint16_t                  MemAddress,
                                   [[maybe_unused]] uint16_t MemAddSize,
                                   uint8_t*                  pData,
                                   uint16_t                  Size,
                                   [[maybe_unused]] uint32_t Timeout)
{
    if (HAL_I2C_IsDeviceReady(hi2c, DevAddress) == HAL_OK)
    {
        s_i2cDevices[hi2c->id].at(DevAddress).RxFromRegister(MemAddress, pData, Size);
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef*        hi2c,
                                        uint16_t                  DevAddress,
                                        [[maybe_unused]] uint32_t Trials,
                                        [[maybe_unused]] uint32_t Timeout)
{
    if (s_i2cDevices[hi2c->id].contains(DevAddress) &&
        s_i2cDevices[hi2c->id].at(DevAddress).IsReady)
    {
        return HAL_OK;
    }
    return HAL_TIMEOUT;
}

void Nilai_I2C_Init(I2C_HandleTypeDef* i2c)
{
    s_i2cDevices[s_i2cBuffersIdCount] = {};

    i2c->id = s_i2cBuffersIdCount++;
}

HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef* i2c)
{
    s_i2cDevices.erase(i2c->id);

    return HAL_OK;
}
