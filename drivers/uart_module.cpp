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
#include "uart_module.h"
#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)
#    include "main.h"

#    include <cstdarg>    // For va_list.
#    include <cstdio>
#    include <cstring>

/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#    define UART_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define UART_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)


static std::vector<Nilai::Uart::UartDataBuffer> s_dataBuffers;

/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */

/*************************************************************************************************/
/* Public method definitions                                                                     */
/*************************************************************************************************/
namespace Nilai::Drivers
{
UartModule::UartModule(UART_HandleTypeDef* uart, std::string label)
: m_handle(uart), m_label(std::move(label))
{
    NILAI_ASSERT(uart != nullptr, "UART Handle is NULL!");
    m_txBuf.reserve(64);
    m_sof.reserve(2);
    m_eof.reserve(2);

    s_dataBuffers.emplace_back(515, uart);
    m_dataBufferIdx = s_dataBuffers.size() - 1;

#    if defined(NILAI_UART_REGISTER_CALLBACKS)
    HAL_UART_RegisterCallback(m_handle, HAL_UART_RX_COMPLETE_CB_ID, &ReceptionCompleteCb);
#    endif

    HAL_UART_Receive_DMA(
      m_handle, s_dataBuffers.back().rxDmaData.data(), s_dataBuffers.back().rxDmaData.size());

    __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);

    UART_INFO("Initialized");
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
    UART_INFO("POST OK");
    return true;
}

void UartModule::Run()
{
    if (s_dataBuffers[m_dataBufferIdx].rcvCompleted)
    {
        // We received something! Copy it into our own buffer.
        m_latestFrames = Uart::Frame(s_dataBuffers[m_dataBufferIdx].rxDmaData, GetTime());
        s_dataBuffers[m_dataBufferIdx].rcvCompleted = false;
        m_framePending                              = true;
        // Restart the reception.
        HAL_UART_Receive_DMA(m_handle,
                             s_dataBuffers[m_dataBufferIdx].rxDmaData.data(),
                             s_dataBuffers[m_dataBufferIdx].rxDmaData.size());
        m_cb();
    }
}

void UartModule::Transmit(const char* msg, size_t len)
{
    NILAI_ASSERT(msg != nullptr, "msg is NULL in UartModule::Transmit");

    if (!WaitUntilTransmitionComplete())
    {
        // Timed out.
        return;
    }

    // Copy the message into the transmission buffer.
    m_txBuf.resize(len);
    memcpy((void*)m_txBuf.data(), (void*)msg, len);

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuf.data(), (uint16_t)m_txBuf.size()) != HAL_OK)
    {
        UART_ERROR("In Transmit: Unable to transmit message");
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

[[maybe_unused]] void UartModule::VTransmit(const char* fmt, ...)
{
    static char buff[256];

    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);

    Transmit(buff, len);
}

Uart::Frame UartModule::Receive()
{
    m_framePending = false;
    return m_latestFrames;
}

void UartModule::SetExpectedRxLen(size_t len)
{
    m_expectedLen = len;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModule::ClearExpectedRxLen()
{
    m_expectedLen = 1024;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModule::SetFrameReceiveCpltCallback(const std::function<void()>& cb)
{
    NILAI_ASSERT(
      cb, "[%s]: In SetFrameReceiveCpltCallback, invalid callback function", m_label.c_str());

    if (cb)
    {
        m_cb = cb;
    }
}

void UartModule::ClearFrameReceiveCpltCallback()
{
    m_cb = []() {};
}

void UartModule::SetStartOfFrameSequence(uint8_t* sof, size_t len)
{
    NILAI_ASSERT(sof != nullptr, "[%s]: In SetStartOfFrameSequence, sof is NULL", m_label.c_str());
    NILAI_ASSERT(len > 0, "[%s]: In SetStartOfFrameSequence, len is 0", m_label.c_str());
    SetStartOfFrameSequence(std::string(sof, sof + len));
}

void UartModule::SetStartOfFrameSequence(const std::string& sof)
{
    m_sof = sof;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModule::SetStartOfFrameSequence(const std::vector<uint8_t>& sof)
{
    SetStartOfFrameSequence(std::string(sof.data(), sof.data() + sof.size()));
}

void UartModule::ClearStartOfFrameSequence()
{
    m_sof.clear();
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModule::SetEndOfFrameSequence(uint8_t* eof, size_t len)
{
    NILAI_ASSERT(eof != nullptr, "[%s]: In SetEndOfFrameSequence, sof is NULL", m_label.c_str());
    NILAI_ASSERT(len > 0, "[%s]: In SetStartOfFrameSequence, len is 0", m_label.c_str());
    SetEndOfFrameSequence(std::string(eof, eof + len));
}

void UartModule::SetEndOfFrameSequence(const std::string& eof)
{
    m_eof = eof;
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

void UartModule::SetEndOfFrameSequence(const std::vector<uint8_t>& eof)
{
    SetEndOfFrameSequence(std::string(eof.data(), eof.data() + eof.size()));
}

void UartModule::ClearEndOfFrameSequence()
{
    m_eof.clear();
    ResizeDmaBuffer(m_sof.size(), m_expectedLen, m_eof.size());
}

/*************************************************************************************************/
/* Private method definitions                                                                    */
/*************************************************************************************************/
bool UartModule::WaitUntilTransmitionComplete()
{
    uint32_t timeoutTime = GetTime() + UartModule::TIMEOUT;

    while (GetTime() < timeoutTime)
    {
        if (m_handle->gState == HAL_UART_STATE_READY)
        {
            return true;
        }
    }

    return false;
}

bool UartModule::ResizeDmaBuffer(size_t sofLen, size_t len, size_t eofLen)
{
    size_t newSize = (sofLen + len + eofLen);

    HAL_StatusTypeDef s = HAL_UART_DMAStop(m_handle);
    if (s != HAL_OK)
    {
        UART_ERROR("Unable to stop the DMA stream! %i", (int)s);
        return false;
    }

    s_dataBuffers[m_dataBufferIdx].rxDmaData.resize(newSize);
    s = HAL_UART_Receive_DMA(m_handle,
                             s_dataBuffers[m_dataBufferIdx].rxDmaData.data(),
                             s_dataBuffers[m_dataBufferIdx].rxDmaData.size());
    if (s != HAL_OK)
    {
        UART_ERROR("Unable to start the DMA stream! %i", (int)s);
        return false;
    }

    return true;
}


/*************************************************************************************************/
/* Private functions definitions --------------------------------------------------------------- */
#    if defined(NILAI_UART_REGISTER_CALLBACKS)
void UartModule::ReceptionCompleteCb(UART_HandleTypeDef* huart)
#    else
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
#    endif
{
    for (auto& buff : s_dataBuffers)
    {
        // Search for the corresponding data buffer.
        if (buff.instance != huart)
        {
            continue;
        }

        // Mark the reception as being completed.
        buff.rcvCompleted = true;
        return;
    }
}
}    // namespace Nilai::Drivers
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
