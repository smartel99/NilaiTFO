/**
 * @file    SaiModule.h
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
#ifndef NILAI_SAIMODULE_H
#define NILAI_SAIMODULE_H

#if defined(NILAI_USE_SAI)
#    include "../defines/internal_config.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_SAI_MODULE_ENABLED)

#        include "audio_device.h"
#        include "SAI/enums.h"

#        include <functional>
#        include <string>

#        include <map>

namespace Nilai::Drivers
{
#        if defined(NILAI_USE_EVENTS) && defined(NILAI_USE_SAI_EVENTS)
#            define NILAI_SAI_REGISTER_EVENTS
#        elif USE_HAL_SAI_REGISTER_CALLBACKS == 1
#            define NILAI_SAI_REGISTER_CALLBACKS
#        else
#            define NILAI_SAI_OVERRIDE_HAL
#        endif

class SaiModule : public AudioDevice
{
public:
    SaiModule(SAI_HandleTypeDef* handle,
              std::string        label,
              Sai::Protocol      protocol = {},
              Sai::DataSize      size     = {},
              uint8_t            nbSlot   = {});
    ~SaiModule() override = default;

    bool DoPost() override;
    void Run() override;

    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    [[nodiscard]] Sai::SamplingRate GetSampleRate() const;
    void                            SetSampleRate(Sai::SamplingRate sr);

    void StartClock() override;
    void StopClock() override;

    void SetTxHalfCpltCb(const std::function<void()>& cb) override;
    void SetTxCpltCb(const std::function<void()>& cb) override;

    [[nodiscard]] bool IsStreaming() const override { return m_isStreaming; }

    /**
     * @brief Streams audio data using DMA.
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
    SAI_HandleTypeDef* GetHandle() { return m_handle; }

    void TxHalfCplt();
    void TxCplt();
    void Error();

protected:
    void SetStreamingCallbacks();
    void SetClockCallbacks();
#        if defined(NILAI_SAI_REGISTER_EVENTS)
    void UnregisterEvents();
#        endif

    static void HalRestartClock(SAI_HandleTypeDef* sai);

#        if defined(NILAI_SAI_REGISTER_CALLBACKS)
    static void HalTxHalfCplt(SAI_HandleTypeDef* sai);
    static void HalTxCplt(SAI_HandleTypeDef* sai);
    static void HalError(SAI_HandleTypeDef* sai);
#        endif

protected:
    SAI_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;

    std::function<void()> m_txHalfCpltCb;
    std::function<void()> m_txCpltCb;

    bool m_isStreaming   = false;
    bool m_isClockActive = false;

    Sai::Protocol m_protocol = {};
    Sai::DataSize m_dataSize = {};
    uint8_t       m_numSlot  = 2;

    uint32_t m_lastError = 0;

#        if defined(NILAI_SAI_REGISTER_EVENTS)
    size_t m_txHalfCpltCbId = std::numeric_limits<size_t>::max();
    size_t m_txCpltCbId     = std::numeric_limits<size_t>::max();
#        endif
};
}    // namespace Nilai::Drivers

#    else
#        if defined(WARM_MISSING_STM_DRIVERS)
#            warning NilaiTFO SAI module is enabled, but HAL_SAI_MODULE_ENABLED is undefined!
#        endif
#    endif
#elif defined(NILAI_TEST)
#    include "../test/Mocks/drivers/sai_module.h"
#endif
#endif    // NILAI_SAIMODULE_H
