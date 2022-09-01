/**
 * @file    sw_tas5760.h
 * @author  Samuel Martel
 * @date    2022-03-03
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
#ifndef NILAI_TAS5760_SWTAS5760_H
#define NILAI_TAS5760_SWTAS5760_H

#if defined(NILAI_USE_SW_TAS5760)
#    if defined(NILAI_TEST)
#    else
#        include "tas5760_module.h"

#        include "sw_config.h"

#        if defined(NILAI_USE_EVENTS)
#            include "../../defines/events/events.h"
#        endif

#        include <functional>
#        include <string>

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup TAS5760
 * @{
 */

namespace Nilai::Interfaces
{
/**
 * @class SwTas5760
 * @brief Class representing a TAS5760 digital audio amplifier controlled by software.
 * @tparam Device A type capable of streaming audio to the TAS5760.
 *
 * Refer to the <a
 * href=https://www.ti.com/lit/ds/symlink/tas5760md.pdf?ts=1660571967342&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTAS5760MD>
 * datasheet</a> for more information.
 */
template<typename Device>
class SwTas5760 : public Tas5760Module<Device>
{
    using Handle = typename Tas5760Module<Device>::Handle;

public:
    template<typename... Args>
    SwTas5760(const TAS5760::SwConfig& cfg, Handle handle, const std::string& label, Args&&... args)
    : Tas5760Module<Device>(handle, label, std::forward<Args>(args)...), m_cfg(cfg)
    {
        Build(cfg);
    }
    SwTas5760(const SwTas5760&)            = delete;
    SwTas5760(SwTas5760&&)                 = delete;
    SwTas5760& operator=(const SwTas5760&) = delete;
    SwTas5760& operator=(SwTas5760&&)      = delete;
    /**
     * @brief Initializes the TAS5760 with the passed configuration.
     * @param cfg The desired configuration of the TAS5760.
     */
    void Build(const TAS5760::SwConfig& cfg);

    ~SwTas5760() override = default;

    void Run() override;
    bool DoPost() override;

    void ToggleMute(bool s) override;

    void               ToggleSleep(bool s) override;
    [[nodiscard]] bool IsAsleep() const override;

    void               ToggleShutdown(bool s) override;
    [[nodiscard]] bool IsShutdown() const override;

    /**
     * @brief Sets the volume of the desired channel.
     * @param ch The desired channel to affect.
     * @param vol The new volume.
     */
    void SetChannelVolume(TAS5760::Channels ch, uint8_t vol);

    /**
     * @brief Sets the digital clipping level of the TAS5760.
     * @param lvl The desired clipping level.
     * @returns True if the operation succeeded.
     * @returns False if the operation failed.
     */
    bool SetDigitalClipLevel(uint32_t lvl);

    bool Stream(const void* samples, size_t cnt) override;
    bool PauseStream() override;
    bool ResumeStream() override;
    bool StopStream() override;

private:
    bool Init();
    bool Deinit();

    void HandleFault();
    void HandleHeadphoneChange();

    bool                      SetRegister(TAS5760::Registers r, uint8_t v);
    static const std::string& StatusToStr(uint8_t s);

private:
    TAS5760::SwConfig m_cfg;

    std::function<void()> m_faultFunction    = []() {};
    std::function<void()> m_hpChangeFunction = []() {};

#        if defined(NILAI_USE_EVENTS)
    Events::EventTypes    m_faultEvent       = Events::EventTypes::Exti_Generic;
    Events::EventTypes    m_hpChangeEvent    = Events::EventTypes::Exti_Generic;
#        endif

    bool m_isShutdown = false;

    uint32_t m_faultTime = 0;
    //! Chip has fault.
    bool m_hasFault = false;
    //! Headphones connected/disconnected.
    bool m_hpChanged = false;

    //! Volume for the left channel, 0xCF -> +0dB.
    uint8_t m_lVol = s_defaultVolume;
    //! Volume for the right channel, 0xCF -> +0dB.
    uint8_t m_rVol = s_defaultVolume;

private:
    static constexpr uint8_t s_bothChMuted   = 0x03;
    static constexpr uint8_t s_defaultVolume = 0xCF;

    //! Clock error mask.
    static constexpr uint8_t s_clkeMask = 0x08;
    //! Over current error mask.
    static constexpr uint8_t s_oceMask = 0x04;
    //! DC offset error mask.
    static constexpr uint8_t s_dceMask = 0x02;
    //! Over temperature error mask.
    static constexpr uint8_t s_oteMask = 0x01;
    //! OCE, DCE and OTE are latching errors.
    static constexpr uint8_t s_latchingErrors = 0x07;

    //! Number of milliseconds the fault pin needs to be active to go check the error.
    static constexpr uint32_t s_minErrorTime = 2;
};
}    // namespace Nilai::Interfaces

//!@}
//!@}
//!@}
#    endif
#endif

#endif    // NILAI_TAS5760_SWTAS5760_H
