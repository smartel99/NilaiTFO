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
#            include "../defines/swap_buffer.h"
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

class UartModule : public Nilai::Module
{
public:
    using handle_type    = UART_HandleTypeDef;
    using handle_pointer = std::add_pointer<handle_type>::type;

    using data_type        = uint8_t;
    using signed_data_type = char;

    using raw_buffer_type = std::vector<data_type>;

    using buffer_type = std::vector<data_type>;

    using size_type = typename raw_buffer_type::size_type;

    using timeout_t = Nilai::time_t;

    using callback_t = std::function<void(Uart::Frame)>;

public:
    UartModule() noexcept = default;
    UartModule(std::string    label,
               handle_pointer uart,
               size_type      txl = 64,
               size_type      rxl = 64) noexcept;
    ~UartModule() override;

    bool DoPost() override;
    void Run() override;

    [[nodiscard]] const std::string& GetLabel() const noexcept { return m_label; }

    /**
     * Non blocking send for bytes buffer
     * @param msg bytes to send
     * @param len number of bytes to send
     */
    bool Transmit(const signed_data_type* msg, size_type len);

    /**
     * Blocking send
     * does not cancel transmission on timeout
     * @param msg bytes to send
     * @param len number of bytes to send
     * @param timeout waiting time for transmission
     * @return false on timeout
     */
    bool                  Transmit(const signed_data_type*, size_type len, timeout_t timeout);
    bool                  Transmit(const data_type* buff, size_type len);
    bool                  Transmit(const data_type* buff, size_t len, timeout_t timeout);
    bool                  Transmit(const std::string& msg);
    bool                  Transmit(const raw_buffer_type& msg);
    [[maybe_unused]] bool VTransmit(const signed_data_type* fmt, ...);

    [[nodiscard]] size_type AvailableFrames() const noexcept { return m_rxFrames.Size(); }

    // Blocks until a frame is received.
    Uart::Frame Receive(timeout_t timeout = s_rxTimeout);

    void ForceSwap();

    void SetExpectedRxLen(size_type len);

    void SetFrameReceiveCpltCallback(const callback_t& cb);
    void ClearFrameReceiveCpltCallback();

private:
    bool WaitUntilTransmissionComplete(timeout_t timeout = s_txTimeout);

    void RxCpltCallback(uint16_t size);
    bool ResizeDma(size_t newSize);
    bool StartDma();
    void MoveCompleteFrameToFrameBuff();

protected:
    handle_type* m_handle = nullptr;    //!< Pointer to the hardware peripheral.
    
private:
    std::string m_label;    //!< Label of the module, used for logging purposes.


    raw_buffer_type m_txBuff;    //!< Transmission buffer

    /**
     * Number of bytes that have been received by DMA.
     * When this value is not 0, it means that a DMA event has occurred and that the buffer should
     * be swapped.
     */
    uint16_t    m_bytesInRxBuff = 0;
    buffer_type m_dmaBuff;
    buffer_type m_rxBuff;    //!< Buffer where the raw, unprocessed data is received.

    //! Buffer where received frames are stored.
    CircularBuffer<Uart::Frame, NILAI_UART_RX_FRAME_BUFF_SIZE> m_rxFrames;

    callback_t m_cb;

    static constexpr timeout_t s_txTimeout = 100;    // Systicks.
    static constexpr timeout_t s_rxTimeout = 50;     // Systicks.

    static CircularBuffer<UartModule*, 6> s_uarts;

public:
    static void RxCpltCallback(UartModule::handle_type* handle, uint16_t size);

private:
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
