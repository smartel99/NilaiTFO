/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  uart
 * @{
 * @file        uartModule.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/13  -  09:25
 *
 * @brief       UART communication module
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"
#include "shared/defines/misc.hpp"
#include "shared/defines/module.hpp"
#include "shared/services/ticks.hpp"

#ifdef HAL_UART_MODULE_ENABLED
#include "Core/Inc/usart.h"
#endif


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define UART1_MODULE (UART::UARTModule::GetInstance(0))
#define UART2_MODULE (UART::UARTModule::GetInstance(1))
#define UART3_MODULE (UART::UARTModule::GetInstance(2))
#define UART4_MODULE (UART::UARTModule::GetInstance(3))


namespace UART
{


/*************************************************************************************************/
/* Enumerated Types ---------------------------------------------------------------------------- */

#pragma region Status
/**
 * @addtogroup  UART_Status
 * @brief       UART module status, mostly describing error states.
 */
enum class Status
{
    OK                 = 0x0000,    //!< UART_OK.
    NOT_ENABLED        = 0x0001,    //!< UART_NOT_ENABLED.
    TIMEOUT            = 0x0002,    //!< UART_TIMEOUT.
    BUSY               = 0x0004,    //!< UART_BUSY.
    ERROR              = 0x0008,    //!< UART_ERROR.
    DROPPED_BYTE       = 0x0010,    //!< UART_DROPPED_BYTE.
    RX_BUFF_OVERFLOW   = 0x0020,    //!< UART_RX_BUFF_OVERFLOW.
    NOT_DONE_RECEIVING = 0x0040,    //!< UART_NOT_DONE_RECEIVING.
    BAD_START_OF_FRAME = 0x0080,    //!< UART_BAD_START_OF_FRAME.
};

/** From: https://stackoverflow.com/a/15889501 */
[[nodiscard]] constexpr inline Status operator|(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                               static_cast<std::underlying_type_t<Status>>(b));
}
[[nodiscard]] constexpr inline Status operator&(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) noexcept
{
    return a = a | b;
}
/**
 * @}
 */
#pragma endregion


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */

class Sequence
{
private:
    bool        m_useSof = false;
    bool        m_useEof = false;
    std::string m_startOfFrame;
    std::string m_endOfFrame;

public:
    ALWAYS_INLINE void SetSOF(std::string_view startOfFrameSequence)
    {
        m_startOfFrame = startOfFrameSequence;
        m_useSof       = true;
    }
    ALWAYS_INLINE void SetEOF(std::string_view endOfFrameSequence)
    {
        m_endOfFrame = endOfFrameSequence;
        m_useEof     = true;
    }
};

class UARTModule : public cep::Module
{
private:
    friend class Sequence;
    using TxPacket = std::vector<std::uint8_t>;
    using RxPacket = std::vector<std::uint8_t>;

    UART_HandleTypeDef*                        m_handle = nullptr;
    Status                                     m_status = Status::OK;
    std::vector<RxPacket>                      m_rxBuffer;
    std::vector<TxPacket>                      m_txBuffer;
    std::vector<std::function<void(RxPacket)>> m_callbacks;

public:
    UARTModule(UART_HandleTypeDef* handle, std::string_view name)
    : cep::Module(name), m_handle{handle}
    {
    }
};

/*************************************************************************************************/
}; /* namespace UART */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
