/**
 * @file    enums.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#ifndef NILAI_SAI_ENUMS_H
#define NILAI_SAI_ENUMS_H

#if defined(NILAI_USE_SAI)
#    include "../../defines/internal_config.h"
#    include NILAI_HAL_HEADER

namespace Nilai::Sai
{
/**
 * @brief Mode of the SAI block.
 *
 * A Master is in charge of driving the clock signals.
 * A Slave receives the clock signals from another source.
 *
 * A Transmitter is in charge of sending the data on the bus.
 * A Receiver receives the data from the bus.
 */
enum class AudioMode : uint32_t
{
    MasterTx = SAI_MODEMASTER_TX,    //! Mode: Master, transmitter.
    MasterRx = SAI_MODEMASTER_RX,    //! Mode: Master, receiver.
    SlaveTx  = SAI_MODESLAVE_TX,     //! Mode: Slave, transmitter.
    SlaveRx  = SAI_MODESLAVE_RX,     //! Mode: Slave, receiver.
};

/**
 * @brief Method of synchronisation between blocks/peripheral.
 */
enum class SyncMode : uint32_t
{
    Asynchronous = SAI_ASYNCHRONOUS,    //! Not synchronised.
    Synchronous =
      SAI_SYNCHRONOUS,    //! Synchronised with the other block of the same SAI peripheral.
    SynchronousExtSai1 = SAI_SYNCHRONOUS_EXT_SAI1,    //! Synchronised with SAI1.
    SynchronousExtSai2 = SAI_SYNCHRONOUS_EXT_SAI2,    //! Synchronised with SAI2.
};

/**
 * @brief Selects whether the outputs of the SAI block are driven or not.
 */
enum class OutputDrive : uint32_t
{
    Disable = SAI_OUTPUTDRIVE_DISABLE,    //! Output drive disabled.
    Enable  = SAI_OUTPUTDRIVE_ENABLE,     //! Output drive enabled.
};

/**
 * @brief Selects whether the master clock is divided or not.
 */
enum class MclkDivider : uint32_t
{
    Disable = SAI_MASTERDIVIDER_DISABLE,    //! Master clock division disabled.
    Enable  = SAI_MASTERDIVIDER_ENABLE,     //! Master clock division enabled.
};

/**
 * @brief Threshold level of the FIFO.
 *
 * TODO: Find out what this threshold is for.
 */
enum class FifoThreshold : uint32_t
{
    Empty            = SAI_FIFOTHRESHOLD_EMPTY,    //! Triggers when FIFO is empty.
    QuarterFull      = SAI_FIFOTHRESHOLD_1QF,      //! Triggers when FIFO is 25% full.
    HalfFull         = SAI_FIFOTHRESHOLD_HF,       //! Triggers when FIFO is 50% full.
    ThreeQuarterFull = SAI_FIFOTHRESHOLD_3QF,      //! Triggers when FIFO is 75% full.
    Full             = SAI_FIFOTHRESHOLD_FULL,     //! Triggers when FIFO is 100% full.
};

/**
 * @brief Sampling rate of the data.
 */
enum class SamplingRate : uint32_t
{
    sr192k = SAI_AUDIO_FREQUENCY_192K,      //! Sampling rate is 192kHz.
    sr96k  = SAI_AUDIO_FREQUENCY_96K,       //! Sampling rate is 96kHz.
    sr48k  = SAI_AUDIO_FREQUENCY_48K,       //! Sampling rate is 48kHz.
    sr44k  = SAI_AUDIO_FREQUENCY_44K,       //! Sampling rate is 44kHz.
    sr32k  = SAI_AUDIO_FREQUENCY_32K,       //! Sampling rate is 32kHz.
    sr22k  = SAI_AUDIO_FREQUENCY_22K,       //! Sampling rate is 22kHz.
    sr16k  = SAI_AUDIO_FREQUENCY_16K,       //! Sampling rate is 16kHz.
    sr11k  = SAI_AUDIO_FREQUENCY_11K,       //! Sampling rate is 11kHz.
    sr8k   = SAI_AUDIO_FREQUENCY_8K,        //! Sampling rate is 8kHz.
    mckDiv = SAI_AUDIO_FREQUENCY_MCKDIV,    //! TODO What's this?
};

/**
 * @brief Source of the synchronisation.
 * This is used to select the "master" (source) that directs the synchronisation.
 *
 * @note This value should be the same for Block A and Block B if both blocks are in the same
 * peripheral.
 */
enum class SyncSource : uint32_t
{
    Disable = SAI_SYNCEXT_DISABLE,             //! Synchronisation disabled.
    BlockA  = SAI_SYNCEXT_OUTBLOCKA_ENABLE,    //! Synchronisation driven by block A.
    BlockB  = SAI_SYNCEXT_OUTBLOCKB_ENABLE,    //! Synchronisation driven by block B.
};

/**
 * @brief Content of a two-slot frame.
 */
enum class FrameContent : uint32_t
{
    Mono  = SAI_MONOMODE,      //! Frame contains mono data.
    Stero = SAI_STEREOMODE,    //! Frame contains stereo data.
};

/**
 * @brief The companding method used.
 */
enum class CompandingMethod : uint32_t
{
    None     = SAI_NOCOMPANDING,            //! No companding used.
    Ulaw1Cpl = SAI_ULAW_1CPL_COMPANDING,    //! u-Law companding, 1's complement.
    Alaw1Cpl = SAI_ALAW_1CPL_COMPANDING,    //! a-Law companding, 1's complement.
    Ulaw2Cpl = SAI_ULAW_2CPL_COMPANDING,    //! u-Law companding, 2's complement.
    Alaw2Cpl = SAI_ALAW_2CPL_COMPANDING,    //! a-Law companding, 2's complement.
};

/**
 * @brief The method of signal release used.
 */
enum class ReleaseMethod : uint32_t
{
    NotReleased       = SAI_OUTPUT_NOTRELEASED,    //! Output is not released?
    ReleasedFromPress = SAI_OUTPUT_RELEASED,       //! Output is released?
};

/**
 * @brief The protocol used by the peripheral.
 */
enum class Protocol : uint32_t
{
    I2sStandard     = SAI_I2S_STANDARD,        //! Standard I2S mode.
    I2sMsbJustified = SAI_I2S_MSBJUSTIFIED,    //! I2S mode, MSB justified.
    I2sLsbJustified = SAI_I2S_LSBJUSTIFIED,    //! I2S mode, LSB justified.
    PcmLong         = SAI_PCM_LONG,            //! PCM mode, long.
    PcmShort        = SAI_PCM_SHORT,           //! PCM mode, short.
};

/**
 * @brief Number of bits contained in a sample.
 */
enum class DataSize : uint32_t
{
    Bit16    = SAI_PROTOCOL_DATASIZE_16BIT,            //! 16 bits per sample.
    Bit16Ext = SAI_PROTOCOL_DATASIZE_16BITEXTENDED,    //! 16 bits per sample in a 32 bits frame.
    Bit24    = SAI_PROTOCOL_DATASIZE_24BIT,            //! 24 bits per sample in a 32 bits frame.
    Bit32    = SAI_PROTOCOL_DATASIZE_32BIT,            //! 32 bits per sample.
};
}    // namespace Nilai::Sai
#endif

#endif    // NILAI_ENUMS_H
