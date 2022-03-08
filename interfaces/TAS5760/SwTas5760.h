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

#    include "Tas5760Module.h"

#    include "SwConfig.h"

#include <functional>

class SwTas5760 : public Tas5760Module
{
public:
    SwTas5760(const cep::Tas5760::SwConfig& cfg, I2S_HandleTypeDef* i2s, const std::string& label);
    ~SwTas5760() override = default;

    void Run() override;
    bool DoPost() override;

    void               ToggleSleep(bool s) override;
    [[nodiscard]] bool IsAsleep() const override;

    void               ToggleShutdown(bool s) override;
    [[nodiscard]] bool IsShutdown() const override;

    void SetChannelVolume(cep::Tas5760::Channels ch, uint8_t vol);

    bool SetDigitalClipLevel(uint32_t lvl);

private:
    bool Init();

    void HandleFault();
    void HandleHeadphoneChange();

    bool SetRegister(cep::Tas5760::Registers r, uint8_t v);
private:
    cep::Tas5760::SwConfig m_cfg;

    std::function<void()> m_faultFunction = [](){};
    std::function<void()> m_hpChangeFunction = [](){};

    bool m_isShutdown = false;

    //! Chip has fault.
    bool m_hasFault = false;
    //! Headphones connected/disconnected.
    bool m_hpChanged = false;

    //! Volume for the left channel, 0xCF -> +0dB.
    uint8_t m_lVol = s_defaultVolume;
    //! Volume for the right channel, 0xCF -> +0dB.
    uint8_t m_rVol = s_defaultVolume;

private:
    static constexpr uint8_t s_bothChMuted = 0x03;
    static constexpr uint8_t s_defaultVolume = 0xCF;
};

#endif

#endif    // NILAI_TAS5760_SWTAS5760_H
