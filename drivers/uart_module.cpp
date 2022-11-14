/**
 * @file        uart_module.cpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @author      Paul Thomas
 * @date        2020/08/13  -  09:40
 *
 * @brief       UART communication module
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "uart_module.h"
#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)
#    include "../services/logger.h"
#    include "../services/time.h"

#    include <cstdarg>    // For va_list.
#    include <cstdio>
#    include <cstring>

#    include "UART/interval.h"

#    include <algorithm>
#    include <cstdarg>    // For va_list.
#    include <ranges>
#    include <string>
#    include <utility>

#    define UART_DEBUG(msg, ...) LOG_DEBUG("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define UART_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define UART_WARN(msg, ...)                                                                    \
        LOG_WARNING("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define UART_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)

#    define NILAI_UART_USE_IT   0
#    define NILAI_UART_USE_DMA  1
#    define NILAI_UART_USE_IMPL NILAI_UART_USE_DMA


namespace Nilai::Drivers
{
CircularBuffer<UartModule*, 6> UartModule::s_uarts;

UartModule::UartModule(std::string label, handle_type* uart, size_type txl, size_type rxl) noexcept
: m_handle(uart), m_label(std::move(label))
{
    NILAI_ASSERT(uart != nullptr, "Handle is NULL!");
    s_uarts.Push(this);

    m_txBuff.resize(txl);

    m_dmaBuff.resize(rxl);
    m_rxBuff.resize(rxl);

    StartDma();
    UART_INFO("Uart initialized");
}

UartModule::~UartModule()
{
    for (auto&& uart : s_uarts)
    {
        if (uart == this)
        {
            // Remove the module from the list of modules.
            uart = nullptr;
        }
    }
    HAL_UART_DeInit(m_handle);
}

/**
 * If the initialization passed, the POST passes.
 * @return
 */
bool UartModule::DoPost()
{
    UART_INFO("POST OK!");
    return true;
}

void UartModule::Run()
{
    if (m_bytesInRxBuff != 0)
    {
        // DMA has received something!
        UART_DEBUG("Reception complete!");
        MoveCompleteFrameToFrameBuff();
        if (m_cb)
        {
            m_cb(m_rxFrames.Pop().value_or(Uart::Frame {}));
        }
    }
}

bool UartModule::Transmit(const signed_data_type* msg, size_type len)
{
    return Transmit(reinterpret_cast<const data_type*>(msg), len);
}

bool UartModule::Transmit(const signed_data_type* msg, size_type len, timeout_t timeout)
{
    Transmit(msg, len);
    return WaitUntilTransmissionComplete(timeout);
}

bool UartModule::Transmit(const data_type* buff, size_type len)
{
    NILAI_ASSERT(buff != nullptr, "buff is NULL");

    if (!WaitUntilTransmissionComplete())
    {
        // Timed out.
        return false;
    }

    // Copy the message into the transmission buffer.
    m_txBuff.resize(len);
    memcpy(m_txBuff.data(), buff, len);

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuff.data(), static_cast<uint16_t>(m_txBuff.size())) !=
        HAL_OK)
    {
        UART_ERROR("Unable to transmit message");
        return false;
    }

    return true;
}

bool UartModule::Transmit(const data_type* buff, size_type len, timeout_t timeout)
{
    Transmit(buff, len);
    return WaitUntilTransmissionComplete(timeout);
}

bool UartModule::Transmit(const std::string& msg)
{
    return Transmit(msg.c_str(), msg.size());
}

bool UartModule::Transmit(const raw_buffer_type& msg)
{
    return Transmit(msg.data(), msg.size());
}

[[maybe_unused]] bool UartModule::VTransmit(const signed_data_type* fmt, ...)
{
    if (!WaitUntilTransmissionComplete())
    {
        // Timed out.
        return false;
    }

    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(reinterpret_cast<char*>(m_txBuff.data()), m_txBuff.size(), fmt, args);
    va_end(args);

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuff.data(), static_cast<uint16_t>(len)) != HAL_OK)
    {
        UART_ERROR("Unable to transmit message");
        return false;
    }

    return true;
}

Uart::Frame UartModule::Receive(timeout_t timeout)
{
    timeout_t deadline = GetTicks() + timeout;

    while (GetTicks() <= deadline)
    {
        if (!m_rxFrames.Empty())
        {
            break;
        }
    }
    // Return an empty frame on timeout.
    return m_rxFrames.Pop().value_or(Uart::Frame {});
}

void UartModule::ForceSwap()
{
#    if NILAI_UART_USE_IMPL == NILAI_UART_USE_DMA
    HAL_UART_DMAStop(m_handle);
#    else
    HAL_UART_AbortReceive_IT(m_handle);
#    endif
    uint16_t rcvd = m_dmaBuff.size() - __HAL_DMA_GET_COUNTER(m_handle->hdmarx);
    std::memcpy(m_rxBuff.data(), m_dmaBuff.data(), m_dmaBuff.size());
    //    m_rxBuff = m_dmaBuff;
    RxCpltCallback(rcvd);
}

void UartModule::SetExpectedRxLen(size_type len)
{
    ResizeDma(len);
}

void UartModule::SetFrameReceiveCpltCallback(const callback_t& cb)
{
    if (cb)
    {
        m_cb = cb;
    }
}

void UartModule::ClearFrameReceiveCpltCallback()
{
    m_cb = {};
}

/*************************************************************************************************/
/* Private method definitions */
/*************************************************************************************************/
bool UartModule::WaitUntilTransmissionComplete(timeout_t timeout)
{
    timeout_t timeoutTime = GetTicks() + timeout;

    while (GetTicks() < timeoutTime)
    {
        if (m_handle->gState == HAL_UART_STATE_READY)
        {
            return true;
        }
    }

    return false;
}

void UartModule::MoveCompleteFrameToFrameBuff()
{
    auto frameData = std::vector<uint8_t> {m_rxBuff.begin(), m_rxBuff.begin() + m_bytesInRxBuff};
    m_rxFrames.Emplace(frameData, GetTicks());
    m_bytesInRxBuff = 0;
}

bool UartModule::StartDma()
{
#    if NILAI_UART_USE_IMPL == NILAI_UART_USE_DMA
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(m_handle, m_dmaBuff.data(), m_dmaBuff.size());
#    else
    HAL_StatusTypeDef status = HAL_UART_Receive_IT(m_handle, m_dmaBuff.data(), m_dmaBuff.size());
#    endif
    return status == HAL_OK;
}

bool UartModule::ResizeDma(size_t newSize)
{
#    if NILAI_UART_USE_IMPL == NILAI_UART_USE_DMA
    HAL_StatusTypeDef s = HAL_UART_DMAStop(m_handle);
#    else
    HAL_StatusTypeDef s      = HAL_UART_AbortReceive_IT(m_handle);
#    endif
    if (s != HAL_OK)
    {
        UART_ERROR("Unable to stop the DMA stream! %i", static_cast<int>(s));
        return false;
    }

    // Resizing the DMA buffer drops whatever data is pending!
    m_rxBuff.resize(newSize);
    m_dmaBuff.resize(newSize);

    return StartDma();
}

void UartModule::RxCpltCallback(UartModule::handle_type* handle, uint16_t size)
{
    // Find the module to which this handle belongs, then call its callback.
    for (auto&& module : s_uarts)
    {
        if (module->m_handle == handle)
        {
            module->RxCpltCallback(size);
            return;
        }
    }
}

void UartModule::RxCpltCallback(uint16_t size)
{
    if (m_bytesInRxBuff != 0)
    {
        // Frame already pending, run hasn't done its job yet, do it for it.
        MoveCompleteFrameToFrameBuff();
    }
    m_bytesInRxBuff = size;
    StartDma();
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size)
{
    UartModule::RxCpltCallback(huart, size);
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    UartModule::RxCpltCallback(huart, huart->RxXferSize);
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
    // placeholder for now, just in case.
}


/*************************************************************************************************/
/* Private functions definitions
 * --------------------------------------------------------------- */

}    // namespace Nilai::Drivers

#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
