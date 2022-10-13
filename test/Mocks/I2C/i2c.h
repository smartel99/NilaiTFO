/**
 * @file    i2c.h
 * @author  Samuel Martel
 * @date    2022-10-13
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

#ifndef GUARD_NILAI_TEST_MOCKS_I2C_H
#define GUARD_NILAI_TEST_MOCKS_I2C_H

extern "C"
{
    struct I2C_HandleTypeDef
    {
        uint32_t data;
    };

    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef* i2c,
                                              uint16_t           devAddr,
                                              const uint8_t*     data,
                                              uint16_t           size,
                                              uint32_t           timeout);
    HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef* hi2c,
                                             uint16_t           DevAddress,
                                             uint8_t*           pData,
                                             uint16_t           Size,
                                             uint32_t           Timeout);

    HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef* hi2c,
                                        uint16_t           DevAddress,
                                        uint16_t           MemAddress,
                                        uint16_t           MemAddSize,
                                        const uint8_t*     pData,
                                        uint16_t           Size,
                                        uint32_t           Timeout);
    HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef* hi2c,
                                       uint16_t           DevAddress,
                                       uint16_t           MemAddress,
                                       uint16_t           MemAddSize,
                                       uint8_t*           pData,
                                       uint16_t           Size,
                                       uint32_t           Timeout);

    HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef* hi2c,
                                            uint16_t           DevAddress,
                                            uint32_t           Trials,
                                            uint32_t           Timeout);
}

#endif    // GUARD_NILAI_TEST_MOCKS_I2C_H
