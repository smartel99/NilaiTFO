/**
 * @file    i2sModule.h
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
#ifndef NILAI_I2SMODULE_H
#define NILAI_I2SMODULE_H

#if defined(NILAI_USE_I2S)
#    include "../defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_I2S_MODULE_ENABLED)
#        include "../defines/module.hpp"
#        include "Core/Inc/i2s.h"

#        include <functional>
#        include <map>

namespace cep::I2S
{
enum class AudioFreqs
{
    f192k   = I2S_AUDIOFREQ_192K,
    f96k    = I2S_AUDIOFREQ_96K,
    f48k    = I2S_AUDIOFREQ_48K,
    f44k1   = I2S_AUDIOFREQ_44K,
    f32k    = I2S_AUDIOFREQ_32K,
    f22k05  = I2S_AUDIOFREQ_22K,
    f16k    = I2S_AUDIOFREQ_16K,
    f11k025 = I2S_AUDIOFREQ_11K,
    f8k     = I2S_AUDIOFREQ_8K,
};

}

class I2sModule : public cep::Module
{
public:
    I2sModule(I2S_HandleTypeDef* handle, std::string label);
    ~I2sModule() override = default;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    [[nodiscard]] cep::I2S::AudioFreqs GetAudioFreq() const;
    void                               SetAudioFreq(cep::I2S::AudioFreqs f);

    void StartClock();
    void StopClock();

    void SetTxHalfCpltCb(const std::function<void()>& cb);
    void SetTxCpltCb(const std::function<void()>& cb);

    [[nodiscard]] bool IsStreaming() const { return m_isStreaming; }
    /**
     * @brief Streams I2S data using DMA.
     * @param samples The samples to send. Must be valid until the DMA transfer is completed.
     * @param cnt The number of samples to send, combining L+R channels.
     * @return True if the stream was successfully started.
     */
    bool Stream(const uint16_t* samples, size_t cnt);
    bool Restream(const uint16_t* samples, size_t cnt);

    /**
     * @brief Pauses an ongoing stream.
     * @return True if successfully paused.
     */
    bool PauseStream();

    /**
     * @brief Resumes an ongoing stream.
     * @return True if successfully resumed.
     */
    bool ResumeStream();

    /**
     * @brief Stops an ongoing stream.
     * @return True if successfully stopped.
     */
    bool StopStream();

private:
    void TxHalfCplt();
    void TxCplt();

    void SetStreamingCallbacks();
    void SetClockCallbacks();

    static void HalTxHalfCplt(I2S_HandleTypeDef* i2s);
    static void HalTxCplt(I2S_HandleTypeDef* i2s);
    static void HalRestartClock(I2S_HandleTypeDef* i2s);

private:
    static std::map<I2S_HandleTypeDef*, I2sModule*> s_modules;

    I2S_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;

    std::function<void()> m_txHalfCpltCb;
    std::function<void()> m_txCpltCb;

    bool m_isStreaming   = false;
    bool m_isClockActive = false;
};

#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO I2S module is enabled, but HAL_I2S_MODULE_ENABLED is undefined!
#        endif
#    endif
#endif
#endif    // NILAI_I2SMODULE_H
