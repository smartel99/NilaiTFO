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
#ifndef NILAI_UARTMODULE_HPP
#    define NILAI_UARTMODULE_HPP
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#    if defined(NILAI_USE_UART)
#        include "../defines/internalConfig.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_UART_MODULE_ENABLED)
#            include "../defines/macros.hpp"
#            include "../defines/misc.hpp"
#            include "../defines/module.hpp"

#            include "UART/Enums.h"
#            include "UART/Structs.h"

#            if defined(NILAI_USE_EVENTS)
#                include "../defines/Events/Events.h"
#            endif

#            include <cstdint>       // For uint8_t, size_t
#            include <functional>    // For std::function
#            include <string>        // For std::string
#            include <vector>        // For std::vector

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Enumerated Types ---------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class UartModule : public cep::Module
{
public:
    UartModule(UART_HandleTypeDef* uart, std::string label);
    ~UartModule() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

#            if defined(NILAI_USE_EVENTS)
    bool OnEvent(cep::Events::Event* event) override;
#            endif

    void                  Transmit(const char* msg, size_t len);
    void                  Transmit(const std::string& msg);
    void                  Transmit(const std::vector<uint8_t>& msg);
    [[maybe_unused]] void VTransmit(const char* fmt, ...);

    [[nodiscard]] size_t GetNumberOfWaitingFrames() const { return (m_framePending ? 1 : 0); }
    cep::Uart::Frame     Receive();

    void SetExpectedRxLen(size_t len);
    void ClearExpectedRxLen();

    void SetFrameReceiveCpltCallback(const std::function<void()>& cb);
    void ClearFrameReceiveCpltCallback();

    void SetStartOfFrameSequence(uint8_t* sof, size_t len);
    void SetStartOfFrameSequence(const std::string& sof);
    void SetStartOfFrameSequence(const std::vector<uint8_t>& sof);
    void ClearStartOfFrameSequence();

    void SetEndOfFrameSequence(uint8_t* eof, size_t len);
    void SetEndOfFrameSequence(const std::string& eof);
    void SetEndOfFrameSequence(const std::vector<uint8_t>& eof);
    void ClearEndOfFrameSequence();

private:
    bool WaitUntilTransmitionComplete();

#            if defined(NILAI_USE_UART_EVENTS)
    static void ReceptionCompleteCb(UART_HandleTypeDef* huart);
#            endif
    bool ResizeDmaBuffer(size_t sofLen, size_t len, size_t eofLen);

private:
    UART_HandleTypeDef* m_handle = nullptr;
    std::string         m_label;

    cep::Uart::Status    m_status           = cep::Uart::Status::Ok;
    size_t               m_txBytesRemaining = -1;
    std::vector<uint8_t> m_txBuf;
    size_t               m_expectedLen               = 0;
    uint32_t             m_lastCharReceivedTimestamp = 0;
    cep::Uart::Frame     m_latestFrames;
    bool                 m_framePending = false;

    std::string m_sof;
    bool        m_hasReceivedSof = false;
    std::string m_eof;

    std::function<void()> m_cb = []() {};

    size_t m_dataBufferIdx = 0;

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
