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
#ifndef NILAI_UART_STRUCTS_H
#define NILAI_UART_STRUCTS_H

#if defined(NILAI_USE_UART)
#    if defined(NILAI_TEST)
#    else
#        include "../../defines/internal_config.h"
#        include NILAI_HAL_HEADER


#        include <cstdint>
#        include <string>
#        include <vector>

namespace Nilai::Uart
{

struct Frame
{
    std::vector<uint8_t> data;
    size_t               len       = 0;
    uint32_t             timestamp = 0;

    Frame() = default;
    Frame(const std::vector<uint8_t>& d, uint32_t t) : timestamp(t)
    {
        data = d;
        len  = data.size();
    }

    [[nodiscard]] std::string ToStr() const { return std::string {(char*)data.data()}; }
    bool                      operator==(const std::string& s) const
    {
        return (std::string((char*)data.data(), len) == s);
    }
    bool operator!=(const std::string& s) const { return !operator==(s); }
};

struct UartDataBuffer
{
    bool                 rcvCompleted = false;
    bool                 txCompleted  = true;
    UART_HandleTypeDef*  instance     = nullptr;
    std::vector<uint8_t> rxDmaData;

    UartDataBuffer(size_t len, UART_HandleTypeDef* ins) : instance(ins) { rxDmaData.resize(len); }
};
}    // namespace Nilai::Uart

#    endif
#endif

#endif    // NILAI_UART_STRUCTS_H
