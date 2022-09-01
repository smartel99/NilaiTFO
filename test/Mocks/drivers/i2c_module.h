/**
 * @file    i2c_module.h
 * @author  Samuel Martel
 * @date    2022-05-19
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

#ifndef NILAI_MOCKS_I2C_MODULE_H
#define NILAI_MOCKS_I2C_MODULE_H

#if defined(NILAI_TEST)
#    include <limits>
#    include <vector>

#    include "../../../drivers/I2C/structs.h"

#    include "handle.h"


namespace Nilai::Test::Drivers
{
class I2cMockHandle : public Handle<Nilai::I2C::Frame>
{
public:
    void               ReadyDevice(uint8_t addr) noexcept;
    void               UnreadyDevice(uint8_t addr) noexcept;
    [[nodiscard]] bool IsReady(uint8_t addr) const noexcept;

protected:
    std::vector<uint8_t> m_readyDevices = {};
};
}    // namespace Nilai::Test::Drivers

static constexpr const size_t HAL_OK    = 0;
static constexpr const size_t HAL_ERROR = std::numeric_limits<size_t>::max();

extern "C" size_t HAL_I2C_Master_Transmit(Nilai::Test::Drivers::I2cMockHandle* handle,
                                          uint8_t                              addr,
                                          const uint8_t*                       data,
                                          uint16_t                             len,
                                          [[maybe_unused]] size_t              timeout);

extern "C" size_t HAL_I2C_Mem_Write(Nilai::Test::Drivers::I2cMockHandle* handle,
                                    uint8_t                              addr,
                                    uint8_t                              regAddr,
                                    uint8_t                              regAddrLen,
                                    const uint8_t*                       data,
                                    uint16_t                             len,
                                    size_t                               timeout);

extern "C" size_t HAL_I2C_Master_Receive(Nilai::Test::Drivers::I2cMockHandle* handle,
                                         uint8_t                              addr,
                                         uint8_t*                             data,
                                         uint16_t                             len,
                                         size_t                               timeout);

extern "C" size_t HAL_I2C_Mem_Read(Nilai::Test::Drivers::I2cMockHandle* handle,
                                   uint8_t                              devAddr,
                                   uint8_t                              regAddr,
                                   size_t                               regAddrLen,
                                   uint8_t*                             data,
                                   uint16_t                             dataLen,
                                   size_t                               timeout);

extern "C" size_t HAL_I2C_IsDeviceReady(Nilai::Test::Drivers::I2cMockHandle* handle,
                                        uint8_t                              addr,
                                        uint8_t                              attempts,
                                        uint8_t                              timeout);


#endif
#endif    // NILAI_MOCKS_I2C_MODULE_H
