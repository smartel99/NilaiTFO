/**
 * @file    i2c_module.cpp
 * @author  Samuel Martel
 * @date    2022-08-16
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
#include "i2c_module.h"

namespace Nilai::Test::Drivers
{
void Nilai::Test::Drivers::I2cMockHandle::ReadyDevice(uint8_t addr) noexcept
{
    // If address not already in the list, add it.
    if (std::find(m_readyDevices.begin(), m_readyDevices.end(), addr) == m_readyDevices.end())
    {
        m_readyDevices.push_back(addr);
    }
}

void Nilai::Test::Drivers::I2cMockHandle::UnreadyDevice(uint8_t addr) noexcept
{
    // If address in the list, remove it.
    auto it = std::find(m_readyDevices.begin(), m_readyDevices.end(), addr);
    if (it != m_readyDevices.end())
    {
        m_readyDevices.erase(it);
    }
}

bool Nilai::Test::Drivers::I2cMockHandle::IsReady(uint8_t addr) const noexcept
{
    return std::find(m_readyDevices.begin(), m_readyDevices.end(), addr) != m_readyDevices.end();
}
}    // namespace Nilai::Test::Drivers

extern "C" size_t HAL_I2C_Master_Transmit(Nilai::Test::Drivers::I2cMockHandle* handle,
                                          uint8_t                              addr,
                                          const uint8_t*                       data,
                                          uint16_t                             len,
                                          [[maybe_unused]] size_t              timeout)
{
    std::vector<uint8_t> d = std::vector<uint8_t>(len);
    for (size_t i = 0; i < len; i++)
    {
        d[i] = data[i];
    }

    handle->QueueTxFrame(addr, d);

    return HAL_OK;
}

size_t HAL_I2C_Mem_Write(Nilai::Test::Drivers::I2cMockHandle* handle,
                         uint8_t                              addr,
                         uint8_t                              regAddr,
                         [[maybe_unused]] uint8_t             regAddrLen,
                         const uint8_t*                       data,
                         uint16_t                             len,
                         [[maybe_unused]] size_t              timeout)
{
    std::vector<uint8_t> d = std::vector<uint8_t>(len);
    for (size_t i = 0; i < len; i++)
    {
        d[i] = data[i];
    }

    handle->QueueTxFrame(addr, regAddr, d);

    return HAL_OK;
}

size_t HAL_I2C_Master_Receive(Nilai::Test::Drivers::I2cMockHandle* handle,
                              uint8_t                              addr,
                              uint8_t*                             data,
                              uint16_t                             len,
                              [[maybe_unused]] size_t              timeout)
{
    auto frame = handle->GetRxFrame(addr);

    if (!frame || (frame->data.size() != len))
    {
        // Frame not found in buffer.
        return HAL_ERROR;
    }

    for (size_t i = 0; i < len; i++)
    {
        data[i] = frame->data[i];
    }

    return HAL_OK;
}

size_t HAL_I2C_Mem_Read(Nilai::Test::Drivers::I2cMockHandle* handle,
                        uint8_t                              devAddr,
                        uint8_t                              regAddr,
                        [[maybe_unused]] size_t              regAddrLen,
                        uint8_t*                             data,
                        uint16_t                             dataLen,
                        [[maybe_unused]] size_t              timeout)
{
    auto frame = handle->GetRxFrame(devAddr, regAddr);

    if (!frame || (frame->data.size() != dataLen))
    {
        // Frame not found in buffer.
        return HAL_ERROR;
    }

    for (size_t i = 0; i < dataLen; i++)
    {
        data[i] = frame->data[i];
    }

    return HAL_OK;
}

size_t HAL_I2C_IsDeviceReady(Nilai::Test::Drivers::I2cMockHandle* handle,
                             uint8_t                              addr,
                             [[maybe_unused]] uint8_t             attempts,
                             [[maybe_unused]] uint8_t             timeout)
{
    return static_cast<size_t>(handle->IsReady(addr));
}
