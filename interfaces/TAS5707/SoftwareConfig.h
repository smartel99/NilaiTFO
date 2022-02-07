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

#include "Enums.h"

namespace cep::Tas5707
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
    //! Serial Data Mode.
    uint8_t SerialMode = SerialDataMode::I2S24Bits;
    //! Toggles the output channels.
    uint8_t ChannelsEnable = ChannelEnable::Disable;
    //! Soft mute control.
};
}    // namespace cep::Tas5707

#endif

#endif    // NILAI_TAS5707_SOFTWARECONFIG_H
