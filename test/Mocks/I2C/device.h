/**
 * @file    device.h
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

#ifndef GUARD_NILAI_TEST_MOCK_I2C_DEVICE_H
#define GUARD_NILAI_TEST_MOCK_I2C_DEVICE_H

#include "../generic_stm32.h"

#include <cstdint>
#include <map>

namespace Nilai::Test::Internal
{

using I2CIoBuffer = IoBuffer<uint8_t>;

struct I2CRegisterDef
{
    uint16_t Addr;
    size_t   TxLen;
    size_t   RxLen;
};

struct I2CDevice
{
    bool IsReady = true;

    std::map<uint16_t, I2CIoBuffer> Registers;
    static constexpr uint16_t       s_masterRegisterId = 0xFFFF;

    template<typename... Rs>
    I2CDevice(size_t txLen, size_t rxLen, const Rs&... registers)
        requires(((std::same_as<Rs, I2CRegisterDef> ||
                   std::constructible_from<I2CRegisterDef, Rs>)&&...))
    {
        ((Registers[registers.Addr] = I2CIoBuffer(registers.TxLen, registers.RxLen)), ...);
        Registers[s_masterRegisterId] = I2CIoBuffer(txLen, rxLen);
    }

    void TxToDev(const uint8_t* data, uint16_t size)
    {
        return TxToRegister(s_masterRegisterId, data, size);
    }
    void TxToRegister(uint16_t regAddr, const uint8_t* data, uint16_t size)
    {
        Registers[regAddr].tx.Push(data, size);
    }

    size_t RxFromDev(uint8_t* data, uint16_t len)
    {
        return RxFromRegister(s_masterRegisterId, data, len);
    }
    size_t RxFromRegister(uint16_t regAddr, uint8_t* data, uint16_t len)
    {
        return Registers[regAddr].rx.Read(data, len);
    }

    I2CIoBuffer::value_type& NilaiGetDevTxBuffer()
    {
        return NilaiGetRegisterTxBuffer(s_masterRegisterId);
    }
    I2CIoBuffer::value_type& NilaiGetRegisterTxBuffer(uint16_t regAddr)
    {
        return Registers[regAddr].rx;
    }
    I2CIoBuffer::value_type& NilaiGetDevRxBuffer()
    {
        return NilaiGetRegisterTxBuffer(s_masterRegisterId);
    }
    I2CIoBuffer::value_type& NilaiGetRegisterRxBuffer(uint16_t regAddr)
    {
        return Registers[regAddr].rx;
    }
};
}    // namespace Nilai::Test::Internal
#endif    // GUARD_NILAI_TEST_MOCK_I2C_DEVICE_H
