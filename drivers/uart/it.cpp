#include "it.hpp"

#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
/*************************************************************************************************/

/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */
/*************************************************************************************************/

/*************************************************************************************************/
/* Public method definitions                                                                     */
/*************************************************************************************************/

UartModuleIt::UartModuleIt(UART_HandleTypeDef* uart, const std::string& label)
: UartModule(uart, label) {
    CEP_ASSERT(uart != nullptr, "UART Handle is NULL!");
    m_txBuf.reserve(64);
    m_rxBuf.reserve(512);
    m_sof.reserve(2);
    m_eof.reserve(2);
    //        m_latestFrames.reserve(8);

    __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);
    LOG_INFO("[%s]: Initialized", label.c_str());
}

void UartModuleIt::Run() {
    if (m_hasNewData == true) {
        m_hasNewData = false;
        // If we want a sof:
        if ((m_sof.empty() == false) && (m_hasReceivedSof == false)) {
            // Check if it is contained in the buffer.
            // #TODO Ideally we'd just want to do that when we don't know if we've received it yet.
            size_t pos = cep::FindStringInVector(m_sof, m_rxBuf);
            if (pos == std::string::npos) {
                // We might just not have received enough chars for SoF, check the first one
                // in the buffer and check how many chars there are in the buffer.
                /* #TODO There's a bug where if the first character matches the SoF
                 * and that m_sof.size() * 2 == m_expectedLen, the packet will be valid no matter
                 * what. Example: m_sof = "qwer" m_eof = "" m_expectedLen = 8 receiving "qasdfghj",
                 * "q       ", "qweasdfg", etc will trigger the end of frame reception.
                 */
                if ((m_rxBuf[0] != m_sof[0]) || (m_rxBuf.size() > (m_sof.size() * 2))) {
                    // First char is not same as SoF, or we have more data than size of SoF + some
                    // margin.
                    m_rxBuf.clear();
                }
            } else {
                // SoF is start of packet, just keep starting from there.
                m_rxBuf.erase(m_rxBuf.begin(), m_rxBuf.begin() + pos + m_sof.size());
                m_hasReceivedSof = true;
            }
        }

        // If we want a EoF:
        bool rxComplete = false;
        if (m_eof.empty() == false) {
            // Search for it in the buffer.
            size_t pos = cep::FindStringInVector(m_eof, m_rxBuf);
            if (pos != std::string::npos) {
                // We found it, packet complete.
                m_rxBuf.erase(m_rxBuf.begin() + pos, m_rxBuf.end());
                rxComplete = true;
            }
        }
        // #TODO Also find a way to add timeout functionality without creating a mess and
        // interdependencies.
        else if (m_rxBuf.size() >= m_expectedLen) {
            rxComplete = true;
        }

        if (rxComplete == true) {
            //            m_latestFrames.push_back(UART::Frame(m_rxBuf, HAL_GetTick( )));
            m_latestFrames   = std::move(CEP_UART::Frame(m_rxBuf, HAL_GetTick()));
            m_framePending   = true;
            m_hasReceivedSof = false;
            // Clear reception buffer and call the callback.
            m_rxBuf.clear();
            if (m_cb) { m_cb(); }
        }
    }
}

void UartModuleIt::SetExpectedRxLen(size_t len) {
    m_expectedLen = len;
    m_rxBuf.reserve(len);
}

void UartModuleIt::ClearExpectedRxLen() {
    m_expectedLen = -1;
    m_rxBuf.shrink_to_fit();
}

void UartModuleIt::SetStartOfFrameSequence(const std::string& sof) {
    m_sof = sof;
}

void UartModuleIt::SetEndOfFrameSequence(const std::string& eof) {
    m_eof = eof;
}

void UartModuleIt::ClearEndOfFrameSequence() {
    m_eof = "";
}

void UartModuleIt::HandleReceptionIRQ() {
    if (m_handle->Instance->NILAI_UART_IRQ_STATUS_REG & UART_IT_RXNE) {
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
    } else {
        HAL_UART_IRQHandler(m_handle);
        // Re-enable RX IRQ, in case it was disabled.
        //        __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);
    }
}

#endif
