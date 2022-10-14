/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  uart
 * @{
 * @file        uartModule.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @author      Paul Thomas
 * @date        2020/08/13  -  09:25
 *
 * @brief       UART communication module
 */
#ifndef GUARD_UARTMODULE_HPP
#    define GUARD_UARTMODULE_HPP
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#    define NILAI_USE_UART
#    if defined(NILAI_USE_UART)

#        include "../defines/internal_config.h"

#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Wunused-parameter"

#        include NILAI_HAL_HEADER

#        pragma GCC diagnostic pop
#        define HAL_UART_MODULE_ENABLED
#        if defined(HAL_UART_MODULE_ENABLED)

#            include "../defines/circular_buffer.h"
#            include "../defines/macros.h"
#            include "../defines/misc.h"
#            include "../defines/module.h"
#            include "../services/time.h"

#            include "UART/frame.h"
#            include "UART/status.h"

#            include <cstdint>       // For uint8_t, size_t
#            include <functional>    // For std::function
#            include <string>        // For std::string
#            include <vector>        // For std::vector

#            ifdef GTEST
#                include <gtest/gtest_prod.h>
#            endif

namespace Nilai::Drivers
{

enum class SectionState
{
    NotComplete,
    Complete,
};

class UartModule : public Nilai::Module
{
public:
    using handle_type    = UART_HandleTypeDef;
    using handle_pointer = std::add_pointer<handle_type>::type;

    using data_type        = uint8_t;
    using signed_data_type = char;

    using raw_buffer_type = std::vector<data_type>;
    using buffer_type     = CircularBuffer<typename raw_buffer_type::value_type>;
    using size_type       = buffer_type::size_type;

    using timeout_t = Nilai::time_t;

    using callback_t = std::function<void()>;

public:
    UartModule(const std::string& label,
               handle_pointer     uart,
               size_type          txl = 512,
               size_type          rxl = 512);
    ~UartModule() override;

    bool DoPost() override;
    void Run() override;

    [[nodiscard]] const std::string& GetLabel() const noexcept { return m_label; }

    /**
     * Non blocking send for bytes buffer
     * @param msg bytes to send
     * @param len number of bytes to send
     */
    void Transmit(const signed_data_type* msg, size_type len);

    /**
     * Blocking send
     * does not cancel transmission on timeout
     * @param msg bytes to send
     * @param len number of bytes to send
     * @param timeout waiting time for transmission
     * @return false on timeout
     */
    bool                  Transmit(const signed_data_type*, size_type len, timeout_t timeout);
    void                  Transmit(const data_type* buff, size_type len);
    bool                  Transmit(const data_type* buff, size_t len, timeout_t timeout);
    void                  Transmit(const std::string& msg);
    void                  Transmit(const raw_buffer_type& msg);
    [[maybe_unused]] void VTransmit(const signed_data_type* fmt, ...);

    [[nodiscard]] size_type AvailableBytes() const noexcept { return m_rxCirc.Size(); }
    [[nodiscard]] size_type AvailableFrames() const noexcept { return m_rxFrames.Size(); }

    size_type   Receive(data_type* buf, size_type len);
    size_type   Receive(data_type* buf, size_type len, timeout_t timeout);
    Uart::Frame Receive();

    void SetExpectedRxLen(size_type len);
    void ClearExpectedRxLen();

    void SetFrameReceiveCpltCallback(const callback_t& cb);
    void ClearFrameReceiveCpltCallback();

    void SetStartOfFrameSequence(const std::string& sof);
    void SetStartOfFrameSequence(const raw_buffer_type& sof);
    void SetStartOfFrameSequence(const data_type* sof, size_type len);
    void ClearStartOfFrameSequence();

    void SetEndOfFrameSequence(const std::string& eof);
    void SetEndOfFrameSequence(const raw_buffer_type& eof);
    void SetEndOfFrameSequence(const data_type* eof, size_type len);
    void ClearEndOfFrameSequence();

    void FlushRecv();

private:
    bool WaitUntilTransmissionComplete(timeout_t timeout = s_txTimeout);
    bool ResizeDmaBuffer();
    void SetTriage();
    void SearchFrame(const raw_buffer_type&  data,
                     const raw_buffer_type&  pattern,
                     std::vector<size_type>& result,
                     size_type               max_depth = 0,
                     size_type               offset    = 0);

protected:
    std::string m_label;

private:
    handle_type* m_handle = nullptr;

    Uart::Status m_status = Uart::Status::Ok;

    std::string m_sof;        // start of frame
    std::string m_eof;        // end of frame
    size_type   m_efl = 0;    // expected frame length

    size_type m_txl;    // transmission buffer size
    size_type m_rxl;    // reception buffer size

    size_type                   m_sBuffId = 0;    // Static buffer id
    raw_buffer_type             m_txBuff;         // transmission buffer
    raw_buffer_type             m_rxBuff;         // reception buffer
    buffer_type                 m_rxCirc;         // circular read access for reception buffer
    CircularBuffer<Uart::Frame> m_rxFrames;       // reception frame buffer

    callback_t m_cb;

    callback_t m_run;

    // Triage
    callback_t m_triage;

    static constexpr timeout_t s_txTimeout = 100;    // Systicks.
    static constexpr timeout_t s_rxTimeout = 50;     // Systicks.

#            ifdef GTEST
private:
    FRIEND_TEST(NilaiUart, TriageSof);
    FRIEND_TEST(NilaiUart, TriageEof);
    FRIEND_TEST(NilaiUart, TriageSofEof);
    FRIEND_TEST(NilaiUart, TriageElf);
    FRIEND_TEST(NilaiUart, TriageNone);
    FRIEND_TEST(NilaiUart, Sequence);
#            endif
};
}    // namespace Nilai::Drivers

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
