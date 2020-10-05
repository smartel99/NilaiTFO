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
#include "defines/macros.hpp"
#include "main.h"

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */

/*************************************************************************************************/
/* Public method definitions                                                                     */
/*************************************************************************************************/

UartModule::~UartModule()
{
    HAL_UART_DeInit(m_handle);
}

void UartModule::Run()
{
}

void UartModule::Transmit(const char *msg, size_t len)
{
    CEP_ASSERT(msg != nullptr, "msg is NULL in UartModule::Transmit");
    Transmit(std::vector<uint8_t>(msg, msg + len));
}

void UartModule::Transmit(const std::string &msg)
{
    Transmit(std::vector<uint8_t>(msg.data(), msg.data() + msg.size()));
}

void UartModule::Transmit(const std::vector<uint8_t> &msg)
{
    if (WaitUntilTransmitionComplete() == false)
    {
        // Timed out.
        return;
    }

    // Copy the message into the transmission buffer.
    m_txBuf = msg;

    m_txBytesRemaining = m_txBuf.size();

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuf.data(), m_txBuf.size())
            != HAL_OK)
    {
        LOG_ERROR("In {}::Transmit: Unable to transmit message", m_label);
        return;
    }
}

UART::Frame UartModule::Receive()
{
    UART::Frame frame = m_latestFrames.back();
    m_latestFrames.pop_back();
    return frame;
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

void UartModule::SetFrameReceiveCpltCallback(const std::function<void()> &cb)
{
    CEP_ASSERT(cb != nullptr,
               "In {}::SetFrameReceiveCpltCallback, invalid callback function",
               m_label);

    m_cb = cb;
}

void UartModule::ClearFrameReceiveCpltCallback()
{
    m_cb = std::function<void()>();
}

void UartModule::SetStartOfFrameSequence(uint8_t *sof, size_t len)
{
    CEP_ASSERT(sof != nullptr, "In {}::SetStartOfFrameSequence, sof is NULL",
               m_label);
    CEP_ASSERT(len > 0, "In {}::SetStartOfFrameSequence, len is 0", m_label);
    SetStartOfFrameSequence(std::string(sof, sof + len));
}

void UartModule::SetStartOfFrameSequence(const std::string &sof)
{
    m_sof = sof;
}

void UartModule::SetStartOfFrameSequence(const std::vector<uint8_t> &sof)
{
    SetStartOfFrameSequence(std::string(sof.data(), sof.data() + sof.size()));
}

void UartModule::ClearStartOfFrameSequence()
{
    m_sof = "";
}

void UartModule::SetEndOfFrameSequence(uint8_t *eof, size_t len)
{
    CEP_ASSERT(eof != nullptr, "In {}::SetEndOfFrameSequence, sof is NULL",
               m_label);
    CEP_ASSERT(len > 0, "In {}::SetStartOfFrameSequence, len is 0", m_label);
    SetEndOfFrameSequence(std::string(eof, eof + len));
}

void UartModule::SetEndOfFrameSequence(const std::string &eof)
{
    m_eof = eof;
}

void UartModule::SetEndOfFrameSequence(const std::vector<uint8_t> &eof)
{
    SetEndOfFrameSequence(std::string(eof.data(), eof.data() + eof.size()));
}

void UartModule::ClearEndOfFrameSequence()
{
    m_eof = "";
}

void UartModule::HandleReceptionIRQ()
{
    HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_SET);
    if (m_handle->Instance->SR & UART_IT_RXNE)
    {
        // Read data register not empty.

        // Read the received character.
        uint8_t newChar = (uint8_t) m_handle->Instance->DR & (uint8_t) 0xFF;
        __HAL_UART_FLUSH_DRREGISTER(m_handle);

        // Re-enable reception interrupts, in case an idiot disabled it.
        // Disabling RXNE would basically disable the UART module,
        // with no ways of knowing it.
        __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);

        m_rxBuf.emplace_back(newChar);

        // If we want a sof:
        if (m_sof.empty() == false)
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
                if ((m_rxBuf[0] != m_sof[0])
                        || (m_rxBuf.size() > (m_sof.size() * 2)))
                {
                    // First char is not same as SoF, or we have more data than size of SoF + some
                    // margin.
                    m_rxBuf.clear();
                }
            }
            else
            {
                // SoF is start of packet, just keep starting from there.
                m_rxBuf.erase(m_rxBuf.begin(), m_rxBuf.begin() + pos);
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
            m_latestFrames.emplace_back(UART::Frame { m_rxBuf, HAL_GetTick() });
            // Clear reception buffer and call the callback
            m_rxBuf.clear();
            if (m_cb)
            {
                m_cb();
            }
        }
    }
    else
    {
        HAL_UART_IRQHandler(m_handle);
    }
    HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_RESET);
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

/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
