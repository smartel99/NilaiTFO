/**
 * @file    SoftwareConfig.cpp
 * @author  Samuel Martel
 * @date    2022-02-08
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
#include "software_config.h"

#if defined(NILAI_USE_TAS5707)
#    include "../../services/logger.hpp"

#    define TAS_ERROR(msg, ...) LOG_ERROR("[TAS5707]: " msg, ##__VA_ARGS__)

namespace Nilai::Tas5707
{

bool SoftwareConfig::Validate() const
{
    bool isValid = true;

    isValid &= SampleRates::IsSampleRate(SamplingRate);
    isValid &= MclkFrequencies::IsMclkFrequency(MclkFreq);
    isValid &= PwmDcBlock::IsValid(BlockDc);
    isValid &= ClkErrRecoveryModes::IsValid(ClkRecoverMode);
    isValid &= DeEmphasisModes::IsValid(DeEmphasisMode);
    isValid &= SerialDataMode::IsValid(SerialMode);
    isValid &= ChannelEnable::IsValid(ChannelsEnable);
    isValid &= Ch1SoftMute::IsValid(Ch1Mute);
    isValid &= Ch2SoftMute::IsValid(Ch2Mute);
    isValid &= ChannelVolume::IsValid(MasterVolume);
    isValid &= FineVolume::IsValid(MasterFineVolume);
    isValid &= ChannelVolume::IsValid(Ch1Volume);
    isValid &= ChannelVolume::IsValid(Ch2Volume);
    isValid &= VolumeSlewRates::IsValid(VolumeSlewRate);
    isValid &= ModulationLimit::IsValid(MaxPwmDC);
    isValid &= InterchannelDelays::IsValid(Ch1Delay);
    isValid &= InterchannelDelays::IsValid(Ch2Delay);
    isValid &= StartStopPeriods::IsValid(StartStopDelay);
    isValid &= OscillatorTrimSelect::IsValid(OscTrimMode);
    isValid &= BackEndRestartDelays::IsValid(BackEndRestartDelay);
    isValid &= Channel1ModulationModes::IsValid(Ch1ModMode);
    isValid &= Channel2ModulationModes::IsValid(Ch2ModMode);
    isValid &= Channel1InputSources::IsValid(Ch1Source);
    isValid &= Channel2InputSources::IsValid(Ch2Source);
    isValid &= OutASources::IsValid(OutASource);
    isValid &= OutBSources::IsValid(OutBSource);
    isValid &= OutCSources::IsValid(OutCSource);
    isValid &= OutDSources::IsValid(OutDSource);
    isValid &= DrcModes::IsValid(DRC.State);
    isValid &= EqModes::IsValid(UseEqs);
    isValid &= BiquadModes::IsValid(BiquadMode);
    isValid &= BankSwitchingModes::IsValid(BankSwitchMode);
    isValid &= BankSampleRates::IsValid(Bank1Rates);
    isValid &= BankSampleRates::IsValid(Bank2Rates);
    isValid &= BankSampleRates::IsValid(Bank3Rates);

    if (!isValid)
    {
        TAS_ERROR("Configuration is not valid!");
    }
    return isValid;
}

uint8_t SoftwareConfig::ToClockCtrlReg() const
{
    // Sampling rate is bit 5 to 7.
    // MCLK is bit 2 to 4.
    return SamplingRate | MclkFreq;
}
uint8_t SoftwareConfig::ToSysCtrl1Reg() const
{
    return BlockDc | ClkRecoverMode | DeEmphasisMode;
}
uint8_t SoftwareConfig::ToSoftMuteReg() const
{
    return Ch1Mute | Ch2Mute;
}
uint32_t SoftwareConfig::ToInMuxReg() const
{
    return Ch1Source | Ch2Source;
}
uint32_t SoftwareConfig::ToPwmMuxReg() const
{
    return OutASource | OutBSource | OutCSource | OutDSource;
}
uint32_t SoftwareConfig::ToBankSwitchCtrlReg() const
{
    return UseEqs | BiquadMode | BankSwitchMode | BankSampleRates::ToBank1(Bank1Rates) |
           BankSampleRates::ToBank2(Bank2Rates) | BankSampleRates::ToBank3(Bank3Rates);
}

}    // namespace Nilai::Tas5707

#endif