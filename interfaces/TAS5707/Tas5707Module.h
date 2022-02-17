/**
 * @file    Tas5707Module.h
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
#ifndef NILAI_TAS5707MODULE_H
#define NILAI_TAS5707MODULE_H

#if defined(NILAI_USE_TAS5707)
#    include "../../defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_I2S_MODULE_ENABLED) && defined(HAL_I2C_MODULE_ENABLED)
#        include "../../defines/module.hpp"
#        include "../../drivers/i2cModule.hpp"
#        include "../../drivers/i2sModule.h"

#        include "HardwareConfig.h"
#        include "SoftwareConfig.h"

class Tas5707Module : public I2sModule
{
public:
    /**
     * @brief Initializes the TAS5707 digital amplifier using the provided parameters.
     *
     * The initialization sequence goes as follows:
     * <ol>
     *  <li> Hold all digital inputs low and ramp up AVDD/DVDD to at least 3V (should already be
     * done if using the same supply as the STM32).</li>
     *  <li> Initialize digital inputs and PVDD supply as follows:
     *      <ul>
     *          <li>Drive RESET=0, PDN=1, and other digital inputs to their desired state.</li>
     *          <li>Provide stable and valid I2S clocks (MCLK, LRCLK and SCLK) then wait 100us</li>
     *          <li>Drive RESET=1, then wait another 13.5ms</li>
     *          <li>Ramp up PVDD to at least 8V while ensuring that it remains below 6V for at least
     * 100us after AVDD/DVDD reaches 3V, then wait at least another 10us</li>
     *      </ul>
     *  </li>
     *  <li>Identify the I2C address to use (0x36 for the TAS5707, 0x3A for the TAS5707A)</li>
     *  <li>Trim oscillator (write 0x00 to register 0x1B) and wait at least 50ms.</li>
     *  <li>Configure the DAP via I2C:
     *      <ul>
     *          <li>Biquads (0x29-0x36)</li>
     *          <li>DRC parameters (0x3A-0x3C, 0x40-0x42 and 0x46)</li>
     *          <li>Bank select (0x50)</li>
     *      </ul>
     *  </li>
     *  <li>Configure remaining registers</li>
     *  <li>Exit shutdown mode:
     *      <ol>
     *          <li>Ensure I2S clocks have been stable and valid for at least 50ms</li>
     *          <li>Write 0x00 to register 0x05 (it may take up to 240ms for the exit shutdown to be
     * executed by the chip following a power-up)</li>
     *          <li>Wait at least 1ms+1.3*Tstart, where Tstart is specified by register 0x1A)</li>
     *          <li>Proceed with normal operation</li>
     *      </ol></li>
     * </ol>
     * @param hw
     * @param sw
     * @param label
     */
    Tas5707Module(const cep::Tas5707::HardwareConfig& hw,
                  const cep::Tas5707::SoftwareConfig& sw,
                  I2S_HandleTypeDef*                  i2s,
                  std::string                         label);
    ~Tas5707Module() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    void ToggleSoftMute(bool ch1, bool ch2);
    void SetMasterVolume(uint8_t vol);
    void SetChannelVolume(cep::Tas5707::Channels ch, uint8_t vol);

    void SendConfiguration();
    bool Enable();
    bool Disable();

    void SetBiquadFilters(const cep::Tas5707::BiquadBanks& ch1,
                          const cep::Tas5707::BiquadBanks& ch2 = {});
    void SetDynamicRangeCtrl(const cep::Tas5707::DynamicRangeControl& drc);
    void SetBankSwitchingMode(uint32_t mode);
    void SetSerialDataInterface(uint8_t in);
    void SetSysCtrl1Reg(uint8_t dcBlock, uint8_t muteRecover, uint8_t deEmphasis);
    void SetChannelInputAndModes(uint32_t ch1Mode,
                                 uint32_t ch1Source,
                                 uint32_t ch2Mode,
                                 uint32_t ch2Source);
    void SetChannelOutputs(uint32_t ASrc, uint32_t BSrc, uint32_t CSrc, uint32_t DSrc);

private:
    bool FindTas5707I2CAddr();
    void TrimOscillator();

    cep::Tas5707::HardwareConfig m_hw = {};
    cep::Tas5707::SoftwareConfig m_sw = {};
    std::string                  m_label;

    //! I2C address of the TAS5707 on the bus.
    uint8_t m_i2cAddr = 0;

    //! Time at which the chip finished trimming its PLL.
    uint32_t m_initStartTime = 0;

    //! Minimum time required between oscillator trimming and shutdown request, 240ms as per the
    //! datasheet.
    static constexpr uint8_t s_pllTime = 240;
};

#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO I2S module is enabled, but HAL_I2S_MODULE_ENABLED is undefined!
#        endif
#    endif
#endif



#endif    // NILAI_TAS5707MODULE_H
