/**
 * @file    SwTas5760.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
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

namespace Nilai::Interfaces
{
template<typename Device>
class SwTas5760 : public Tas5760Module<Device>
{
    using Handle = typename Tas5760Module<Device>::Handle;

public:
    template<typename... Args>
    SwTas5760(const Tas5760::SwConfig& cfg, Handle handle, const std::string& label, Args... args)
    : Tas5760Module<Device>(handle, label, args...), m_cfg(cfg)
    {
        Build(cfg);
    }
    void Build(const Tas5760::SwConfig& cfg);

    ~SwTas5760() override = default;

    void Run() override;
    bool DoPost() override;

    void ToggleMute(bool s) override;

    void               ToggleSleep(bool s) override;
    [[nodiscard]] bool IsAsleep() const override;

    void               ToggleShutdown(bool s) override;
    [[nodiscard]] bool IsShutdown() const override;

    void SetChannelVolume(Tas5760::Channels ch, uint8_t vol);

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

    bool               SetRegister(Tas5760::Registers r, uint8_t v);
    static std::string StatusToStr(uint8_t s);

private:
    Tas5760::SwConfig m_cfg;

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
#    endif
#endif

#endif    // NILAI_TAS5760_SWTAS5760_H
