/**
 * @file    i2s_module.cpp
 * @author  Samuel Martel
 * @date    2022-02-07
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#include "i2s_module.h"

#if defined(NILAI_USE_I2S) && !defined(NILAI_TEST)
#    include "../defines/macros.h"

#    define I2S_INFO(msg, ...)    LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define I2S_WARNING(msg, ...) LOG_WARNING("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define I2S_ERROR(msg, ...)   LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

namespace Nilai::Drivers
{
static std::map<I2S_HandleTypeDef*, I2sModule*> s_modules = {};

#    if !defined(NILAI_I2S_REGISTER_CALLBACKS)
static bool s_restartClock = false;
#    endif

I2sModule::I2sModule(I2S_HandleTypeDef* handle, std::string label)
: m_handle(handle), m_label(std::move(label)), m_txHalfCpltCb([]() {}), m_txCpltCb([]() {})
{
    NILAI_ASSERT(m_handle != nullptr, "Handle is null!");
    s_modules[handle] = this;

    SetStreamingCallbacks();

    I2S_INFO("Initialized.");
}

bool I2sModule::DoPost()
{
    // TODO: Do I2S POST.
    I2S_INFO("POST OK");
    return true;
}

void I2sModule::Run()
{
}

void I2sModule::SetTxHalfCpltCb(const std::function<void()>& cb)
{
    if (cb)
    {
        m_txHalfCpltCb = cb;
    }
}

void I2sModule::SetTxCpltCb(const std::function<void()>& cb)
{
    if (cb)
    {
        m_txCpltCb = cb;
    }
}

void I2sModule::TxHalfCplt()
{
    m_txHalfCpltCb();
}

void I2sModule::TxCplt()
{
    m_isStreaming = false;
    m_txCpltCb();
}

void I2sModule::HalTxHalfCplt(I2S_HandleTypeDef* i2s)
{
    s_modules[i2s]->TxHalfCplt();
}

void I2sModule::HalTxCplt(I2S_HandleTypeDef* i2s)
{
    s_modules[i2s]->TxCplt();
}

bool I2sModule::Stream(const void* samples, size_t cnt)
{
    if (m_isStreaming)
    {
        return false;
    }

    // If the clock is running, stop it and change the callbacks.
    if (m_isClockActive)
    {
        HAL_I2S_DMAStop(m_handle);
        SetStreamingCallbacks();
    }

    if (!Restream(samples, cnt))
    {
        I2S_ERROR("Unable to start streaming!");
        return false;
    }

    m_isStreaming = true;
    return true;
}

bool I2sModule::Restream(const void* samples, size_t cnt)
{
    m_isStreaming = true;
    __HAL_I2S_ENABLE(m_handle);
    __HAL_I2S_ENABLE_IT(m_handle, I2S_IT_ERR);
    return HAL_I2S_Transmit_DMA(m_handle, (uint16_t*)samples, cnt) == HAL_OK;
}

bool I2sModule::PauseStream()
{
    if (!m_isStreaming)
    {
        return true;
    }

    if (HAL_I2S_DMAPause(m_handle) != HAL_OK)
    {
        I2S_ERROR("Unable to pause stream!");
        return false;
    }

    return true;
}

bool I2sModule::ResumeStream()
{
    if (!m_isStreaming)
    {
        return true;
    }

    if (HAL_I2S_DMAResume(m_handle) != HAL_OK)
    {
        I2S_ERROR("Unable to resume stream!");
        return false;
    }

    return true;
}

bool I2sModule::StopStream()
{
    if (!m_isStreaming)
    {
        return false;
    }

    m_isStreaming = false;

    // If DMA is actually running, stop it.
    if (m_handle->hdmatx->State == HAL_DMA_STATE_BUSY && HAL_I2S_DMAStop(m_handle) != HAL_OK)
    {
        I2S_ERROR("Unable to stop stream!");
        return false;
    }
    __HAL_I2S_DISABLE_IT(m_handle, I2S_IT_ERR);
    __HAL_I2S_DISABLE(m_handle);

    // Restart the clock if it was running.
    if (m_isClockActive)
    {
        StartClock();
    }

    return true;
}

void I2sModule::StartClock()
{
    // Constantly send nothing to generate the clocks for I2S.
    m_isClockActive = true;
    SetClockCallbacks();
    HalRestartClock(m_handle);
}

void I2sModule::StopClock()
{
    HAL_I2S_DMAStop(m_handle);
    m_isClockActive = false;
    SetStreamingCallbacks();
}

void I2sModule::SetStreamingCallbacks()
{
#    if defined(NILAI_I2S_REGISTER_CALLBACKS)
    HAL_I2S_RegisterCallback(m_handle, HAL_I2S_TX_HALF_COMPLETE_CB_ID, &HalTxHalfCplt);
    HAL_I2S_RegisterCallback(m_handle, HAL_I2S_TX_COMPLETE_CB_ID, &HalTxCplt);
#    else
    s_restartClock = false;
#    endif
}

void I2sModule::SetClockCallbacks()
{
#    if defined(NILAI_I2S_REGISTER_CALLBACKS)
    HAL_I2S_UnRegisterCallback(m_handle, HAL_I2S_TX_HALF_COMPLETE_CB_ID);
    HAL_I2S_RegisterCallback(m_handle, HAL_I2S_TX_COMPLETE_CB_ID, &HalRestartClock);
#    else
    s_restartClock = true;
#    endif
}

void I2sModule::HalRestartClock(I2S_HandleTypeDef* i2s)
{
    static uint16_t s[2] = {0, 0};
    HAL_I2S_Transmit_DMA(i2s, s, 2);
}

I2S::AudioFreqs I2sModule::GetAudioFreq() const
{
    return static_cast<I2S::AudioFreqs>(m_handle->Init.AudioFreq);
}

void I2sModule::SetAudioFreq([[maybe_unused]] I2S::AudioFreqs f)
{
    // TODO this causes the SPI/I2S peripheral to freeze. Maybe setting the registers directly would
    // fix it.

    I2S_WARNING("Setting sampling rate is not supported!");

    //    if (m_isStreaming)
    //    {
    //        // Currently streaming, stop it first.
    //        StopStream();
    //    }
    //
    //    HAL_I2S_DeInit(m_handle);    // De-init the I2S peripheral.
    //
    //    m_handle->Init.AudioFreq = static_cast<uint32_t>(f);
    //    NILAI_ASSERT(HAL_I2S_Init(m_handle) == HAL_OK, "Unable to re-init I2S!");
}

void I2sModule::ReInit()
{
    HAL_I2S_DeInit(m_handle);
    HAL_I2S_Init(m_handle);
    SetStreamingCallbacks();
}

#    if !defined(NILAI_I2S_REGISTER_CALLBACKS)
extern "C" void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef* hi2s)
{
    if (!s_restartClock)
    {
        s_modules[hi2s]->HalTxHalfCplt(hi2s);
    }
}

extern "C" void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef* hi2s)
{
    if (s_restartClock)
    {
        s_modules[hi2s]->HalRestartClock(hi2s);
    }
    else
    {
        s_modules[hi2s]->HalTxCplt(hi2s);
    }
}
#    endif

}    // namespace Nilai::Drivers
#endif
