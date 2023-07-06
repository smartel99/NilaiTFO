#pragma once
#include <type_traits>

namespace Nilai::Drivers::Uart {
/**
 * @addtogroup  UART_Status
 * @brief       UART module status, mostly describing error states.
 */
    enum class Status {
        Ok = 0x0000,
        //!< UART_OK.
        NotEnabled = 0x0001,
        //!< UART_NOT_ENABLED.
        Timeout = 0x0002,
        //!< UART_TIMEOUT.
        Busy = 0x0004,
        //!< UART_BUSY.
        Error = 0x0008,
        //!< UART_ERROR.
        DroppedByte = 0x0010,
        //!< UART_DROPPED_BYTE.
        RxBuffOverflow = 0x0020,
        //!< UART_RX_BUFF_OVERFLOW.
        NotDoneReceiving = 0x0040,
        //!< UART_NOT_DONE_RECEIVING.
        BadStartOfFrame = 0x0080,
        //!< UART_BAD_START_OF_FRAME.
    };


/** From: https://stackoverflow.com/a/15889501 */
    constexpr inline Status operator|(Status a, Status b) {
        return static_cast<Status>(static_cast<std::underlying_type_t <Status>>(a) |
                                   static_cast<std::underlying_type_t <Status>>(b));
    }

    constexpr inline Status operator&(Status a, Status b) {
        return static_cast<Status>(static_cast<std::underlying_type_t <Status>>(a) &
                                   static_cast<std::underlying_type_t <Status>>(b));
    }

    constexpr inline Status operator|=(Status &a, const Status &b) {
        return a = a | b;
    }
}