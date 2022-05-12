/**
 * @file    Enums.h
 * @author  Samuel Martel
 * @date    2022-04-19
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
#ifndef NILAI_CAN_ENUMS_H
#define NILAI_CAN_ENUMS_H

#if defined(NILAI_USE_CAN)
#    include <type_traits>

namespace Nilai::Can
{
/**
 * @addtogroup  CAN_Status
 * @brief       Enum listing all the possible CAN status
 * @{
 */
enum class Status
{
    //! No error.
    ERROR_NONE = 0x00000000U,
    //! Protocol Error Warning.
    ERROR_EWG = 0x00000001U,
    //! Error Passive.
    ERROR_EPV = 0x00000002U,
    //! Bus-off error.
    ERROR_BOF = 0x00000004U,
    //! Stuff error.
    ERROR_STF = 0x00000008U,
    //! Form error.
    ERROR_FOR = 0x00000010U,
    //! Acknowledgment error.
    ERROR_ACK = 0x00000020U,
    //! Bit recessive error.
    ERROR_BR = 0x00000040U,
    //! Bit dominant error.
    ERROR_BD = 0x00000080U,
    //! CRC error.
    ERROR_CRC = 0x00000100U,

    //! Rx FIFO0 overrun error.
    ERROR_RX_FOV0 = 0x00000200U,
    //! Rx FIFO1 overrun error.
    ERROR_RX_FOV1 = 0x00000400U,

    //! TxMailbox 0 transmit failure due to arbitration lost.
    ERROR_TX_ALST0 = 0x00000800U,
    //! TxMailbox 0 transmit failure due to transmit error.
    ERROR_TX_TERR0 = 0x00001000U,
    //! TxMailbox 1 transmit failure due to arbitration lost.
    ERROR_TX_ALST1 = 0x00002000U,
    //! TxMailbox 1 transmit failure due to transmit error.
    ERROR_TX_TERR1 = 0x00004000U,
    //! TxMailbox 2 transmit failure due to arbitration lost.
    ERROR_TX_ALST2 = 0x00008000U,
    //! TxMailbox 2 transmit failure due to transmit error.
    ERROR_TX_TERR2 = 0x00010000U,

    //! Timeout error.
    ERROR_TIMEOUT = 0x00020000U,
    //! Peripheral not initialized.
    ERROR_NOT_INIT = 0x00040000U,
    //! Peripheral not ready.
    ERROR_NOT_READY = 0x00080000U,
    //! Peripheral not started.
    ERROR_NOT_STARTED = 0x00100000U,
    //! Parameter error.
    ERROR_PARAM = 0x00200000U,

#    if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    //! Invalid Callback error */
    ERROR_INVALID_CALLBACK = 0x00400000U,
#    endif

    //! Internal error.
    ERROR_INTERNAL = 0x00800000U,
    //! Packet received before last one has been read.
    ERROR_DROPPED_PKT = 0x01000000U,
    //! A packet has been read.
    RCVD_NEW_PACKET = 0x02000000U,
    //! A packet has been read.
    RCVD_OLD_PACKET = 0x04000000U,
    //! No packet has been received.
    NO_PACKET_RECEIVED = 0x08000000U,
    //! The transmission was aborted.
    PKT_ABORTED = 0x10000000U,
    //! An error occurred during transmission.
    TX_ERROR = 0x20000000U,
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

[[maybe_unused]] static constexpr const char* StatusToStr(Status s)
{
#    if defined(NILAI_CAN_STATUS_STRING)
    switch (s)
    {
        case Status::ERROR_NONE: return "No error";
        case Status::ERROR_EWG: return "Protocol Error Warning";
        case Status::ERROR_EPV: return "Error Passive";
        case Status::ERROR_BOF: return "Bus-off Error";
        case Status::ERROR_STF: return "Stuff Error";
        case Status::ERROR_FOR: return "Form Error";
        case Status::ERROR_ACK: return "Acknowledgment Error";
        case Status::ERROR_BR: return "Bit Recessive Error";
        case Status::ERROR_BD: return "Bit Dominant Error";
        case Status::ERROR_CRC: return "CRC Error";
        case Status::ERROR_RX_FOV0: return "Rx FIFO0 Overrun Error";
        case Status::ERROR_RX_FOV1: return "Rx FIFO1 Overrun Error";
        case Status::ERROR_TX_ALST0: return "TxMailbox 0 transmit failure due to arbitration lost";
        case Status::ERROR_TX_TERR0: return "TxMailbox 0 transmit failure due to transmit error";
        case Status::ERROR_TX_ALST1: return "TxMailbox 1 transmit failure due to arbitration lost";
        case Status::ERROR_TX_TERR1: return "TxMailbox 1 transmit failure due to transmit error";
        case Status::ERROR_TX_ALST2: return "TxMailbox 2 transmit failure due to arbitration lost";
        case Status::ERROR_TX_TERR2: return "TxMailbox 1 transmit failure due to transmit error";
        case Status::ERROR_TIMEOUT: return "Timeout Error";
        case Status::ERROR_NOT_INIT: return "Peripheral Not Initialized";
        case Status::ERROR_NOT_READY: return "Peripheral Not Ready";
        case Status::ERROR_NOT_STARTED: return "Peripheral Not Started";
        case Status::ERROR_PARAM: return "Parameter Error";
        case Status::ERROR_INTERNAL: return "Internal Error";
        case Status::ERROR_DROPPED_PKT: return "Packet received before last one has been read";
        case Status::RCVD_NEW_PACKET: return "A new packet has been read";
        case Status::RCVD_OLD_PACKET: return "An old packet has been read";
        case Status::NO_PACKET_RECEIVED: return "No packet has been received";
        case Status::PKT_ABORTED: return "The transmission was aborted";
        case Status::TX_ERROR: return "An error occurred during transmission";
        default: return "UNKNOWN";
    }
#    else
    NILAI_UNUSED(s);
    return "";
#    endif
}
/**
 * @}
 */

#    if defined(NILAI_TEST)
#        ifndef CAN_IT_TX_MAILBOX_EMPTY
#            define CAN_IT_TX_MAILBOX_EMPTY 0
#        endif

#        ifndef CAN_IT_RX_FIFO0_MSG_PENDING
#            define CAN_IT_RX_FIFO0_MSG_PENDING 1
#        endif

#        ifndef CAN_IT_RX_FIFO0_FULL
#            define CAN_IT_RX_FIFO0_FULL 2
#        endif

#        ifndef CAN_IT_RX_FIFO0_OVERRUN
#            define CAN_IT_RX_FIFO0_OVERRUN 3
#        endif

#        ifndef CAN_IT_RX_FIFO1_MSG_PENDING
#            define CAN_IT_RX_FIFO1_MSG_PENDING 4
#        endif

#        ifndef CAN_IT_RX_FIFO1_FULL
#            define CAN_IT_RX_FIFO1_FULL 5
#        endif

#        ifndef CAN_IT_RX_FIFO1_OVERRUN
#            define CAN_IT_RX_FIFO1_OVERRUN 6
#        endif

#        ifndef CAN_IT_WAKEUP
#            define CAN_IT_WAKEUP 7
#        endif

#        ifndef CAN_IT_SLEEP_ACK
#            define CAN_IT_SLEEP_ACK 8
#        endif

#        ifndef CAN_IT_ERROR_WARNING
#            define CAN_IT_ERROR_WARNING 9
#        endif

#        ifndef CAN_IT_ERROR_PASSIVE
#            define CAN_IT_ERROR_PASSIVE 10
#        endif

#        ifndef CAN_IT_BUSOFF
#            define CAN_IT_BUSOFF 11
#        endif

#        ifndef CAN_IT_LAST_ERROR_CODE
#            define CAN_IT_LAST_ERROR_CODE 12
#        endif

#        ifndef CAN_IT_ERROR
#            define CAN_IT_ERROR 13
#        endif
#    endif

enum class Irq
{
    TxMailboxEmpty      = CAN_IT_TX_MAILBOX_EMPTY,
    Fifo0MessagePending = CAN_IT_RX_FIFO0_MSG_PENDING,
    Fifo0Full           = CAN_IT_RX_FIFO0_FULL,
    Fifo0Overrun        = CAN_IT_RX_FIFO0_OVERRUN,
    Fifo1MessagePending = CAN_IT_RX_FIFO1_MSG_PENDING,
    Fifo1Full           = CAN_IT_RX_FIFO1_FULL,
    Fifo1Overrun        = CAN_IT_RX_FIFO1_OVERRUN,
    Wakeup              = CAN_IT_WAKEUP,
    SleepAck            = CAN_IT_SLEEP_ACK,
    ErrorWarning        = CAN_IT_ERROR_WARNING,
    ErrorPassive        = CAN_IT_ERROR_PASSIVE,
    BusOffError         = CAN_IT_BUSOFF,
    LastErrorCode       = CAN_IT_LAST_ERROR_CODE,
    ErrorStatus         = CAN_IT_ERROR,
};
}    // namespace Nilai::Can
#endif
#endif    // NILAI_CAN_ENUMS_H
