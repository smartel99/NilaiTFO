/**
 * @file    i2s_module.h
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
#ifndef NILAI_I2SMODULE_H
#define NILAI_I2SMODULE_H

#if defined(NILAI_USE_I2S)
#    if defined(NILAI_TEST)
#    else
#        include "../defines/internal_config.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_I2S_MODULE_ENABLED)
#            include "audio_device.h"
#            include "I2S/enums.h"

#            include <functional>
#            include <map>
#            include <string>

#            if USE_HAL_I2S_REGISTER_CALLBACKS == 1
#                define NILAI_I2S_REGISTER_CALLBACKS
#            endif

namespace Nilai::Drivers
{
class I2sModule : public AudioDevice
{
public:
     I2sModule(I2S_HandleTypeDef* handle, std::string label);
    ~I2sModule() override = default;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    [[nodiscard]] I2S::AudioFreqs GetAudioFreq() const;
    void                          SetAudioFreq(I2S::AudioFreqs f);

    void StartClock() override;
    void StopClock() override;

    void SetTxHalfCpltCb(const std::function<void()>& cb) override;
    void SetTxCpltCb(const std::function<void()>& cb) override;

    [[nodiscard]] bool IsStreaming() const override { return m_isStreaming; }
    /**
     * @brief Streams I2S data using DMA.
     * @param samples The samples to send. Must be valid until the DMA transfer is completed.
     * @param cnt The number of samples to send, combining L+R channels.
     * @return True if the stream was successfully started.
     */
    bool Stream(const void* samples, size_t cnt) override;
    bool Restream(const void* samples, size_t cnt) override;

    /**
     * @brief Pauses an ongoing stream.
     * @return True if successfully paused.
     */
    bool PauseStream() override;

    /**
     * @brief Resumes an ongoing stream.
     * @return True if successfully resumed.
     */
    bool ResumeStream() override;

    /**
     * @brief Stops an ongoing stream.
     * @return True if successfully stopped.
     */
    bool StopStream() override;

    /**
     * @brief Helper function to retrieve the HAL handle.
     */
    I2S_HandleTypeDef* GetHandle() { return m_handle; }

    void ReInit();

    static void HalTxHalfCplt(I2S_HandleTypeDef* i2s);
    static void HalTxCplt(I2S_HandleTypeDef* i2s);
    static void HalRestartClock(I2S_HandleTypeDef* i2s);

protected:
    void TxHalfCplt();
    void TxCplt();

    void SetStreamingCallbacks();
    void SetClockCallbacks();

protected:
    I2S_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;

    std::function<void()> m_txHalfCpltCb;
    std::function<void()> m_txCpltCb;

    bool m_isStreaming   = false;
    bool m_isClockActive = false;
};
}    // namespace Nilai::Drivers
#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO I2S module is enabled, but HAL_I2S_MODULE_ENABLED is undefined!
#            endif
#        endif
#    endif
#endif
#endif    // NILAI_I2SMODULE_H
