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
#include "drivers/uartModule.hpp"
#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)
#include "defines/macros.hpp"
#include "main.h"

#include <cstring>
#include <cstdarg>    // For va_list.
#include <cstdio>

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */

/*************************************************************************************************/
/* Public method definitions                                                                     */
/*************************************************************************************************/
UartModule::UartModule(UART_HandleTypeDef* uart, const std::string& label)
: m_handle(uart), m_label(label)
{
    CEP_ASSERT(uart != nullptr, "UART Handle is NULL!");
    m_txBuf.reserve(64);
    m_rxBuf.reserve(512);
    m_sof.reserve(2);
    m_eof.reserve(2);
    //        m_latestFrames.reserve(8);

    __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);
    LOG_INFO("[%s]: Initialized", label.c_str());
}

UartModule::~UartModule()
{
    HAL_UART_DeInit(m_handle);
}

/**
 * If the initialization passed, the POST passes.
 * @return
 */
bool UartModule::DoPost()
{
    LOG_INFO("[%s]: POST OK", m_label.c_str());
    return true;
}

void UartModule::Run()
{
    if (m_hasNewData == true)
    {
        m_hasNewData = false;
        // If we want a sof:
        if ((m_sof.empty() == false) && (m_hasReceivedSof == false))
        {
            // Check if it is contained in the buffer.
            // #TODO Ideally we'd just want to do that when we don't know if we've received it yet.
            size_t pos = cep::FindStringInVector(m_sof, m_rxBuf);
            if (pos == std::string::npos)
            {
                // We might just not have received enough chars for SoF, check the first one
                // in the buffer and check how many chars there are in the buffer.
                /* #TODO There's a bug where if the first character matches the SoF
                 * and that m_sof.size() * 2 == m_expectedLen, the packet will be valid no matter
                 * what. Example: m_sof = "qwer" m_eof = "" m_expectedLen = 8 receiving "qasdfghj",
                 * "q       ", "qweasdfg", etc will trigger the end of frame reception.
                 */
                if ((m_rxBuf[0] != m_sof[0]) || (m_rxBuf.size() > (m_sof.size() * 2)))
                {
                    // First char is not same as SoF, or we have more data than size of SoF + some
                    // margin.
                    m_rxBuf.clear();
                }
            }
            else
            {
                // SoF is start of packet, just keep starting from there.
                m_rxBuf.erase(m_rxBuf.begin(), m_rxBuf.begin() + pos + m_sof.size());
                m_hasReceivedSof = true;
            }
        }

        // If we want a EoF:
        bool rxComplete = false;
        if (m_eof.empty() == false)
        {
            // Search for it in the buffer.
            size_t pos = cep::FindStringInVector(m_eof, m_rxBuf);
            if (pos != std::string::npos)
            {
                // We found it, packet complete.
                m_rxBuf.erase(m_rxBuf.begin() + pos, m_rxBuf.end());
                rxComplete = true;
            }
        }
        // #TODO Also find a way to add timeout functionality without creating a mess and
        // interdependencies.
        else if (m_rxBuf.size() >= m_expectedLen)
        {
            rxComplete = true;
        }

        if (rxComplete == true)
        {
            //            m_latestFrames.push_back(UART::Frame(m_rxBuf, HAL_GetTick( )));
            m_latestFrames   = std::move(CEP_UART::Frame(m_rxBuf, HAL_GetTick()));
            m_framePending   = true;
            m_hasReceivedSof = false;
            // Clear reception buffer and call the callback.
            m_rxBuf.clear();
            if (m_cb)
            {
                m_cb();
            }
        }
    }
}

void UartModule::Transmit(const char* msg, size_t len)
{
    CEP_ASSERT(msg != nullptr, "msg is NULL in UartModule::Transmit");

    if (WaitUntilTransmitionComplete() == false)
    {
        // Timed out.
        return;
    }

    // Copy the message into the transmission buffer.
    m_txBuf.resize(len);
    memcpy((void*)m_txBuf.data(), (void*)msg, len);

    m_txBytesRemaining = m_txBuf.size();

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuf.data(), (uint16_t)m_txBuf.size()) != HAL_OK)
    {
        LOG_ERROR("[%s]: In Transmit: Unable to transmit message", m_label.c_str());
        return;
    }
}

void UartModule::Transmit(const std::string& msg)
{
    Transmit(msg.c_str(), msg.size());
}

void UartModule::Transmit(const std::vector<uint8_t>& msg)
{
    Transmit((const char*)msg.data(), msg.size());
}

void UartModule::VTransmit(const char* fmt, ...)
{
    static char buff[256];

    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);

    Transmit(buff, len);
}

CEP_UART::Frame UartModule::Receive()
{
    //    UART::Frame frame = m_latestFrames.back( );
    //    m_latestFrames.pop_back( );
    //    return frame;
    m_framePending = false;
    return m_latestFrames;
}

void UartModule::SetExpectedRxLen(size_t len)
{
    m_expectedLen = len;
    m_rxBuf.reserve(len);
}

void UartModule::ClearExpectedRxLen()
{
    m_expectedLen = -1;
    m_rxBuf.shrink_to_fit();
}

void UartModule::SetFrameReceiveCpltCallback(const std::function<void()>& cb)
{
    CEP_ASSERT(cb != nullptr,
               "[%s]: In SetFrameReceiveCpltCallback, invalid callback function",
               m_label.c_str());

    m_cb = cb;
}

void UartModule::ClearFrameReceiveCpltCallback()
{
    m_cb = std::function<void()>();
}

void UartModule::SetStartOfFrameSequence(uint8_t* sof, size_t len)
{
    CEP_ASSERT(sof != nullptr, "[%s]: In SetStartOfFrameSequence, sof is NULL", m_label.c_str());
    CEP_ASSERT(len > 0, "[%s]: In SetStartOfFrameSequence, len is 0", m_label.c_str());
    SetStartOfFrameSequence(std::string(sof, sof + len));
}

void UartModule::SetStartOfFrameSequence(const std::string& sof)
{
    m_sof = sof;
}

void UartModule::SetStartOfFrameSequence(const std::vector<uint8_t>& sof)
{
    SetStartOfFrameSequence(std::string(sof.data(), sof.data() + sof.size()));
}

void UartModule::ClearStartOfFrameSequence()
{
    m_sof = "";
}

void UartModule::SetEndOfFrameSequence(uint8_t* eof, size_t len)
{
    CEP_ASSERT(eof != nullptr, "[%s]: In SetEndOfFrameSequence, sof is NULL", m_label.c_str());
    CEP_ASSERT(len > 0, "[%s]: In SetStartOfFrameSequence, len is 0", m_label.c_str());
    SetEndOfFrameSequence(std::string(eof, eof + len));
}

void UartModule::SetEndOfFrameSequence(const std::string& eof)
{
    m_eof = eof;
}

void UartModule::SetEndOfFrameSequence(const std::vector<uint8_t>& eof)
{
    SetEndOfFrameSequence(std::string(eof.data(), eof.data() + eof.size()));
}

void UartModule::ClearEndOfFrameSequence()
{
    m_eof = "";
}

void UartModule::HandleReceptionIRQ()
{
    if (m_handle->Instance->NILAI_UART_IRQ_STATUS_REG & UART_IT_RXNE)
    {
        // Read data register not empty.

        // Read the received character.
        uint8_t newChar = (uint8_t)m_handle->Instance->NILAI_UART_DATA_REG & (uint8_t)0xFF;
        __HAL_UART_FLUSH_DRREGISTER(m_handle);

        // Re-enable reception interrupts, in case an idiot disabled it.
        // Disabling RXNE would basically disable the UART module,
        // with no ways of knowing it.
        __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);

        m_rxBuf.emplace_back(newChar);

        m_hasNewData = true;
    }
    else
    {
        HAL_UART_IRQHandler(m_handle);
        // Re-enable RX IRQ, in case it was disabled.
        //        __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);
    }
}

/*************************************************************************************************/
/* Private method definitions                                                                    */
/*************************************************************************************************/
bool UartModule::WaitUntilTransmitionComplete()
{
    uint32_t timeoutTime = HAL_GetTick() + UartModule::TIMEOUT;

    while (HAL_GetTick() < timeoutTime)
    {
        if (m_handle->gState == HAL_UART_STATE_READY)
        {
            return true;
        }
    }

    return false;
}

/*************************************************************************************************/
/* Private functions definitions --------------------------------------------------------------- */
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
