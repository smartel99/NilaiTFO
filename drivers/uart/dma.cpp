#include "dma.hpp"

#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
/*************************************************************************************************/

static std::vector<CEP_UART::UartDataBuffer> s_dataBuffers;
/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */
/*************************************************************************************************/

/*************************************************************************************************/
/* Public method definitions                                                                     */
/*************************************************************************************************/
UartModuleDma::UartModuleDma(UART_HandleTypeDef* uart, const std::string& label)
: UartModule(uart, label) {
    CEP_ASSERT(uart != nullptr, "UART Handle is NULL!");
    m_txBuf.reserve(64);
    m_sof.reserve(2);
    m_eof.reserve(2);
    //        m_latestFrames.reserve(8);

    s_dataBuffers.emplace_back(515, uart);
    m_dataBufferIdx = s_dataBuffers.size() - 1;
    __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);

    HAL_UART_Receive_DMA(m_handle, s_dataBuffers.back().rxDmaData.data(), s_dataBuffers.back().rxDmaData.size());
    LOG_INFO("[%s]: Initialized", label.c_str());
}

void UartModuleDma::Run() {
    if (s_dataBuffers[m_dataBufferIdx].rcvCompleted) {
        // We received something! Copy it into our own buffer.
        m_latestFrames = CEP_UART::Frame(s_dataBuffers[m_dataBufferIdx].rxDmaData, HAL_GetTick());
        s_dataBuffers[m_dataBufferIdx].rcvCompleted = false;
        m_framePending                              = true;
        // Restart the reception.
        HAL_UART_Receive_DMA(
          m_handle,
          s_dataBuffers[m_dataBufferIdx].rxDmaData.data(),
          s_dataBuffers[m_dataBufferIdx].rxDmaData.size());
    }
}

void UartModuleDma::SetExpectedRxLen(size_t len) {
    m_expectedLen = len;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModuleDma::ClearExpectedRxLen() {
    m_expectedLen = 1024;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModuleDma::SetStartOfFrameSequence(const std::string& sof) {
    m_sof = sof;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModuleDma::SetEndOfFrameSequence(const std::string& eof) {
    m_eof = eof;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModuleDma::ClearEndOfFrameSequence() {
    m_eof = "";
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}


/*************************************************************************************************/
/* Private method definitions                                                                    */
/*************************************************************************************************/
bool UartModuleDma::ResizeDmaBuffer(size_t sofLen, size_t len, size_t eofLen) {
    size_t newSize = (sofLen + len + eofLen);

    HAL_StatusTypeDef s = HAL_UART_DMAStop(m_handle);
    if (s != HAL_OK) {
        LOG_ERROR("[%s]: Unable to stop the DMA stream! %i", m_label.c_str(), (int)s);
        return false;
    }

    s_dataBuffers[m_dataBufferIdx].rxDmaData.resize(newSize);
    s = HAL_UART_Receive_DMA(
      m_handle,
      s_dataBuffers[m_dataBufferIdx].rxDmaData.data(),
      s_dataBuffers[m_dataBufferIdx].rxDmaData.size());
    if (s != HAL_OK) {
        LOG_ERROR("[%s]: Unable to start the DMA stream! %i", m_label.c_str(), (int)s);
        return false;
    }

    return true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    for (auto& buff : s_dataBuffers) {
        // Search for the corresponding data buffer.
        if (buff.instance != huart) { continue; }

        // Mark the reception as being completed.
        buff.rcvCompleted = true;
    }
}

#endif
