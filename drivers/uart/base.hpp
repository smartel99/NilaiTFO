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
#ifndef GUARD_UARTMODULE_HPP
#define GUARD_UARTMODULE_HPP
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#if defined(NILAI_USE_UART)
#include "defines/internalConfig.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include NILAI_HAL_HEADER
#pragma GCC diagnostic pop
#if defined(HAL_UART_MODULE_ENABLED)

#include "defines/circular_buffer.hpp"
#include "defines/macros.hpp"
#include "defines/misc.hpp"
#include "defines/module.hpp"

#include <algorithm>
#include <cstdint>       // For uint8_t, size_t
#include <functional>    // For std::function
#include <string>        // For std::string
#include <vector>        // For std::vector

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Enumerated Types ---------------------------------------------------------------------------- */
namespace CEP_UART {
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
constexpr inline Status operator|(Status a, Status b) {
    return static_cast<Status>(
      static_cast<std::underlying_type_t<Status>>(a) | static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator&(Status a, Status b) {
    return static_cast<Status>(
      static_cast<std::underlying_type_t<Status>>(a) & static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) {
    return a = a | b;
}
/**
 * @}
 */

enum class SectionState
{
    NotComplete,
    Complete,
};

struct Frame {
#if 0
    uint8_t* data      = nullptr;
    size_t   len       = 0;
#endif
    std::vector<uint8_t> data;
    size_t               len       = 0;
    uint32_t             timestamp = 0;

    Frame() = default;
#if 0
    Frame(const std::vector<uint8_t>& d, uint32_t t) : timestamp(t)
    {
        data = new uint8_t[d.size()];
        CEP_ASSERT(data != nullptr, "Unable to allocate memory for data!");
        len = d.size();
        for (size_t i = 0; i < d.size(); i++)
        {
            data[i] = d[i];
        }
    }
    ~Frame()
    {
        delete[] data;
        data = nullptr;
        len  = 0;
    }
#endif
    Frame(uint8_t* buf, size_t len, uint32_t t)
    : timestamp(t) {
        //        std::copy_n(buf, len, data); // Cannot be used, no iterator for uint8_t, somehow
        for (size_t i = 0; i < len; ++i)
            data.push_back(buf[i]);
        this->len = len;
    }

    Frame(const std::vector<uint8_t>& d, uint32_t t)
    : timestamp(t) {
        data = d;
        len  = data.size();
    }

    [[nodiscard]] std::string ToStr() const {
        return std::string((char*)data.data(), len);
    }
    bool operator==(const std::string& s) const {
        return (std::string((char*)data.data(), len) == s);
    }
    bool operator!=(const std::string& s) const {
        return (std::string((char*)data.data(), len) != s);
    }
};

// DMA Only
struct UartDataBuffer {
    bool                 rcvCompleted = false;
    bool                 txCompleted  = true;
    UART_HandleTypeDef*  instance     = nullptr;
    std::vector<uint8_t> rxDmaData;

    UartDataBuffer(size_t len, UART_HandleTypeDef* ins)
    : instance(ins) {
        rxDmaData.resize(len);
    }
};

enum class Kind
{
    IT,
    DMA
};

}    // namespace CEP_UART

/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class UartModule : public cep::Module {
  public:
    UartModule(UART_HandleTypeDef* uart, const std::string& label)
    : m_handle(uart)
    , m_label(label) {
    }
    ~UartModule();

    bool DoPost() override;

    [[nodiscard]] const std::string& GetLabel() const override {
        return m_label;
    }

    void                  Transmit(const char* msg, size_t len);
    void                  Transmit(const std::string& msg);
    void                  Transmit(const std::vector<uint8_t>& msg);
    [[maybe_unused]] void VTransmit(const char* fmt, ...);

    [[nodiscard]] size_t GetNumberOfWaitingFrames() const {
        return (m_framePending ? 1 : 0);
        //    	    return m_latestFrames.size();
    }
    CEP_UART::Frame Receive();

    virtual void SetExpectedRxLen(size_t len) = 0;
    virtual void ClearExpectedRxLen()         = 0;

    void SetFrameReceiveCpltCallback(const std::function<void()>& cb);
    void ClearFrameReceiveCpltCallback();

    void         SetStartOfFrameSequence(uint8_t* sof, size_t len);
    virtual void SetStartOfFrameSequence(const std::string& sof) = 0;
    void         SetStartOfFrameSequence(const std::vector<uint8_t>& sof);
    void         ClearStartOfFrameSequence();

    void         SetEndOfFrameSequence(uint8_t* eof, size_t len);
    virtual void SetEndOfFrameSequence(const std::string& eof) = 0;
    void         SetEndOfFrameSequence(const std::vector<uint8_t>& eof);
    virtual void ClearEndOfFrameSequence() = 0;

  protected:
    bool WaitUntilTransmitionComplete();


  protected:
    UART_HandleTypeDef* m_handle = nullptr;
    std::string         m_label;

    CEP_UART::Status     m_status           = CEP_UART::Status::Ok;
    size_t               m_txBytesRemaining = -1;
    std::vector<uint8_t> m_txBuf;
    //    std::vector<uint8_t> m_rxBuf;    // IT only
    CircularBuffer<uint8_t> m_rxBuf;
    size_t                  m_expectedLen               = 0;
    uint32_t                m_lastCharReceivedTimestamp = 0;
    //    std::vector<UART::Frame> m_latestFrames;
    CEP_UART::Frame m_latestFrames;
    bool            m_framePending = false;

    std::string m_sof;
    bool        m_hasReceivedSof = false;
    std::string m_eof;

    std::function<void()> m_cb;

    bool   m_hasNewData    = false;    // IT only
    size_t m_dataBufferIdx = 0;        // DMA only

    static constexpr uint32_t TIMEOUT    = 100;    // Systicks.
    static constexpr uint32_t RX_TIMEOUT = 50;     // Systicks.
};

#else
#if WARN_MISSING_STM_DRIVERS
#warning NilaiTFO UART module enabled, but HAL_UART_MODULE_ENABLE is not defined!
#endif
#endif
#endif
#endif

/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
