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
#    include "../defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_SAI_MODULE_ENABLED)

#        if defined(NILAI_USE_EVENTS)
#            include "../defines/Events/Events.h"
#        endif

#        include "../defines/module.hpp"
#        include "SAI/Enums.h"

#        include <functional>

#        if !defined(NILAI_USE_SAI_EVENTS)
#            include <map>
#        endif

namespace Nilai::Drivers
{
class SaiModule : public cep::Module
{
public:
    SaiModule(SAI_HandleTypeDef* handle,
              Sai::Protocol      protocol,
              Sai::DataSize      size,
              uint8_t            nbSlot,
              std::string        label);
    ~SaiModule() override = default;

#        if defined(NILAI_USE_EVENTS)
    bool OnEvent(cep::Events::Event* event) override;
#        endif

    bool DoPost() override;
    void Run() override;

    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    [[nodiscard]] Sai::SamplingRate GetSampleRate() const;
    void                            SetSampleRate(Sai::SamplingRate sr);

    void StartClock();
    void StopClock();

    void SetTxHalfCpltCb(const std::function<void()>& cb);
    void SetTxCpltCb(const std::function<void()>& cb);

    [[nodiscard]] bool IsStreaming() const { return m_isStreaming; }

    /**
     * @brief Streams audio data using DMA.
     * @param samples The samples to send. Must be valid until the DMA transfer is completed.
     * @param cnt The number of samples to send, combining L+R channels.
     * @return True if the stream was successfully started.
     */
    virtual bool Stream(const uint8_t* samples, size_t cnt);
    virtual bool Restream(const uint8_t* samples, size_t cnt);

    /**
     * @brief Pauses an ongoing stream.
     * @return True if successfully paused.
     */
    virtual bool PauseStream();

    /**
     * @brief Resumes an ongoing stream.
     * @return True if successfully resumed.
     */
    virtual bool ResumeStream();

    /**
     * @brief Stops an ongoing stream.
     * @return True if successfully stopped.
     */
    virtual bool StopStream();

protected:
    void TxHalfCplt();
    void TxCplt();

    void SetStreamingCallbacks();
    void SetClockCallbacks();

    static void HalRestartClock(SAI_HandleTypeDef* sai);

#        if !defined(NILAI_USE_SAI_EVENTS)
    static void HalTxHalfCplt(SAI_HandleTypeDef* sai);
    static void HalTxCplt(SAI_HandleTypeDef* sai);
#        endif

protected:
#        if !defined(NILAI_USE_SAI_EVENTS)
    static std::map<SAI_HandleTypeDef*, SaiModule*> s_modules;
#        endif

    SAI_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;

    std::function<void()> m_txHalfCpltCb;
    std::function<void()> m_txCpltCb;

    bool m_isStreaming   = false;
    bool m_isClockActive = false;

    Sai::Protocol m_protocol;
    Sai::DataSize m_dataSize;
    uint8_t       m_numSlot;

    uint32_t m_lastError = 0;

#        if defined(NILAI_USE_SAI_EVENTS)
    std::function<void()> m_eventTxHalfCpltCb;
    std::function<void()> m_eventTxCpltCb;
#        endif
};
}    // namespace Nilai::Drivers

#    else
#        if defined(WARM_MISSING_STM_DRIVERS)
#            warning NilaiTFO SAI module is enabled, but HAL_SAI_MODULE_ENABLED is undefined!
#        endif
#    endif
#elif defined(NILAI_TEST)
#    include "../test/Mocks/drivers/SaiModule.h"
#endif
#endif    // NILAI_SAIMODULE_H
