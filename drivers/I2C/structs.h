/**
 * @file    structs.h
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
#ifndef NILAI_DRIVERS_I2C_STRUCTS_H
#define NILAI_DRIVERS_I2C_STRUCTS_H

#if defined(NILAI_USE_I2C)

#    include <cstdint>
#    include <vector>

namespace Nilai::I2C
{
/**
 * I2C Frame to be sent or received.
 */
struct Frame
{
    //! 8-bit I2C Address of the device.
    uint8_t deviceAddress = 0;
    //! Optional, address of the register to write to/read from.
    uint8_t registerAddress = 0;
    //! Data that was exchanged with the device.
    std::vector<uint8_t> data = {};

    Frame() = default;
    Frame(uint8_t devAddr, std::vector<uint8_t> pData = std::vector<uint8_t>())
    : deviceAddress(devAddr), data(std::move(pData))
    {
    }
    Frame(uint8_t devAddr, uint8_t regAddr, std::vector<uint8_t> pData = std::vector<uint8_t>())
    : deviceAddress(devAddr), registerAddress(regAddr), data(std::move(pData))
    {
    }
};
}    // namespace Nilai::I2C
#endif

#endif    // NILAI_DRIVERS_I2C_STRUCTS_H
