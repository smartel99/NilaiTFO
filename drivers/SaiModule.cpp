/**
 * @file    SaiModule.cpp
 * @author  Samuel Martel
 * @date    2022-05-03
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#include "SaiModule.h"

#if defined(NILAI_USE_SAI)
#    include "../defines/macros.hpp"

#    define SAI_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define SAI_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

static std::string ErrorCodeToStr(uint32_t code);

namespace Nilai::Drivers
{

#    if !defined(NILAI_USE_SAI_EVENTS)
std::map<SAI_HandleTypeDef*, SaiModule*> SaiModule::s_modules = {};
#    endif

SaiModule::SaiModule(SAI_HandleTypeDef* handle,
                     Sai::Protocol      protocol,
                     Sai::DataSize      size,
                     uint8_t            nbSlot,
                     std::string        label)
: m_handle(handle),
  m_label(std::move(label)),
  m_txHalfCpltCb([]() {}),
  m_txCpltCb([]() {}),
  m_protocol(protocol),
  m_dataSize(size),
  m_numSlot(nbSlot)
#    if defined(NILAI_USE_SAI_EVENTS)
  ,
  m_eventTxHalfCpltCb([]() {}),
  m_eventTxCpltCb([]() {})
#    endif
{
    CEP_ASSERT(m_handle != nullptr, "Handle is null!");

#    if !defined(NILAI_USE_SAI_EVENTS)
    s_modules[handle] = this;
#    endif

    SetStreamingCallbacks();

    SAI_INFO("Initialized.");
}

#    if defined(NILAI_USE_EVENTS)
bool SaiModule::OnEvent([[maybe_unused]] cep::Events::Event* event)
{
#        if defined(NILAI_USE_SAI_EVENTS)
    if (event->Category == cep::Events::EventCategories::Sai)
    {
        auto* saiEvent = reinterpret_cast<cep::Events::SaiEvent*>(event);
        if (saiEvent->Sai == m_handle)
        {
            DISABLE_WARNING("-Wswitch-enum")
            switch (saiEvent->Type)
            {
                [[likely]] case cep::Events::EventTypes::SAI_TxHalfCplt : m_eventTxHalfCpltCb();
                break;
                [[likely]] case cep::Events::EventTypes::SAI_TxCplt : m_eventTxCpltCb();
                break;
                [[unlikely]] case cep::Events::EventTypes::SAI_Error : m_lastError =
                                                                         m_handle->ErrorCode;
                break;
                default: break;
            }
            DISABLE_WARNING_POP;

            return true;
        }
    }
    return false;
#        else
    return false;
#        endif
}
#    endif

bool SaiModule::DoPost()
{
    // TODO: Do SAI POST.
    SAI_INFO("POST OK");
    return true;
}

void SaiModule::Run()
{
    if (m_lastError != 0)
    {
        SAI_ERROR(
          "An error occurred (0x%04X): %s", m_lastError, ErrorCodeToStr(m_lastError).c_str());
        m_lastError = 0;
    }
}

Sai::SamplingRate SaiModule::GetSampleRate() const
{
    return static_cast<Sai::SamplingRate>(m_handle->Init.AudioFrequency);
}

void SaiModule::SetSampleRate(Sai::SamplingRate sr)
{
    if (m_isStreaming)
    {
        // Currently streaming, stop it first.
        StopStream();
    }

    HAL_SAI_DeInit(m_handle);    // De-init the SAI peripheral.

    m_handle->Init.AudioFrequency = static_cast<uint32_t>(sr);
    // Re-initialise the peripheral directly.
    HAL_StatusTypeDef s = HAL_SAI_InitProtocol(
      m_handle, static_cast<uint32_t>(m_protocol), static_cast<uint32_t>(m_dataSize), m_numSlot);
    CEP_ASSERT(s == HAL_OK, "Unable to re-init SAI!");
}

void SaiModule::StartClock()
{
    // Constantly send nothing to generate the clocks.
    m_isClockActive = true;
    SetClockCallbacks();
    HalRestartClock(m_handle);
}

void SaiModule::StopClock()
{
    HAL_SAI_DMAStop(m_handle);
    m_isClockActive = false;
    SetStreamingCallbacks();
}

void SaiModule::SetTxHalfCpltCb(const std::function<void()>& cb)
{
    if (cb)
    {
        m_txHalfCpltCb = cb;
    }
}

void SaiModule::SetTxCpltCb(const std::function<void()>& cb)
{
    if (cb)
    {
        m_txCpltCb = cb;
    }
}

bool SaiModule::Stream(const uint8_t* samples, size_t cnt)
{
    if (m_isStreaming)
    {
        // Already streaming, can't stream again.
        return false;
    }

    // If the clock is running, stop it and change the callbacks.
    if (m_isClockActive)
    {
        HAL_SAI_DMAStop(m_handle);
        SetStreamingCallbacks();
    }

    // Launch the stream.
    if (!Restream(samples, cnt))
    {
        SAI_ERROR("Unable to start streaming!");
        return false;
    }

    m_isStreaming = true;
    return true;
}

bool SaiModule::Restream(const uint8_t* samples, size_t cnt)
{
    m_isStreaming = true;
    return HAL_SAI_Transmit_DMA(m_handle, const_cast<uint8_t*>(samples), cnt) == HAL_OK;
}

bool SaiModule::PauseStream()
{
    if (!m_isStreaming)
    {
        // We're not streaming, so technically it worked.
        return true;
    }

    if (HAL_SAI_DMAPause(m_handle) != HAL_OK)
    {
        SAI_ERROR("Unable to pause stream!");
        return false;
    }

    return true;
}

bool SaiModule::ResumeStream()
{
    if (!m_isStreaming)
    {
        return true;
    }

    if (HAL_SAI_DMAResume(m_handle) != HAL_OK)
    {
        SAI_ERROR("Unable to resume stream!");
        return false;
    }

    return true;
}

bool SaiModule::StopStream()
{
    if (!m_isStreaming)
    {
        return false;
    }

    m_isStreaming = false;

    // If DMA is actually running, stop it.
    if (m_handle->hdmatx->State == HAL_DMA_STATE_BUSY && HAL_SAI_DMAStop(m_handle) != HAL_OK)
    {
        SAI_ERROR("Unable to stop stream!");
        return false;
    }

    // Restart the clock if it was running.
    if (m_isClockActive)
    {
        StartClock();
    }

    return true;
}

void SaiModule::TxHalfCplt()
{
    m_txHalfCpltCb();
}

void SaiModule::TxCplt()
{
    m_isStreaming = false;
    m_txCpltCb();
}

void SaiModule::SetStreamingCallbacks()
{
#    if defined(NILAI_USE_SAI_EVENTS)
    m_eventTxHalfCpltCb = [this]() { TxHalfCplt(); };
    m_eventTxCpltCb     = [this]() { TxCplt(); };
#    elif USE_HAL_SAI_REGISTER_CALLBACKS == 1
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_TX_HALFCOMPLETE_CB_ID, &HalTxHalfCplt);
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_TX_COMPLETE_CB_ID, &HalTxCplt);
#    else
#        error Not Implemented
#    endif
}

void SaiModule::SetClockCallbacks()
{
#    if defined(NILAI_USE_SAI_EVENTS)
    m_eventTxHalfCpltCb = []() {};
    m_eventTxCpltCb     = [this]() { HalRestartClock(m_handle); };
#    elif USE_HAL_SAI_REGISTER_CALLBACKS == 1
    HAL_SAI_UnRegisterCallback(m_handle, HAL_SAI_TX_HALFCOMPLETE_CB_ID);
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_TX_COMPLETE_CB_ID, &HalRestartClock);
#    else
#        error Not Implemented
#    endif
}

void SaiModule::HalRestartClock(SAI_HandleTypeDef* sai)
{
    static uint32_t v = 0;
    HAL_SAI_Transmit_DMA(sai, reinterpret_cast<uint8_t*>(&v), 2);
}

#    if !defined(NILAI_USE_SAI_EVENTS)
void SaiModule::HalTxHalfCplt(SAI_HandleTypeDef* sai)
{
    s_modules[sai]->TxHalfCplt();
}

void SaiModule::HalTxCplt(SAI_HandleTypeDef* sai)
{
    s_modules[sai]->TxCplt();
}
#    endif

}    // namespace Nilai::Drivers

std::string ErrorCodeToStr(uint32_t code)
{
    std::string s;
    if ((code & HAL_SAI_ERROR_OVR) != 0)
    {
        s += "Overrun, ";
    }
    if ((code & HAL_SAI_ERROR_UDR) != 0)
    {
        s += "Underrun, ";
    }
    if ((code & HAL_SAI_ERROR_AFSDET) != 0)
    {
        s += "Anticipated Frame Sync Detection, ";
    }
    if ((code & HAL_SAI_ERROR_LFSDET) != 0)
    {
        s += "Late Frame Sync Detection, ";
    }
    if ((code & HAL_SAI_ERROR_CNREADY) != 0)
    {
        s += "Codec not ready, ";
    }
    if ((code & HAL_SAI_ERROR_WCKCFG) != 0)
    {
        s += "Clock invalid, ";
    }
    if ((code & HAL_SAI_ERROR_TIMEOUT) != 0)
    {
        s += "Timeout, ";
    }
    if ((code & HAL_SAI_ERROR_DMA) != 0)
    {
        s += "DMA Error, ";
    }
#    if USE_HAL_SAI_REGISTER_CALLBACKS == 1
    if (code & HAL_SAI_ERROR_INVALID_CALLBACK)
    {
        s += "Invalid Callback, ";
    }
#    endif

    return s;
}
#endif
