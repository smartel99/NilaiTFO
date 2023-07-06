/**
 * @file    tas5707_module.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#if defined(NILAI_USE_TAS5707)
#    include "Tas5707Module.h"


#    include "Enums.h"

#    include "../../services/logger.hpp"

#    define TAS_DEBUG(msg, ...) LOG_DEBUG("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define TAS_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define TAS_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

/**
 * @brief Updates the value of a register.
 * @tparam T Can be uint8_t or uint32_t
 * @param i2c I2C handle pointer
 * @param addr I2C address of the chip.
 * @param reg Address of the register to update
 * @param val The new value
 * @param bits The bits to update in the register (defaults to all bits)
 * @return True if successful, false otherwise.
 */
template<typename T>
static bool UpdateRegisterValue(
  I2cModule* i2c, uint8_t addr, uint8_t reg, T val, T bits = std::numeric_limits<T>::max())
{
    T regVal =
      Nilai::VectorToVal<T, true>(i2c->ReceiveFrameFromRegister(addr, reg, sizeof(T)).data);

    // Update the value.
    regVal = (regVal & ~bits) | val;

    i2c->TransmitFrameToRegister(addr, reg, Nilai::ValToVector<true>(regVal));

#    if defined(DEBUG)
    // Verify the data.
    T rb = Nilai::VectorToVal<T, true>(i2c->ReceiveFrameFromRegister(addr, reg, sizeof(T)).data);

    if (rb != regVal)
    {
        return false;
    }
#    endif

    return true;
}

static uint8_t  VolumeToReg(uint8_t vol);
static uint32_t StartStopRegValToTime(uint8_t ss);


Tas5707Module::Tas5707Module(const Nilai::Tas5707::HardwareConfig& hw,
                             const Nilai::Tas5707::SoftwareConfig& sw,
                             I2S_HandleTypeDef*                    i2s,
                             std::string                           label)
: I2sModule(i2s, "i2s"), m_hw(hw), m_sw(sw), m_label(std::move(label))
{
    CEP_ASSERT(m_hw.IsValid(), "Hardware configuration is not valid");
    CEP_ASSERT(m_sw.Validate(), "Software configuration is not valid");

    // Set all digital inputs to low first.
    m_hw.PVddEn.Set(false);
    m_hw.PwrDwn.Set(false);
    m_hw.Reset.Set(false);
    m_hw.Mute.Set(false);
    m_hw.HeadphoneSelect.Set(false);

    // Drive PwrDwn to high and keep Reset low, mute low (no mute), headphone select low (speaker
    // mode).
    m_hw.PwrDwn.Set(true);

    // Start generating the I2S clocks and wait a bit (>100us).
    StartClock();
    HAL_Delay(1);

    // We now release the reset and wait a minimum of 13.5ms.
    m_hw.Reset.Set(true);
    HAL_Delay(14);

    // Enable PVDD and leave it some time to settle. The chip needs at least 10us after PVDD
    // stablizes.
    m_hw.PVddEn.Set(true);
    HAL_Delay(4);    // TODO Tune the PVDD delay.

    // Identify the I2C address of the chip, which varies by models.
    if (!FindTas5707I2CAddr())
    {
        TAS_ERROR("Unable to find TAS5707 on the I2C bus!");
        // Shut it down, to be sure nothing explodes.
        m_hw.PwrDwn.Set(false);
        m_hw.Reset.Set(false);
        m_hw.PVddEn.Set(false);
        CEP_ASSERT(false, "Unable to configure TAS5707");
        return;
    }

    // Tell the chip to trim its internal oscillator.
    TrimOscillator();
    m_initStartTime = HAL_GetTick();

    SendConfiguration();
    Enable();

    // PLL must have been trimmed for >240ms before using the chip.
    if ((HAL_GetTick() - m_initStartTime) < 240)
    {
        HAL_Delay(s_pllTime - (HAL_GetTick() - m_initStartTime));
    }

    TAS_INFO("Initialized.");
}

Tas5707Module::~Tas5707Module()
{
}

bool Tas5707Module::DoPost()
{
    // Nothing really to do here.
    TAS_INFO("POST OK");
    return true;
}

void Tas5707Module::Run()
{
}


bool Tas5707Module::FindTas5707I2CAddr()
{
    using namespace Nilai::Tas5707;
    if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707Default))
    {
        TAS_INFO("Device is default TAS5707");
        m_i2cAddr = I2CAddresses::TAS5707Default;
    }
    else if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707ADefault))
    {
        TAS_INFO("Device is default TAS5707A");
        m_i2cAddr = I2CAddresses::TAS5707ADefault;
    }
    else if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707Alt))
    {
        TAS_INFO("Device is alternative TAS5707");
        m_i2cAddr = I2CAddresses::TAS5707Alt;
    }
    else if (m_hw.I2cHandle->CheckIfDevOnBus(I2CAddresses::TAS5707AAlt))
    {
        TAS_INFO("Device is alternative TAS5707A");
        m_i2cAddr = I2CAddresses::TAS5707AAlt;
    }

    // Returns true if we've found a TAS5707, false otherwise.
    return m_i2cAddr != 0;
}

void Tas5707Module::TrimOscillator()
{
    uint8_t v = 0;
    m_hw.I2cHandle->TransmitFrameToRegister(
      m_i2cAddr, (uint8_t)Nilai::Tas5707::Registers::OscTrim, {0});
    // This can take up to 50ms.
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() <= start + 50) && v != 0xC0)
    {
        v = m_hw.I2cHandle
              ->ReceiveFrameFromRegister(m_i2cAddr, (uint8_t)Nilai::Tas5707::Registers::OscTrim, 1)
              .data[0];
        TAS_DEBUG("Oscillator status: %#02X", v);
        HAL_Delay(1);
    }
    if (v != 0xC0)
    {
        TAS_ERROR("Oscillator is not ready after 50ms!");
        return;
    }

    TAS_INFO("Oscillator ready");
}
void Tas5707Module::SetBiquadFilters(const Nilai::Tas5707::BiquadBanks& ch1,
                                     const Nilai::Tas5707::BiquadBanks& ch2)
{
    using namespace Nilai::Tas5707;
    struct Action
    {
        uint8_t                            BankId = 0;
        const std::array<BiquadFilter, 7>& Ch1;
        const std::array<BiquadFilter, 7>& Ch2;

        void Foo() {}
    };
    std::array<Action, 3U> actions = {Action {0x01, ch1.Bank1, ch2.Bank1},     // Bank 1
                                      Action {0x02, ch1.Bank2, ch2.Bank2},     // Bank 2
                                      Action {0x03, ch1.Bank3, ch2.Bank3}};    // Bank 3

    // First, get the state the bank switch register to avoid changing anything.
    std::vector<uint8_t> bankSwitchReg =
      m_hw.I2cHandle->ReceiveFrameFromRegister(m_i2cAddr, (uint8_t)Registers::BankSwitchCtrl, 4)
        .data;
    uint8_t bankSwitchMode = bankSwitchReg[3];    // Save the current settings.

    for (const auto& action : actions)
    {
        // Set the bank to set the filters in.
        bankSwitchReg.back() = action.BankId;
        m_hw.I2cHandle->TransmitFrameToRegister(
          m_i2cAddr, (uint8_t)Registers::BankSwitchCtrl, bankSwitchReg);

        uint8_t fReg      = (uint8_t)Registers::Ch1Bq_0;
        auto    writeFunc = [&fReg, this](const auto& filters)
        {
            for (const auto& filter : filters)
            {
                auto sFilter = filter.Serialize();
                m_hw.I2cHandle->TransmitFrameToRegister(
                  m_i2cAddr, fReg, sFilter.data(), sFilter.size());
                ++fReg;
            }
        };
        writeFunc(action.Ch1);
        fReg = (uint8_t)Registers::Ch2Bq_0;
        writeFunc(action.Ch2);
    }

#    if defined(DEBUG)
    // Validate the write.
    for (const auto& action : actions)
    {
        // Set the bank to set the filters in.
        bankSwitchReg.back() = action.BankId;
        m_hw.I2cHandle->TransmitFrameToRegister(
          m_i2cAddr, (uint8_t)Registers::BankSwitchCtrl, bankSwitchReg);

        uint8_t fReg     = (uint8_t)Registers::Ch1Bq_0;
        auto    readFunc = [&fReg, this](const std::array<BiquadFilter, 7>& filters) -> bool
        {
            bool isGood = true;
            for (const auto& filter : filters)
            {
                auto rb =
                  m_hw.I2cHandle->ReceiveFrameFromRegister(m_i2cAddr, fReg, sizeof(BiquadFilter))
                    .data;
                if (!std::equal(rb.begin(), rb.end(), filter.Serialize().begin()))
                {
                    TAS_ERROR("Filter at address %#02x doesn't match what's expected!", fReg);
                    isGood = false;
                }
                ++fReg;
            }

            return isGood;
        };
        readFunc(action.Ch1);
        fReg = (uint8_t)Registers::Ch2Bq_0;
        readFunc(action.Ch2);
    }
#    endif

    // Write the bank switch mode back.
    bankSwitchReg.back() = bankSwitchMode;
    m_hw.I2cHandle->TransmitFrameToRegister(
      m_i2cAddr, (uint8_t)Registers::BankSwitchCtrl, bankSwitchReg);

    TAS_INFO("Biquad filters set.");
}

void Tas5707Module::SetDynamicRangeCtrl(const Nilai::Tas5707::DynamicRangeControl& drc)
{
    using namespace Nilai::Tas5707;
    auto writeFunc = [this](const auto& data, Registers reg) -> bool
    {
        m_hw.I2cHandle->TransmitFrameToRegister(m_i2cAddr, (uint8_t)reg, data.data(), data.size());
#    if defined(DEBUG)
        // Verify that the data was successfully written.
        auto rb =
          m_hw.I2cHandle->ReceiveFrameFromRegister(m_i2cAddr, (uint8_t)reg, data.size()).data;
        if (!std::equal(rb.begin(), rb.end(), data.begin()))
        {
            TAS_ERROR("Invalid DRC data read back at address %#02x!", reg);
            return false;
        }
#    endif
        return true;
    };

    writeFunc(drc.Aa.Serialize(), Registers::DrcAa);
    writeFunc(drc.Ae.Serialize(), Registers::DrcAe);
    writeFunc(drc.Ad.Serialize(), Registers::DrcAd);
    writeFunc(Nilai::ValToVector<true>(drc.T), Registers::DrcT);
    writeFunc(Nilai::ValToVector<true>(drc.K), Registers::DrcK);
    writeFunc(Nilai::ValToVector<true>(drc.O), Registers::DrcO);
    writeFunc(Nilai::ValToVector<true>(drc.State), Registers::DrcCtrl);

    TAS_INFO("Dynamic range control initialized.");
}

void Tas5707Module::SetBankSwitchingMode(uint32_t mode)
{
    CEP_ASSERT(Nilai::Tas5707::BankSwitchingModes::IsValid(mode), "Invalid bank switching mode!");

    if (!UpdateRegisterValue(m_hw.I2cHandle,
                             m_i2cAddr,
                             (uint8_t)Nilai::Tas5707::Registers::BankSwitchCtrl,
                             mode,
                             (uint32_t)0x000000FFU))
    {
        TAS_ERROR("Unable to update bank switching mode!");
    }
    else
    {
        TAS_INFO("Bank switching mode set");
    }
}

void Tas5707Module::SetSerialDataInterface(uint8_t in)
{
    CEP_ASSERT(Nilai::Tas5707::SerialDataMode::IsValid(in), "Invalid serial data interface!");

    if (!UpdateRegisterValue(
          m_hw.I2cHandle, m_i2cAddr, (uint8_t)Nilai::Tas5707::Registers::SerDataInter, in))
    {
        TAS_ERROR("Unable to update serial data interface!");
    }
    else
    {
        TAS_INFO("Serial data interface set");
    }
}
void Tas5707Module::SetSysCtrl1Reg(uint8_t dcBlock, uint8_t muteRecover, uint8_t deEmphasis)
{
    using namespace Nilai::Tas5707;
    CEP_ASSERT(PwmDcBlock::IsValid(dcBlock), "PWM DC block mode is not valid!");
    CEP_ASSERT(ClkErrRecoveryModes::IsValid(muteRecover),
               "Mute mode on recovery from clock error is not valid");
    CEP_ASSERT(DeEmphasisModes::IsValid(deEmphasis), "De-Emphasis mode is not valid");

    if (!UpdateRegisterValue(m_hw.I2cHandle,
                             m_i2cAddr,
                             (uint8_t)Registers::SysCtrl1,
                             uint8_t(dcBlock | muteRecover | deEmphasis)))
    {
        TAS_ERROR("Unable to update system control register 1!");
    }
    else
    {
        TAS_INFO("System control register 1 set");
    }
}

void Tas5707Module::ToggleSoftMute(bool ch1, bool ch2)
{
    if (!UpdateRegisterValue(m_hw.I2cHandle,
                             m_i2cAddr,
                             (uint8_t)Nilai::Tas5707::Registers::SoftMute,
                             (uint8_t)((uint8_t)ch1 | ((uint8_t)ch2 << 1))))
    {
        TAS_ERROR("Unable to update soft mute register!");
    }
    else
    {
        TAS_INFO("Soft mute register set");
    }
}

void Tas5707Module::SetMasterVolume(uint8_t vol)
{
    uint8_t rVal = VolumeToReg(vol);

    if (!UpdateRegisterValue(
          m_hw.I2cHandle, m_i2cAddr, (uint8_t)Nilai::Tas5707::Registers::MasterVol, rVal))
    {
        TAS_ERROR("Unable to update master volume register!");
    }
    else
    {
        TAS_INFO("Set master volume to %d", vol);
    }
}

void Tas5707Module::SetChannelVolume(Nilai::Tas5707::Channels ch, uint8_t vol)
{
    uint8_t rVal    = VolumeToReg(vol);
    uint8_t channel = (uint8_t)Nilai::Tas5707::Registers::MasterVol + (uint8_t)ch;

    if (!UpdateRegisterValue(m_hw.I2cHandle, m_i2cAddr, channel, rVal))
    {
        TAS_ERROR("Unable to update channel %d volume register!", (uint8_t)ch);
    }
    else
    {
        TAS_INFO("Set channel %d volume to %d", (uint8_t)ch, vol);
    }
}

void Tas5707Module::SendConfiguration()
{
    // Set the biquad filters.
    SetBiquadFilters(m_sw.Ch1BiquadFilters, m_sw.Ch2BiquadFilters);

    SetDynamicRangeCtrl(m_sw.DRC);

    SetBankSwitchingMode(m_sw.BankSwitchMode);

    SetSysCtrl1Reg(m_sw.BlockDc, m_sw.ClkRecoverMode, m_sw.DeEmphasisMode);
    SetSerialDataInterface(m_sw.SerialMode);
    ToggleSoftMute(m_sw.Ch1Mute != 0, m_sw.Ch2Mute != 0);
    SetMasterVolume(0x9E);                                         // 0dB
    SetChannelVolume(Nilai::Tas5707::Channels::Channel1, 0x9E);    // 0dB
    SetChannelVolume(Nilai::Tas5707::Channels::Channel2, 0x9E);    // 0dB

    SetChannelInputAndModes(m_sw.Ch1ModMode, m_sw.Ch1Source, m_sw.Ch2ModMode, m_sw.Ch2Source);
    SetChannelOutputs(m_sw.OutASource, m_sw.OutBSource, m_sw.OutCSource, m_sw.OutDSource);
}

bool Tas5707Module::Enable()
{
    // Writing 0x00 in SysCtrl2 (0x05) requests an exit of shutdown mode.
    if (!UpdateRegisterValue(
          m_hw.I2cHandle, m_i2cAddr, (uint8_t)Nilai::Tas5707::Registers::SysCtrl2, (uint8_t)0x00))
    {
        TAS_ERROR("Unable to exit all-channel shutdown mode!");
        return false;
    }
    else
    {
        TAS_INFO("Leaving all-channel shutdowm...");
        // tStart = 1 + 1.3 * startTime.
        uint32_t tStart = (uint32_t)((float)StartStopRegValToTime(m_sw.StartStopDelay) * 1.3f + 1);
        HAL_Delay(tStart);
        TAS_INFO("Now running!");
        return true;
    }
}

bool Tas5707Module::Disable()
{
    // Writing 0x40 in SysCtrl2 (0x05) requests an entrance into shutdown mode.
    if (!UpdateRegisterValue(
          m_hw.I2cHandle, m_i2cAddr, (uint8_t)Nilai::Tas5707::Registers::SysCtrl2, (uint8_t)0x40))
    {
        TAS_ERROR("Unable to enter all-channel shutdown mode!");
        return false;
    }
    else
    {
        TAS_INFO("Entering all-channel shutdowm...");
        // tStart = 1 + 1.3 * startTime.
        uint32_t tStart = (uint32_t)((float)StartStopRegValToTime(m_sw.StartStopDelay) * 1.3f + 1);
        HAL_Delay(tStart);
        TAS_INFO("Now stopped!");
        return true;
    }
}

void Tas5707Module::SetChannelInputAndModes(uint32_t ch1Mode,
                                            uint32_t ch1Source,
                                            uint32_t ch2Mode,
                                            uint32_t ch2Source)
{
    using namespace Nilai::Tas5707;
    CEP_ASSERT(Channel1ModulationModes::IsValid(ch1Mode), "Invalid modulation mode for channel 1");
    CEP_ASSERT(Channel2ModulationModes::IsValid(ch2Mode), "Invalid modulation mode for channel 2");
    CEP_ASSERT(Channel1InputSources::IsValid(ch1Source), "Invalid input source for channel 1");
    CEP_ASSERT(Channel2InputSources::IsValid(ch2Source), "Invalid input source for channel 2");

    uint32_t regVal = ch1Mode | ch1Source | ch2Mode | ch2Source;

    if (!UpdateRegisterValue(
          m_hw.I2cHandle, m_i2cAddr, (uint8_t)Registers::InMux, regVal, (uint32_t)0x00FF0000))
    {
        TAS_ERROR("Unable to update Input Multiplexer Register!");
    }
    else
    {
        TAS_INFO("Updated input mux register");
    }
}

void Tas5707Module::SetChannelOutputs(uint32_t ASrc, uint32_t BSrc, uint32_t CSrc, uint32_t DSrc)
{
    using namespace Nilai::Tas5707;
    CEP_ASSERT(OutASources::IsValid(ASrc), "Invalid output source for OUT_A");
    CEP_ASSERT(OutBSources::IsValid(BSrc), "Invalid output source for OUT_B");
    CEP_ASSERT(OutCSources::IsValid(CSrc), "Invalid output source for OUT_C");
    CEP_ASSERT(OutDSources::IsValid(DSrc), "Invalid output source for OUT_D");

    // Reserved values for this register: 0x01XXXX45
    uint32_t regVal = 0x01000000 | ASrc | BSrc | CSrc | DSrc | 0x00000045;

    if (!UpdateRegisterValue(m_hw.I2cHandle, m_i2cAddr, (uint8_t)Registers::PwmMux, regVal))
    {
        TAS_ERROR("Unable to update PWM Output Register!");
    }
    else
    {
        TAS_INFO("Updated PWM output mux register");
    }
}


//--------------------------------------------------------------------------------------------------
// Static functions
/**
 * @brief Translates a value ranging from 0 (muted) to 206 (max volume) to the value to be used in
 * the register.
 * @param vol The desired volume, from 0x00 to 0xCE
 * @return The value to set the register to.
 * @note If @c vol is over 0xCE, it will be clamped to 0xCE.
 */
uint8_t VolumeToReg(uint8_t vol)
{
    static constexpr uint8_t maxVol = 0xCE;
    static constexpr uint8_t muted  = 0xFF;
    // Clamp volume to 0xCE.
    vol = std::min((uint8_t)maxVol, vol);

    // MUTE (0) has a value of 0xFF.
    if (vol == 0)
    {
        return muted;
    }
    else
    {
        return maxVol - vol;
    }
}

uint32_t StartStopRegValToTime(uint8_t ss)
{
    using namespace Nilai::Tas5707;
    // These values are taken from the datasheet.
    switch (ss)
    {
        case StartStopPeriods::None: return 0;
        case StartStopPeriods::Ms16_5: return 17;
        case StartStopPeriods::Ms23_9: return 24;
        case StartStopPeriods::Ms31_4: return 32;
        case StartStopPeriods::Ms40_4: return 41;
        case StartStopPeriods::Ms53_9: return 54;
        case StartStopPeriods::Ms70_3: return 71;
        case StartStopPeriods::Ms94_2: return 95;
        case StartStopPeriods::Ms125_7: return 126;
        case StartStopPeriods::Ms164_6: return 165;
        case StartStopPeriods::Ms239_4: return 240;
        case StartStopPeriods::Ms314_2: return 315;
        case StartStopPeriods::Ms403_9: return 404;
        case StartStopPeriods::Ms538_6: return 539;
        case StartStopPeriods::Ms703_1: return 704;
        case StartStopPeriods::Ms942_5: return 943;
        case StartStopPeriods::Ms1256_6: return 1257;
        case StartStopPeriods::Ms1728_1: return 1729;
        case StartStopPeriods::Ms2513_6: return 2514;
        case StartStopPeriods::Ms3299_1: return 3300;
        case StartStopPeriods::Ms4241_7: return 4242;
        case StartStopPeriods::Ms5655_6: return 5656;
        case StartStopPeriods::Ms7383_7: return 7384;
        case StartStopPeriods::Ms9897_3: return 9898;
        case StartStopPeriods::Ms13196_4: return 13197;
        default: return 0;
    }
}

#endif
