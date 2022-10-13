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
// #    include <cstdio>
// #    include <cstring>
// #    include <iterator>
#    include <ranges>
#    include <string>
// #    include <utility>
#    include <vector>

#    define UART_DEBUG(msg, ...) LOG_DEBUG("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define UART_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define UART_WARN(msg, ...)                                                                    \
        LOG_WARNING("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define UART_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)

namespace Nilai::Drivers
{
UartModule::UartModule(const std::string& label, handle_type* uart, size_type txl, size_type rxl)
: m_label(label), m_handle(uart), m_txl(txl), m_rxl(rxl), m_rxFrames(10)
{
    NILAI_ASSERT(uart != nullptr, "Handle is NULL!");
    m_txBuff.resize(txl);
    m_rxBuff.resize(rxl);

    m_rxCirc = buffer_type(rxl);

    m_sof.reserve(2);
    m_eof.reserve(2);

    if (uart->hdmarx != nullptr)
    {
        m_run = [&]()
        {
            m_rxCirc.DmaCounter(__HAL_DMA_GET_COUNTER(m_handle->hdmarx));
            m_triage();
        };
    }
    else
    {
        m_run = []() {};
    }
    m_triage = []() {};
    m_cb     = []() {};
    HAL_UART_Receive_DMA(m_handle, m_rxBuff.data(), rxl);
    //    __HAL_UART_ENABLE_IT(m_handle, UART_IT_RXNE);
    UART_INFO("Uart initialized");
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
    UART_INFO("POST OK!");
    return true;
}

void UartModule::Run()
{
    m_run();
}

void UartModule::Transmit(const signed_data_type* msg, size_type len)
{
    return Transmit(reinterpret_cast<const data_type*>(msg), len);
}

bool UartModule::Transmit(const signed_data_type* msg, size_type len, timeout_t timeout)
{
    Transmit(msg, len);
    return WaitUntilTransmissionComplete(timeout);
}

void UartModule::Transmit(const data_type* buff, size_type len)
{
    NILAI_ASSERT(buff != nullptr, "buff is NULL");

    if (!WaitUntilTransmissionComplete())
    {
        // Timed out.
        return;
    }

    // Copy the message into the transmission buffer.
    m_txBuff.resize(len);
    memcpy(m_txBuff.data(), buff, len);

    // Send the message.
    if (HAL_UART_Transmit_IT(m_handle, m_txBuff.data(), static_cast<uint16_t>(m_txBuff.size())) !=
        HAL_OK)
    {
        UART_ERROR("Unable to transmit message");
        return;
    }
}

bool UartModule::Transmit(const data_type* buff, size_type len, timeout_t timeout)
{
    Transmit(buff, len);
    return WaitUntilTransmissionComplete(timeout);
}

void UartModule::Transmit(const std::string& msg)
{
    Transmit(msg.c_str(), msg.size());
}

void UartModule::Transmit(const raw_buffer_type& msg)
{
    Transmit(msg.data(), msg.size());
}

[[maybe_unused]] void UartModule::VTransmit(const signed_data_type* fmt, ...)
{
    static signed_data_type s_buff[256];

    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(s_buff, sizeof(s_buff), fmt, args);
    va_end(args);

    Transmit(s_buff, len);
}

size_t UartModule::Receive(data_type* buf, size_type len)
{
    return m_rxCirc.Read(buf, len);
}

size_t UartModule::Receive(data_type* buf, size_type len, timeout_t timeout)
{
    timeout_t deadline = Nilai::GetTicks() + timeout;
    while (Nilai::GetTicks() < deadline)
    {
        m_rxCirc.DmaCounter(__HAL_DMA_GET_COUNTER(m_handle->hdmarx));
        if (m_rxCirc.Size() >= len)
        {
            return Receive(buf, len);
        }
    }
    return 0;
}

Uart::Frame UartModule::Receive()
{
    Uart::Frame frame;
    if (m_rxFrames.Size() != 0)
    {
        frame = m_rxFrames.Front();
        m_rxFrames.Pop();
    }
    return frame;
}

void UartModule::SetExpectedRxLen(size_type len)
{
    m_efl = len;
    SetTriage();
}

void UartModule::ClearExpectedRxLen()
{
    m_efl = 0;
    SetTriage();
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
    m_cb = []() {};
}

void UartModule::SetStartOfFrameSequence(const std::string& sof)
{
    m_sof = sof;
    SetTriage();
}

void UartModule::SetStartOfFrameSequence(const raw_buffer_type& sof)
{
    SetStartOfFrameSequence(std::string(sof.begin(), sof.end()));
}

void UartModule::SetStartOfFrameSequence(const data_type* sof, size_type len)
{
    NILAI_ASSERT(sof != nullptr, "sof is NULL");
    NILAI_ASSERT(len > 0, "len is 0");
    SetStartOfFrameSequence(std::string(sof, sof + len));
}

void UartModule::ClearStartOfFrameSequence()
{
    m_sof.clear();
    SetTriage();
}

void UartModule::SetEndOfFrameSequence(const std::string& eof)
{
    m_eof = eof;
    SetTriage();
}

void UartModule::SetEndOfFrameSequence(const raw_buffer_type& eof)
{
    SetEndOfFrameSequence(std::string {eof.begin(), eof.end()});
}

void UartModule::SetEndOfFrameSequence(const data_type* eof, size_type len)
{
    NILAI_ASSERT(eof != nullptr, "eof is NULL");
    NILAI_ASSERT(len > 0, "len is 0");
    SetEndOfFrameSequence(std::string(eof, eof + len));
}

void UartModule::ClearEndOfFrameSequence()
{
    m_eof.clear();
    SetTriage();
}

void UartModule::FlushRecv()
{
    m_rxCirc.DmaCounter(__HAL_DMA_GET_COUNTER(m_handle->hdmarx));
    m_rxCirc.SetReadPos(m_rxl - __HAL_DMA_GET_COUNTER(m_handle->hdmarx) - 1);
}

/*************************************************************************************************/
/* Private method definitions */
/*************************************************************************************************/
bool UartModule::WaitUntilTransmissionComplete(timeout_t timeout)
{
    timeout_t timeoutTime = Nilai::GetTicks() + timeout;

    while (Nilai::GetTicks() < timeoutTime)
    {
        if (m_handle->gState == HAL_UART_STATE_READY)
        {
            return true;
        }
    }

    return false;
}

bool UartModule::ResizeDmaBuffer()
{
    HAL_StatusTypeDef s = HAL_UART_DMAStop(m_handle);
    if (s != HAL_OK)
    {
        UART_ERROR("Unable to stop the DMA stream! %i", static_cast<int>(s));
        return false;
    }

    if (m_efl != 0)
    {
        m_rxl = m_efl;
        m_rxBuff.resize(m_efl);
        m_rxCirc = CircularBuffer<uint8_t>(m_efl);
    }

    s = HAL_UART_Receive_DMA(m_handle, m_rxBuff.data(), m_rxl);
    if (s != HAL_OK)
    {
        UART_ERROR("Unable to start the DMA stream! %i", static_cast<int>(s));
        return false;
    }

    return true;
}

void UartModule::SetTriage()
{
    static constexpr size_type lim_xof = 5;

    // LEN
    if (m_efl != 0)
    {
        UART_DEBUG("Triage: LEN");
        m_triage = [this]()
        {
            raw_buffer_type data;
            while (m_efl <= m_rxCirc.Size())
            {
                data.resize(m_efl);
                m_rxCirc.Read(data.data(), m_efl);
                m_rxFrames.Push({data, Nilai::GetTicks()});
                m_cb();
            }
        };
    }
    // SOF
    else if (!m_sof.empty() && m_eof.empty())
    {
        UART_DEBUG("Triage: SOF");
        m_triage = [this]()
        {
            raw_buffer_type        data(m_rxCirc.Size());
            std::vector<size_type> sofs;
            m_rxCirc.Peek(data.data(), m_rxCirc.Size());
            SearchFrame(data, raw_buffer_type {m_sof.begin(), m_sof.end()}, sofs, lim_xof);

            // Enough sof for adding frames
            if (sofs.size() > 1)
            {
                for (size_type i = 0; i < sofs.size() - 1; ++i)
                {
                    m_rxFrames.Push({raw_buffer_type(data.begin() + static_cast<int>(sofs[i]) +
                                                       static_cast<int>(m_sof.size()),
                                                     data.begin() + static_cast<int>(sofs[i + 1])),
                                     Nilai::GetTicks()});
                }
                m_rxCirc.Pop(sofs[sofs.size() - 1]);    // removed processed frames
                m_cb();
            }
        };
    }
    // EOF
    else if (m_sof.empty() && !m_eof.empty())
    {
        UART_DEBUG("Triage: EOF");
        m_triage = [this]()
        {
            raw_buffer_type        data(m_rxCirc.Size());
            std::vector<size_type> eofs;
            data.resize(m_rxCirc.Size());
            m_rxCirc.Peek(data.data(), m_rxCirc.Size());
            SearchFrame(data, {m_eof.begin(), m_eof.end()}, eofs, lim_xof);

            if (!eofs.empty())
            {
                size_type lastEof = 0;
                for (auto eof : eofs)
                {
                    m_rxFrames.Push({raw_buffer_type(data.begin() + static_cast<int>(lastEof),
                                                     data.begin() + static_cast<int>(eof)),
                                     Nilai::GetTicks()});
                    lastEof = eof + m_eof.size();
                }
                m_rxCirc.Pop(eofs.back() + m_eof.size());    // removed processed frames
                m_cb();
            }
        };
    }
    // SOF & EOF
    else if (!m_sof.empty() && !m_eof.empty())
    {
        UART_DEBUG("Triage: SOF&EOF");
        m_triage = [this]()
        {
            raw_buffer_type        data(m_rxCirc.Size());
            std::vector<size_type> sofs;
            std::vector<size_type> eofs;
            data.resize(m_rxCirc.Size());
            m_rxCirc.Peek(data.data(), m_rxCirc.Size());
            SearchFrame(data, {m_sof.begin(), m_sof.end()}, sofs, lim_xof);
            SearchFrame(data, {m_eof.begin(), m_eof.end()}, eofs, lim_xof);
            std::vector<Uart::Interval> intervals;
            for (size_t start : sofs)
            {
                for (size_t end : eofs)
                {
                    if (end < start)
                    {
                        continue;
                    }
                    if (!intervals.empty() && (intervals.back().end != 0u))
                    {
                        intervals.pop_back();
                    }
                    intervals.emplace_back(start, end);
                    break;
                }
            }
            for (auto& [start, end] : intervals)
            {
                m_rxFrames.Push({std::vector<uint8_t>(data.begin() + static_cast<int>(start) +
                                                        static_cast<int>(m_sof.size()),
                                                      data.begin() + static_cast<int>(end)),
                                 HAL_GetTick()});
            }
            if (!intervals.empty())
            {
                m_rxCirc.Pop(eofs.back() + m_eof.size());
                m_cb();
            }
        };
    }
    // No triage
    else
    {
        UART_WARN("Triage disabled");
        UART_DEBUG("LEN: %d", static_cast<int>(m_efl));
        UART_DEBUG("SOF: %s", m_sof.empty() ? "[None]" : m_sof.c_str());
        UART_DEBUG("EOF: %s", m_eof.empty() ? "[None]" : m_eof.c_str());
        m_triage = []() {};
    }
}

void UartModule::SearchFrame(const raw_buffer_type&  data,
                             const raw_buffer_type&  pattern,
                             std::vector<size_type>& result,
                             size_type               max_depth,
                             size_type               offset)
{
    auto found = std::ranges::search(
      std::ranges::subrange(data.begin() + static_cast<int>(offset), data.end()), pattern);
    if (found.empty())
    {
        return;
    }

    auto start = std::distance(data.begin(), found.begin());
    auto end   = std::distance(data.begin(), found.end());
    result.push_back(start);

    if ((max_depth == 0) || (result.size() < max_depth))
    {
        SearchFrame(data, pattern, result, max_depth, end);
    }
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
