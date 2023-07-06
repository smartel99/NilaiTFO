/**
 *******************************************************************************
 * @file	ltc2498_module.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#ifndef LTC2498MODULE_H_
#define LTC2498MODULE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_LTC2498)
#    if !defined(NILAI_USE_SPI)
#        error The SPI module must be enabled in order to use the LTC2498 Module
#    endif
#    include "defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    include "defines/module.hpp"
#    include "defines/pin.h"
#    include "drivers/spiModule.hpp"

#    include <array>
#    include <functional>
#    include <string>
#    include <vector>


/***********************************************/
/* Defines */
namespace LTC2498
{
/**
 * @brief   Lists all the possible channels
 */
enum class Channels
{
    CH0  = 0x00,    //!< CH0
    CH1  = 0x08,    //!< CH1
    CH2  = 0x01,    //!< CH2
    CH3  = 0x09,    //!< CH3
    CH4  = 0x02,    //!< CH4
    CH5  = 0x0A,    //!< CH5
    CH6  = 0x03,    //!< CH6
    CH7  = 0x0B,    //!< CH7
    CH8  = 0x04,    //!< CH8
    CH9  = 0x0C,    //!< CH9
    CH10 = 0x05,    //!< CH10
    CH11 = 0x0D,    //!< CH11
    CH12 = 0x06,    //!< CH12
    CH13 = 0x0E,    //!< CH13
    CH14 = 0x07,    //!< CH14
    CH15 = 0x0F,    //!< CH15
};

/**
 * @brief   Lists the possible types of acquisition.
 */
enum class AcquisitionTypes
{
    Differential = 0,    //!< Combines the selected channel with the next one.
    SingleEnded,         //!< Reads the selected channel referenced to the common reference.
};

/**
 * @brief   The polarities for an acquisition.
 */
enum class Polarities
{
    Positive = 0,    //!< The selected channel is the positive input.
    Negative,        //!< The selected channel is the negative input.
};

/**
 * @brief   Lists the two types of input that can be measured.
 */
enum class InputTypes
{
    External = 0,    //!< External channel, selected via @ref Channels.
    TempSensor,      //!< Internal temperature sensor built into the ADC.
};

/**
 * @brief   The filters that can be applied to the input signal.
 */
enum class Filters
{
    None = 0,    //!< No filter active.
    Hz50,        //!< 50Hz filter active.
    Hz60,        //!< 60Hz filter active.
    All,         //!< Both 50Hz and 60Hz filters active.
};

/**
 * @brief   The different acquisition speeds.
 */
enum class Speeds
{
    x1 = 0,    //!< ~7.5 samples/second.
    x2,        //!< ~15 samples/second. This disables the auto-calibration.
};

/**
 * @brief   Contains all the settings used to start a conversion.
 */
struct ConversionSettings
{
    Channels                                              channel  = Channels::CH0;
    AcquisitionTypes                                      type     = AcquisitionTypes::Differential;
    Polarities                                            polarity = Polarities::Positive;
    InputTypes                                            inputType = InputTypes::External;
    Filters                                               filters   = Filters::All;
    Speeds                                                speed     = Speeds::x1;
    std::function<void(float, const ConversionSettings&)> callback  = {};

    std::array<uint8_t, 4> ToRegValues() const;

    ConversionSettings() = default;
    ConversionSettings(Channels                                                     ch,
                       AcquisitionTypes                                             ty,
                       Polarities                                                   pol,
                       InputTypes                                                   in,
                       Filters                                                      fil,
                       Speeds                                                       s,
                       const std::function<void(float, const ConversionSettings&)>& cb)
    : channel(ch), type(ty), polarity(pol), inputType(in), filters(fil), speed(s), callback(cb)
    {
    }
};

/**
 * @brief   Contains the value read on a channel.
 */
struct Reading
{
    ConversionSettings config;            //!< The settings used for this conversion.
    float              reading = 0.0f;    //!< The value read by the conversion, in volts.
    int32_t            raw     = 0;       //!< The raw value read by the conversion, in LSBs.
};

using CurrentConversion = std::vector<ConversionSettings>::iterator;
}    // namespace LTC2498


class Ltc2498Module : public Nilai::Module
{
public:
    Ltc2498Module(const std::string& label,
                  SpiModule*         spi,
                  const Nilai::Pin&  inPin,
                  const Nilai::Pin&  csPin,
                  float              vcom = 0.00f);
    virtual ~Ltc2498Module() override = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

    bool QueueConversions(const std::vector<LTC2498::ConversionSettings>& conversions,
                          bool                                            repeat = false);

    const LTC2498::Reading& GetLastReading() const { return m_lastReading; }

    bool IsConversionInProgress() const;

    bool StartConversion(const LTC2498::ConversionSettings& config);

private:
    std::string m_label   = "";
    SpiModule*  m_spi     = nullptr;
    Nilai::Pin  m_misoPin = {};
    Nilai::Pin  m_csPin   = {};
    float       m_vcom    = 0.00f;

    std::vector<LTC2498::ConversionSettings> m_conversions       = {};
    LTC2498::CurrentConversion               m_currentConversion = m_conversions.end();
    bool                                     m_isIteratorValid   = true;
    bool                                     m_isConverting      = false;
    bool                                     m_repeat            = false;

    LTC2498::Reading m_lastReading = {};

private:
    void                       SetMisoAsGpio();
    void                       SetMisoAsMiso();
    LTC2498::CurrentConversion GetNextConversion();
    std::array<uint8_t, 4>     SetNextConvAndReadResults(const std::array<uint8_t, 4>& config);
    void                       ParseConversionResult(const std::array<uint8_t, 4>&      resp,
                                                     const LTC2498::ConversionSettings& config);
};

/***********************************************/
/* Function declarations */
/* END OF FILE */
#endif
#endif
