/**
 * @file    i2sModule.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#include "i2sModule.h"

#if defined(NILAI_USE_I2S)
#    include "../defines/macros.hpp"

#    define I2S_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define I2S_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

std::map<I2S_HandleTypeDef*, I2sModule*> I2sModule::s_modules = {};

I2sModule::I2sModule(I2S_HandleTypeDef* handle, std::string label)
: m_handle(handle), m_label(std::move(label)), m_txHalfCpltCb([]() {}), m_txCpltCb([]() {})
{
    CEP_ASSERT(m_handle != nullptr, "Handle is null!");
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

bool I2sModule::Stream(const uint16_t* samples, size_t cnt)
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

    if (!Restream(const_cast<uint16_t*>(samples), cnt))
    {
        I2S_ERROR("Unable to start streaming!");
        return false;
    }

    m_isStreaming = true;
    return true;
}

bool I2sModule::Restream(const uint16_t* samples, size_t cnt)
{
    m_isStreaming = true;
    return HAL_I2S_Transmit_DMA(m_handle, const_cast<uint16_t*>(samples), cnt) == HAL_OK;
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
    HAL_I2S_RegisterCallback(m_handle, HAL_I2S_TX_HALF_COMPLETE_CB_ID, &HalTxHalfCplt);
    HAL_I2S_RegisterCallback(m_handle, HAL_I2S_TX_COMPLETE_CB_ID, &HalTxCplt);
}

void I2sModule::SetClockCallbacks()
{
    HAL_I2S_UnRegisterCallback(m_handle, HAL_I2S_TX_HALF_COMPLETE_CB_ID);
    HAL_I2S_RegisterCallback(m_handle, HAL_I2S_TX_COMPLETE_CB_ID, &HalRestartClock);
}

void I2sModule::HalRestartClock(I2S_HandleTypeDef* i2s)
{
    static uint16_t s[2] = {0, 0};
    HAL_I2S_Transmit_DMA(i2s, s, 2);
}

cep::I2S::AudioFreqs I2sModule::GetAudioFreq() const
{
    return static_cast<cep::I2S::AudioFreqs>(m_handle->Init.AudioFreq);
}

void I2sModule::SetAudioFreq(cep::I2S::AudioFreqs f)
{
    if (m_isStreaming)
    {
        // Currently streaming, stop it first.
        StopStream();
    }

    HAL_I2S_DeInit(m_handle);    // De-init the I2S peripheral.

    m_handle->Init.AudioFreq = (uint32_t)f;
    CEP_ASSERT(HAL_I2S_Init(m_handle) == HAL_OK, "Unable to re-init I2S!");
}
#endif
