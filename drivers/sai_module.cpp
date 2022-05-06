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
#include "sai_module.h"

#if defined(NILAI_USE_SAI)
#    include "../Nilai.h"

#    define SAI_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define SAI_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

static void             PrintStatus(uint32_t code);
static std::string_view ErrorCodeToStr(uint32_t code);

namespace Nilai::Drivers
{

static std::map<SAI_HandleTypeDef*, SaiModule*> s_modules = {};

SaiModule::SaiModule(SAI_HandleTypeDef* handle,
                     std::string        label,
                     Sai::Protocol      protocol,
                     Sai::DataSize      size,
                     uint8_t            nbSlot)
: m_handle(handle),
  m_label(std::move(label)),
  m_txHalfCpltCb([]() {}),
  m_txCpltCb([]() {}),
  m_protocol(protocol),
  m_dataSize(size),
  m_numSlot(nbSlot)
{
    NILAI_ASSERT(m_handle != nullptr, "Handle is null!");

    s_modules[handle] = this;

#    if defined(NILAI_SAI_REGISTER_EVENTS)
    Nilai::Application::Get()->RegisterEventCallback(Events::EventTypes::SAI_Error,
                                                     [this](Events::Event*)
                                                     {
                                                         m_lastError = m_handle->ErrorCode;
                                                         return true;
                                                     });
#    else
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_ERROR_CB_ID, &HalError);
#    endif
    SetStreamingCallbacks();

    SAI_INFO("Initialized.");
}


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
        SAI_ERROR("An error occurred: 0x%04X", m_lastError);
#    if defined(NILAI_ENABLE_STATUS_TO_STR)
        PrintStatus(m_lastError);
#    endif
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
    NILAI_ASSERT(s == HAL_OK, "Unable to re-init SAI!");
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

bool SaiModule::Stream(const void* samples, size_t cnt)
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

bool SaiModule::Restream(const void* samples, size_t cnt)
{
    m_isStreaming = true;
    return HAL_SAI_Transmit_DMA(m_handle, (uint8_t*)samples, cnt) == HAL_OK;
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

void SaiModule::Error()
{
    m_lastError = m_handle->ErrorCode;
}

void SaiModule::SetStreamingCallbacks()
{
#    if defined(NILAI_SAI_REGISTER_EVENTS)
    Application* app = Application::Get();
    UnregisterEvents();
    m_txHalfCpltCbId = app->RegisterEventCallback(Events::EventTypes::SAI_TxHalfCplt,
                                                  [this](Events::Event*)
                                                  {
                                                      TxHalfCplt();
                                                      return true;
                                                  });
    m_txCpltCbId     = app->RegisterEventCallback(Events::EventTypes::SAI_TxCplt,
                                              [this](Events::Event*)
                                              {
                                                  TxCplt();
                                                  return true;
                                              });
#    elif defined(NILAI_SAI_REGISTER_CALLBACKS)
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_TX_HALFCOMPLETE_CB_ID, &HalTxHalfCplt);
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_TX_COMPLETE_CB_ID, &HalTxCplt);
#    endif
}

void SaiModule::SetClockCallbacks()
{
#    if defined(NILAI_USE_SAI_EVENTS)
    UnregisterEvents();
    m_txCpltCbId = Application::Get()->RegisterEventCallback(Events::EventTypes::SAI_TxCplt,
                                                             [this](Events::Event*)
                                                             {
                                                                 HalRestartClock(m_handle);
                                                                 return true;
                                                             });
#    elif defined(NILAI_SAI_REGISTER_CALLBACKS)
    HAL_SAI_UnRegisterCallback(m_handle, HAL_SAI_TX_HALFCOMPLETE_CB_ID);
    HAL_SAI_RegisterCallback(m_handle, HAL_SAI_TX_COMPLETE_CB_ID, &HalRestartClock);
#    endif
}

void SaiModule::HalRestartClock(SAI_HandleTypeDef* sai)
{
    static uint32_t v = 0;
    HAL_SAI_Transmit_DMA(sai, reinterpret_cast<uint8_t*>(&v), 2);
}

#    if defined(NILAI_SAI_REGISTER_EVENTS)
void SaiModule::UnregisterEvents()
{
    Application* app = Application::Get();
    if (m_txHalfCpltCbId != std::numeric_limits<size_t>::max())
    {
        app->UnregisterEventCallback(Events::EventTypes::SAI_TxHalfCplt, m_txHalfCpltCbId);
        m_txHalfCpltCbId = std::numeric_limits<size_t>::max();
    }
    if (m_txCpltCbId != std::numeric_limits<size_t>::max())
    {
        app->UnregisterEventCallback(Events::EventTypes::SAI_TxCplt, m_txCpltCbId);
        m_txCpltCbId = std::numeric_limits<size_t>::max();
    }
}
#    endif

#    if !defined(NILAI_SAI_REGISTER_EVENTS)
#        if defined(NILAI_SAI_REGISTER_CALLBACKS)
void SaiModule::HalTxHalfCplt(SAI_HandleTypeDef* sai)
#        else
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef* sai)
#        endif
{
    s_modules[sai]->TxHalfCplt();
}
#    endif

#    if !defined(NILAI_SAI_REGISTER_EVENTS)
#        if defined(NILAI_SAI_REGISTER_CALLBACKS)
void SaiModule::HalTxCplt(SAI_HandleTypeDef* sai)
#        else
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef* sai)
#        endif
{
    s_modules[sai]->TxCplt();
}

#    endif

#    if !defined(NILAI_SAI_REGISTER_EVENTS)
#        if defined(NILAI_SAI_REGISTER_CALLBACKS)
void SaiModule::HalError(SAI_HandleTypeDef* sai)
#        else
void HAL_SAI_ErrorCallback(SAI_HandleTypeDef* sai)
#        endif
{
    s_modules[sai]->Error();
}
#    endif
}    // namespace Nilai::Drivers

//--------------------------------------------------------------------------------------------------
// Private function definitions

std::string_view ErrorCodeToStr(uint32_t code)
{
    using namespace std::literals::string_view_literals;
#    if defined(NILAI_ENABLE_STATUS_TO_STR)
    switch (code)
    {
        case HAL_SAI_ERROR_OVR: return "Overrun"sv;
        case HAL_SAI_ERROR_UDR: return "Underrun"sv;
        case HAL_SAI_ERROR_AFSDET: return "Anticipated Frame Sync Detection"sv;
        case HAL_SAI_ERROR_LFSDET: return "Late Frame Sync Detection"sv;
        case HAL_SAI_ERROR_CNREADY: return "Codec not ready"sv;
        case HAL_SAI_ERROR_WCKCFG: return "Clock Invalid"sv;
        case HAL_SAI_ERROR_TIMEOUT: return "Timeout"sv;
        case HAL_SAI_ERROR_DMA: return "DMA Error"sv;
#        if USE_HAL_SAI_REGISTER_CALLBACKS == 1
        case HAL_SAI_ERROR_INVALID_CALLBACK: return "Invalid Callback"sv;
#        endif
        default: return "Unknown"sv;
    }
#    else
    return ""sv;
#    endif
}

void PrintStatus(uint32_t code)
{
#    if defined(NILAI_ENABLE_STATUS_TO_STR)
    static constexpr uint32_t errorMask =
      HAL_SAI_ERROR_OVR | HAL_SAI_ERROR_UDR | HAL_SAI_ERROR_AFSDET | HAL_SAI_ERROR_LFSDET |
      HAL_SAI_ERROR_CNREADY | HAL_SAI_ERROR_WCKCFG | HAL_SAI_ERROR_TIMEOUT | HAL_SAI_ERROR_DMA
#        if USE_HAL_SAI_REGISTER_CALLBACKS == 1
      | HAL_SAI_ERROR_INVALID_CALLBACK
#        endif
      ;

    // Check only the bits that can be errors.
    for (uint32_t i = 1; (i & errorMask) != 0; i <<= 1)
    {
        LOG_ERROR("\t\t- %s", ErrorCodeToStr(code & i).data());
    }
#    endif
}
#endif
