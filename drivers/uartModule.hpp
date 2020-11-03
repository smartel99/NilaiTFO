/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  uart
 * @{
 * @file        uartModule.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/08/13  -  09:25
 *
 * @brief       UART communication module
 */
#ifndef UART_MODULE_HPP_
#    define UART_MODULE_HPP_
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#    if defined(NILAI_USE_UART)
#        include "stm32f4xx_hal.h"
#        if defined(HAL_UART_MODULE_ENABLED)
#            include "defines/macros.hpp"
#            include "defines/misc.hpp"
#            include "defines/module.hpp"

#            include <cstdint>       // For uint8_t, size_t
#            include <functional>    // For std::function
#            include <string>        // For std::string
#            include <vector>        // For std::vector

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Enumerated Types ---------------------------------------------------------------------------- */
namespace UART
{
/**
 * @addtogroup  UART_Status
 * @brief       UART module status, mostly describing error states.
 */
enum class Status
{
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
constexpr inline Status operator|=(Status& a, const Status& b) { return a = a | b; }
/**
 * @}
 */

enum class SectionState
{
    NotComplete,
    Complete,
};

struct Frame
{
    // std::vector<uint8_t> data;
    uint8_t  data[515] = {0};
    uint32_t timestamp = 0;

    Frame( ) = default;
    Frame(const std::vector<uint8_t>& d, uint32_t t) : timestamp(t)
    {
        //        volatile size_t s = d.size( );
        //        data.reserve(s);
        for (size_t i = 0; i < d.size( ); i++)
        {
            data[i] = d[i];
            //            data.push_back(v);
        }
    }
};
}    // namespace UART

/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class UartModule : public cep::Module
{
public:
    UartModule(UART_HandleTypeDef* uart, const std::string& label) : m_handle(uart), m_label(label)
    {
        CEP_ASSERT(uart != nullptr, "UART Handle is NULL!");
        m_txBuf.reserve(64);
        m_rxBuf.reserve(512);
        m_sof.reserve(2);
        m_eof.reserve(2);
        //        m_latestFrames.reserve(8);

        __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);
        LOG_INFO("[UART] %s Initialized!", label.c_str( ));
    }
    virtual ~UartModule( ) override;

    virtual void Run( ) override;

    virtual const std::string& GetLabel( ) const override { return m_label; }

    void Transmit(const char* msg, size_t len);
    void Transmit(const std::string& msg);
    void Transmit(const std::vector<uint8_t>& msg);

    size_t GetNumberOfWaitingFrames( ) const
    {
        return (m_framePending ? 1 : 0);
        // return m_latestFrames.size( );
    }
    UART::Frame Receive( );

    void SetExpectedRxLen(size_t len);
    void ClearExpectedRxLen( );

    void SetFrameReceiveCpltCallback(const std::function<void( )>& cb);
    void ClearFrameReceiveCpltCallback( );

    void SetStartOfFrameSequence(uint8_t* sof, size_t len);
    void SetStartOfFrameSequence(const std::string& sof);
    void SetStartOfFrameSequence(const std::vector<uint8_t>& sof);
    void ClearStartOfFrameSequence( );

    void SetEndOfFrameSequence(uint8_t* eof, size_t len);
    void SetEndOfFrameSequence(const std::string& eof);
    void SetEndOfFrameSequence(const std::vector<uint8_t>& eof);
    void ClearEndOfFrameSequence( );

    void HandleReceptionIRQ( );

private:
    bool WaitUntilTransmitionComplete( );

private:
    UART_HandleTypeDef* m_handle = nullptr;
    std::string         m_label  = "";

    UART::Status         m_status           = UART::Status::Ok;
    size_t               m_txBytesRemaining = -1;
    std::vector<uint8_t> m_txBuf;
    std::vector<uint8_t> m_rxBuf;
    size_t               m_expectedLen               = 0;
    uint32_t             m_lastCharReceivedTimestamp = 0;
    //    std::vector<UART::Frame> m_latestFrames;
    UART::Frame m_latestFrames;
    bool        m_framePending = false;

    std::string m_sof;
    std::string m_eof;

    std::function<void( )> m_cb;

    static constexpr uint32_t TIMEOUT    = 100;    // Systicks.
    static constexpr uint32_t RX_TIMEOUT = 50;     // Systicks.
};
#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO UART module enabled, but HAL_UART_MODULE_ENABLE is not defined!
#            endif
#        endif
#    endif
#endif

/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
