/**
 * @file    enums.h
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
#ifndef NILAI_UART_ENUMS_H
#define NILAI_UART_ENUMS_H

#if defined(NILAI_USE_UART)
#    include "../../defines/internal_config.h"
#    include NILAI_HAL_HEADER

#    include <type_traits>

namespace Nilai::Uart
{
/**
 * @addtogroup  UART_Status
 * @brief       UART module status, mostly describing error states.
 */
enum class Status
{
    //! UART_OK.
    Ok = 0x0000,
    //! UART_NOT_ENABLED.
    NotEnabled = 0x0001,
    //! UART_TIMEOUT.
    Timeout = 0x0002,
    //! UART_BUSY.
    Busy = 0x0004,
    //! UART_ERROR.
    Error = 0x0008,
    //! UART_DROPPED_BYTE.
    DroppedByte = 0x0010,
    //! UART_RX_BUFF_OVERFLOW.
    RxBuffOverflow = 0x0020,
    //! UART_NOT_DONE_RECEIVING.
    NotDoneReceiving = 0x0040,
    //! UART_BAD_START_OF_FRAME.
    BadStartOfFrame = 0x0080,
};

/** From: https://stackoverflow.com/a/15889501 */
constexpr inline Status operator|(Status a, Status b)
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator&(Status a, Status b)
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b)
{
    return a = a | b;
}

enum class SectionState
{
    NotComplete,
    Complete,
};
}    // namespace Nilai::Uart

#endif

#endif    // NILAI_UART_ENUMS_H
