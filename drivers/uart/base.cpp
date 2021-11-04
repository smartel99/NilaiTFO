/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  uart
 * @{
 * @file        uartModule.cpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/08/13  -  09:40
 *
 * @brief       UART communication module
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
/*************************************************************************************************/
#include "base.hpp"
#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)
#include "defines/macros.hpp"
#include "main.h"

#include <cstdarg>    // For va_list.
#include <cstdio>
#include <cstring>

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
/*************************************************************************************************/

/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */
/*************************************************************************************************/


/*************************************************************************************************/
/* Public method definitions                                                                     */
/*************************************************************************************************/
UartModule::~UartModule() {
    HAL_UART_DeInit(m_handle);
}

/**
 * If the initialization passed, the POST passes.
 * @return
 */
bool UartModule::DoPost() {
    LOG_INFO("[%s]: POST OK", m_label.c_str());
    return true;
}

void UartModule::Transmit(const char* msg, size_t len) {
    CEP_ASSERT(msg != nullptr, "msg is NULL in UartModule::Transmit");

    if (!WaitUntilTransmitionComplete()) {
        // Timed out.
        return;
    }

    // Copy the message into the transmission buffer.
    m_txBuf.resize(len);
    memcpy((void*)m_txBuf.data(), (void*)msg, len);

    m_txBytesRemaining = m_txBuf.size();

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuf.data(), (uint16_t)m_txBuf.size()) != HAL_OK) {
        LOG_ERROR("[%s]: In Transmit: Unable to transmit message", m_label.c_str());
        return;
    }
}

void UartModule::Transmit(const std::string& msg) {
    Transmit(msg.c_str(), msg.size());
}

void UartModule::Transmit(const std::vector<uint8_t>& msg) {
    Transmit((const char*)msg.data(), msg.size());
}

[[maybe_unused]] void UartModule::VTransmit(const char* fmt, ...) {
    static char buff[256];

    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);

    Transmit(buff, len);
}

CEP_UART::Frame UartModule::Receive() {
    //    UART::Frame frame = m_latestFrames.back( );
    //    m_latestFrames.pop_back( );
    //    return frame;
    m_framePending = false;
    return m_latestFrames;
}

void UartModule::SetFrameReceiveCpltCallback(const std::function<void()>& cb) {
    CEP_ASSERT(cb != nullptr, "[%s]: In SetFrameReceiveCpltCallback, invalid callback function", m_label.c_str());

    m_cb = cb;
}

void UartModule::ClearFrameReceiveCpltCallback() {
    m_cb = std::function<void()>();
}

void UartModule::SetStartOfFrameSequence(uint8_t* sof, size_t len) {
    CEP_ASSERT(sof != nullptr, "[%s]: In SetStartOfFrameSequence, sof is NULL", m_label.c_str());
    CEP_ASSERT(len > 0, "[%s]: In SetStartOfFrameSequence, len is 0", m_label.c_str());
    SetStartOfFrameSequence(std::string(sof, sof + len));
}

void UartModule::SetStartOfFrameSequence(const std::vector<uint8_t>& sof) {
    SetStartOfFrameSequence(std::string(sof.data(), sof.data() + sof.size()));
}

void UartModule::ClearStartOfFrameSequence() {
    m_sof = "";
}

void UartModule::SetEndOfFrameSequence(uint8_t* eof, size_t len) {
    CEP_ASSERT(eof != nullptr, "[%s]: In SetEndOfFrameSequence, sof is NULL", m_label.c_str());
    CEP_ASSERT(len > 0, "[%s]: In SetStartOfFrameSequence, len is 0", m_label.c_str());
    SetEndOfFrameSequence(std::string(eof, eof + len));
}

void UartModule::SetEndOfFrameSequence(const std::vector<uint8_t>& eof) {
    SetEndOfFrameSequence(std::string(eof.data(), eof.data() + eof.size()));
}

/*************************************************************************************************/
/* Private method definitions                                                                    */
/*************************************************************************************************/
bool UartModule::WaitUntilTransmitionComplete() {
    uint32_t timeoutTime = HAL_GetTick() + UartModule::TIMEOUT;

    while (HAL_GetTick() < timeoutTime) {
        if (m_handle->gState == HAL_UART_STATE_READY) { return true; }
    }

    return false;
}


/*************************************************************************************************/
/* Private functions definitions --------------------------------------------------------------- */
/*************************************************************************************************/



#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
