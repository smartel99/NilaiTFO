/**
 * @file    SoftwareConfig.h
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
#ifndef NILAI_TAS5707_SOFTWARECONFIG_H
#define NILAI_TAS5707_SOFTWARECONFIG_H

#if defined(NILAI_USE_TAS5707)

#    include "Enums.h"
#    include "Misc.h"

#    include <array>

namespace Nilai::Tas5707
{
/**
 * @brief Structure describing the software (register) configuration of the TAS5705 digital
 * amplifier.
 */
struct SoftwareConfig
{
    //! Sampling rate of the audio being streamed in.
    uint8_t SamplingRate = SampleRates::Sr44_1Or48kHz;
    //! Frequency of the MCLK signal.
    uint8_t MclkFreq = MclkFrequencies::Fs256;
    //! PWM high-pass (DC blocking).
    uint8_t BlockDc = PwmDcBlock::Enabled;
    //! Mode of recovery after a clock error.
    uint8_t ClkRecoverMode = ClkErrRecoveryModes::Hard;
    //! Toggles de-emphasis.
    uint8_t DeEmphasisMode = DeEmphasisModes::None;
    //! Serial Data Mode.
    uint8_t SerialMode = SerialDataMode::I2S24Bits;
    //! Toggles the output channels.
    uint8_t ChannelsEnable = ChannelEnable::Disable;
    //! Channel 1 soft mute.
    uint8_t Ch1Mute = Ch1SoftMute::Unmute;
    //! Channel 2 soft mute.
    uint8_t Ch2Mute = Ch2SoftMute::Unmute;
    //! Master volume control.
    uint8_t MasterVolume = ChannelVolume::Muted;
    //! Master volume fine control.
    uint8_t MasterFineVolume = FineVolume::DB0;
    //! Channel 1 volume control.
    uint8_t Ch1Volume = ChannelVolume::NoAttenuation;
    //! Channel 2 volume control.
    uint8_t Ch2Volume = ChannelVolume::NoAttenuation;
    //! Volume change rate.
    uint8_t VolumeSlewRate = VolumeSlewRates::Steps1024;
    //! Maximum PWM duty cycle.
    uint8_t MaxPwmDC = ModulationLimit::P97_7;
    //! Inter-channel delay for PWM channel 1. Messing this value up can impact performance!
    uint8_t Ch1Delay = InterchannelDelays::Ch1ADMode;
    //! Inter-channel delay for PWM channel 1'. Messing this value up can impact performance!
    uint8_t Ch1PDelay = InterchannelDelays::Ch1PADMode;
    //! Inter-channel delay for PWM channel 2. Messing this value up can impact performance!
    uint8_t Ch2Delay = InterchannelDelays::Ch2ADMode;
    //! Inter-channel delay for PWM channel 2'. Messing this value up can impact performance!
    uint8_t Ch2PDelay = InterchannelDelays::Ch2PADMode;
    //! Start/Stop period, in milliseconds.
    uint8_t StartStopDelay = StartStopPeriods::Ms125_7;
    //! Controls the internal oscillator trimming.
    uint8_t OscTrimMode = OscillatorTrimSelect::Disabled;
    //! Delay before restarting the power-stage after a power error.
    uint8_t BackEndRestartDelay = BackEndRestartDelays::Ms299;
    //! Modulation mode for channel 1.
    uint32_t Ch1ModMode = Channel1ModulationModes::AD;
    //! Modulation mode for channel 2.
    uint32_t Ch2ModMode = Channel2ModulationModes::AD;
    //! Signal source for channel 1.
    uint32_t Ch1Source = Channel1InputSources::SDIN_L;
    //! Signal source for channel 2.
    uint32_t Ch2Source = Channel2InputSources::SDIN_R;
    //! PWM source of OUT_A.
    uint32_t OutASource = OutASources::Ch1;
    //! PWM source of OUT_B.
    uint32_t OutBSource = OutBSources::Ch1P;
    //! PWM source of OUT_C.
    uint32_t OutCSource = OutCSources::Ch2;
    //! PWM source of OUT_D.
    uint32_t OutDSource = OutDSources::Ch2P;
    //! Toggles the EQ filters.
    uint32_t UseEqs = EqModes::Enabled;
    //! Switch the behavior of the biquad filters.
    uint32_t BiquadMode = BiquadModes::Independent;
    //! Bank switching mode.
    uint32_t BankSwitchMode = BankSwitchingModes::NoSwitch;
    //! Which sample rates are mapped to bank 1. Only effective when BankSwitchMode is set to
    //! BankSwitchingModes::Automatic.
    uint8_t Bank1Rates = BankSampleRates::Bank1Default;
    //! Which sample rates are mapped to bank 2. Only effective when BankSwitchMode is set to
    //! BankSwitchingModes::Automatic.
    uint8_t Bank2Rates = BankSampleRates::Bank2Default;
    //! Which sample rates are mapped to bank 3. Only effective when BankSwitchMode is set to
    //! BankSwitchingModes::Automatic.
    uint8_t Bank3Rates = BankSampleRates::Bank3Default;

    //! Biquad filters for channel 1. If BankSwitchMode is set to NoSwitch, only bank1 should be
    //! used.
    BiquadBanks Ch1BiquadFilters = {};
    //! Biquad filters for channel 2. If BankSwitchMode is set to NoSwitch, only bank1 should be
    //! used. If BiquadMode is set to Ganged, the filters of channel 2 are taken from channel 1.
    //! TODO don't use this space if BiquadMode is Ganged
    BiquadBanks Ch2BiquadFilters = {};

    DynamicRangeControl DRC = {};

    //! I2C address of the device.
    //! The TAS5707's default address is 0x36, which can be changed to 0x38.
    //! The TAS5707A's default address is 0x3A, which can be changed to 0x3A.
    uint8_t I2cAddr = 0x36;

    [[nodiscard]] bool Validate() const;

    /**
     * @brief Combines the sample rate and the MCLK frequency.
     * @return The value to write in the ClkCtrl register.
     */
    [[nodiscard]] uint8_t ToClockCtrlReg() const;
    /**
     * @brief Combines the dc blocking filter, clock recovery mode and the de-emphasis mode.
     * @return The value to write in the SysCtrl1 register.
     */
    [[nodiscard]] uint8_t ToSysCtrl1Reg() const;
    /**
     * @brief Combines the soft mutes for channel 1 and 2.
     * @return The value to write in the SoftMute register.
     */
    [[nodiscard]] uint8_t ToSoftMuteReg() const;
    /**
     * @brief Combines the sources for channel 1 and 2.
     * @return The value to write in the InMux register.
     */
    [[nodiscard]] uint32_t ToInMuxReg() const;
    /**
     * @brief Combines the sources for OUT_A, OUT_B, OUT_C and OUT_D.
     * @return The value to write in the PwmMux register.
     */
    [[nodiscard]] uint32_t ToPwmMuxReg() const;
    /**
     * @brief Combines the sample rates for each banks, the EQ status, the bank mapping and the bank
     * switching mode.
     * @return The value to write in the BankSwitchCtrl register.
     */
    [[nodiscard]] uint32_t ToBankSwitchCtrlReg() const;
};
}    // namespace Nilai::Tas5707

#endif

#endif    // NILAI_TAS5707_SOFTWARECONFIG_H
