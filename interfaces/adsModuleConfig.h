/**
 ******************************************************************************
 * @addtogroup adsModuleConfig
 * @{
 * @file    adsModuleConfig
 * @author  Client Microdata
 * @brief   Header for the adsModuleConfig module.
 *
 * @date 9/2/2020 9:50:53 AM
 *
 ******************************************************************************
 */
#ifndef _adsModuleConfig
#define _adsModuleConfig

/*****************************************************************************/
/* Includes */
#include "Core/Inc/gpio.h"

#include <cstdint>


/*****************************************************************************/
/* Exported defines */


/*****************************************************************************/
/* Exported macro */


/*****************************************************************************/
/* Exported types */

namespace ADS
{
#pragma region ADS Configuration Options

/**
 ** @enum SampleRate
 ** @brief  fData rates at fMOD = 4.96MHz
 */
enum class SampleRate
{
    Hz1000   = 0,
    Hz2000,
    Hz4000,
    Hz5120,
    Hz5333,
    Hz8000,
    Hz10240,
    Hz10667,
    Hz16000,
    Hz20480,
    Hz21333,
    Hz32000,
    Hz42667,
    Hz64000,
    Hz85333,
    Hz128000
};

/**
 ** @enum   Hamming
 ** @brief  Select if hamming is on or off
 */
enum class Hamming
{
    On  = 0b01,
    Off = 0b00
};

/**
 ** @enum   WordSize
 ** @brief  SPI word size in bits
 ** */
enum class WordSize
{
    Size16 = 0b10,
    Size24 = 0b00,
    Size32 = 0b01
};

/**
 ** @enum   SpiMode
 ** @brief  ADS SPI mode (synchronous/asynchronous slave/master)
 ** */
enum class SpiMode
{
    SynchronousMaster = 0b00,
    SynchronousSlave  = 0b10,
    AsynchronousSlave = 0b01
};

/**
 ** @enum   Threshold
 ** @brief  Upper voltage threshold for fault detection */
enum class Threshold
{
    P95 = 0b000,
    /*!< 000 : 95%      VDD     | 5%    VSS / -1.5V */
    P92 = 0b001,
    /*!< 001 : 92.5%    VDD     | 7.5%  VSS / -1.5V */
    P90 = 0b010,
    /*!< 010 : 90%      VDD     | 10%   VSS / -1.5V */
    P87 = 0b011,
    /*!< 011 : 87.5%    VDD     | 12.5% VSS / -1.5V */
    P85 = 0b100,
    /*!< 100 : 85%      VDD     | 15%   VSS / -1.5V */
    P80 = 0b101,
    /*!< 101 : 80%      VDD     | 20%   VSS / -1.5V */
    P75 = 0b110,
    /*!< 110 : 75%      VDD     | 25%   VSS / -1.5V */
    P70 = 0b111,
    /*!< 111 : 70%      VDD     | 30%   VSS / -1.5V */
};

/**
 ** @enum   Delay
 ** @brief  Delay values (for DONE pin & Hi-Z assertion on DOUT */
enum class Delay
{
    Ns6 = 0b00,
    /*!< 00 : >= 6ns delay */
    Ns8 = 0b01,
    /*!< 01 : >= 8ns delay */
    Ns10 = 0b10,
    /*!< 10 : >= 10ns delay */
    Ns12 = 0b11,
    /*!< 11 : >= 12ns delay */
};

/**
 ** @enum   ClockSource
 ** @brief  ADC clock source
 ** */
enum class ClockSource
{
    CLKIN = 0b0,
    SCLK  = 0b1
};

/**
 ** @enum   Resolution
 ** @brief  High/Low resolution mode
 ** */
enum class Resolution
{
    LowRes  = 0b0,
    HighRes = 0b1
};

/**
 ** @enum   ClockDivision
 ** @brief  Divider for FCLKIN to obtain FICLK
 ** */
enum class ClockDivision
{
    Div2 = 0b001,
    /*!< 001 : FICLK = FCLKIN / 2  */
    Div4 = 0b010,
    /*!< 010 : FICLK = FCLKIN / 4  */
    Div6 = 0b011,
    /*!< 011 : FICLK = FCLKIN / 6  */
    Div8 = 0b100,
    /*!< 100 : FICLK = FCLKIN / 8  */
    Div10 = 0b101,
    /*!< 101 : FICLK = FCLKIN / 10 */
    Div12 = 0b110,
    /*!< 110 : FICLK = FCLKIN / 12 */
    Div14 = 0b111,
    /*!< 111 : FICLK = FCLKIN / 14 */
};

/**
 ** @enum   Oversampling
 ** @brief  Number of samples to take per data
 ** */
enum class Oversampling
{
    Ovr32 = 0b00001111,
    /*!< 1111 : FDATA = FMOD / 32   */
    Ovr48 = 0b00001110,
    /*!< 1110 : FDATA = FMOD / 48   */
    Ovr64 = 0b00001101,
    /*!< 1101 : FDATA = FMOD / 64   */
    Ovr96 = 0b00001100,
    /*!< 1100 : FDATA = FMOD / 96   */
    Ovr128 = 0b00001011,
    /*!< 1011 : FDATA = FMOD / 128  */
    Ovr192 = 0b00001010,
    /*!< 1010 : FDATA = FMOD / 192  */
    Ovr200 = 0b00001001,
    /*!< 1001 : FDATA = FMOD / 200  */
    Ovr256 = 0b00001000,
    /*!< 1000 : FDATA = FMOD / 256  */
    Ovr384 = 0b00000111,
    /*!< 0111 : FDATA = FMOD / 384  */
    Ovr400 = 0b00000110,
    /*!< 0110 : FDATA = FMOD / 400  */
    Ovr512 = 0b00000101,
    /*!< 0101 : FDATA = FMOD / 512  */
    Ovr768 = 0b00000100,
    /*!< 0100 : FDATA = FMOD / 768  */
    Ovr800 = 0b00000011,
    /*!< 0011 : FDATA = FMOD / 800  */
    Ovr1024 = 0b00000010,
    /*!< 0010 : FDATA = FMOD / 1024 */
    Ovr2048 = 0b00000001,
    /*!< 0001 : FDATA = FMOD / 2048 */
    Ovr4096 = 0b00000000,
    /*!< 0000 : FDATA = FMOD / 4096 */
};

/**
 ** @enum   Enable
 ** @brief  Enable of disable a specific channel
 ** */
enum class Enable
{
    Enable  = 0b1111,
    Disable = 0b0000,
};

/**
 ** @enum   DigitalGain
 ** @brief  Digital gain of the ADC channel output.
 **         (can be specified for every channel)
 ** */
enum class DigitalGain
{
    Gain1  = 0b000,
    Gain2  = 0b001,
    Gain4  = 0b010,
    Gain8  = 0b011,
    Gain16 = 0b100
};

/**
 ** @enum   InternalReferenceEnable
 ** @brief  Enable/disable internal reference
 ** */
enum class InternalReferenceEnable
{
    Enable  = 0b1,
    Disable = 0b0
};

/**
 ** @enum   NegativeChargePumpEnable
 ** @brief  Enable/disable Negative charge pump
 ** */
enum class NegativeChargePumpEnable
{
    Enable  = 0b1,
    Disable = 0b0
};

/**
 ** @enum   ReferenceVoltage
 ** @brief  Set ADS reference voltage level (2.442V or 4.0V)
 ** */
enum class ReferenceVoltage
{
    Ref2V442 = 0b0,
    Ref4V    = 0b1
};

/**
 ** @enum   WatchdogEnable 
 ** @brief  Enable/disable watchdog timer
 ** */
enum class WatchdogEnable
{
    Enable  = 0b1,
    Disable = 0b0
};

/**
 ** @enum   CrcMode
 ** @brief  Select if CRC is valid on device words only or on all bits
 ** */
enum class CrcMode
{
    All        = 0b1,
    DeviceWord = 0b0
};

/**
 ** @enum   FrameMode
 ** @brief  Enable/disable dynamic data frame size
 ** */
enum class FrameMode
{
    Fixed   = 0b1,
    Dynamic = 0b0
};

/**
 ** @enum   CrcEnable
 ** @brief  Enable/disable CRC
 ** */
enum class CrcEnable
{
    Enable  = 0b1,
    Disable = 0b0
};

struct Pin
{
    GPIO_TypeDef* port = nullptr;
    uint16_t pin = 0;
    bool operator==(const Pin& other) const
    {
        return ((this->port == other.port)&&(this->pin == other.pin));
    }
};

struct Pins
{
    Pin chipSelect;
    Pin reset;
    Pin dataReady;
    Pin done;
    bool operator==(const Pins& other) const
    {
        return ((chipSelect == other.chipSelect)
                &&(this->reset == other.reset)
                &&(this->dataReady == other.dataReady)
                &&(this->done == other.done));
    }
};

class Config
{
public:
    Pins pins;
    SampleRate sampleRate = SampleRate::Hz16000;
    Hamming hamming = Hamming::Off;
    WordSize wordSize = WordSize::Size24; /*!< Can only be changed by hardware */
    SpiMode spiMode = SpiMode::AsynchronousSlave; /*!< Can only be changed by hardware */
    Threshold threshold = Threshold::P95;
    Delay highZDelay = Delay::Ns12;
    Delay doneDelay = Delay::Ns12;
    ClockSource clockSource = ClockSource::CLKIN;
    Resolution resolution = Resolution::HighRes;
    ClockDivision clockInDivision = ClockDivision::Div2;
    ClockDivision modulatorClockDivision = ClockDivision::Div2;
    Oversampling oversampling = (Oversampling)sampleRate;
    Enable enable = Enable::Enable;
    DigitalGain ch1DigitalGain = DigitalGain::Gain1;
    DigitalGain ch2DigitalGain = DigitalGain::Gain1;
    DigitalGain ch3DigitalGain = DigitalGain::Gain1;
    DigitalGain ch4DigitalGain = DigitalGain::Gain1;
    InternalReferenceEnable internalReferenceEnable = InternalReferenceEnable::Enable;
    NegativeChargePumpEnable negativeChargePumpEnable = NegativeChargePumpEnable::Disable;
    ReferenceVoltage referenceVoltage = ReferenceVoltage::Ref2V442;
    WatchdogEnable watchdogEnable = WatchdogEnable::Disable;
    CrcMode crcMode = CrcMode::All;
    FrameMode frameMode = FrameMode::Dynamic;
    CrcEnable crcEnable = CrcEnable::Disable;
    
    /**
     ** @brief  Allows to do:
     **         Config myConfigA;
     **         Config myConfigB;
     **         if(myConfigA != myConfigB)
     **         {
     **             DoStuff();
     **         }
     ** */
    bool operator==(const Config& other) const
    {
        return ((this->pins == other.pins)
                &&(this->sampleRate == other.sampleRate)
                &&(this->hamming == other.hamming)
                &&(this->wordSize == other.wordSize)
                &&(this->spiMode == other.spiMode)
                &&(this->threshold == other.threshold)
                &&(this->highZDelay == other.highZDelay)
                &&(this->doneDelay == other.doneDelay)
                &&(this->clockSource == other.clockSource)
                &&(this->resolution == other.resolution)
                &&(this->clockInDivision == other.clockInDivision)
                &&(this->modulatorClockDivision == other.modulatorClockDivision)
                &&(this->oversampling == other.oversampling)
                &&(this->enable == other.enable)
                &&(this->ch1DigitalGain == other.ch1DigitalGain)
                &&(this->ch2DigitalGain == other.ch2DigitalGain)
                &&(this->ch3DigitalGain == other.ch3DigitalGain)
                &&(this->ch4DigitalGain == other.ch4DigitalGain)
                &&(this->internalReferenceEnable == other.internalReferenceEnable)
                &&(this->negativeChargePumpEnable == other.negativeChargePumpEnable)
                &&(this->referenceVoltage == other.referenceVoltage)
                &&(this->watchdogEnable == other.watchdogEnable)
                &&(this->crcMode == other.crcMode)
                &&(this->frameMode == other.frameMode)
                &&(this->crcEnable == other.crcEnable));
    }
};
#pragma endregion

namespace Registers
{
#pragma region Register mapped structures

/**
 ** @class  Id
 ** @brief  ID code register
 ** @note   Read-only
 ** */
class Id
{
public:
    Id() = default;
    
    static const uint8_t Address = 0x00;
    
    /**
     ** @brief  Contains the device channel count:
     **         0x02: 2-channels
     **         0x04: 4-channels
     ** */
    uint8_t nbChannels = 0;
};

class Revision
{
public:
    Revision() = default;
    
    static const uint8_t Address = 0x01;
    
    uint8_t revId = 0;
};

class ModePin
{
public:
    ModePin() = default;
    
    static const uint8_t Address = 0x07;
    
    union
    {
        struct
        {
            /** @brief  Status of hardware M0 pin:
              *        - 00 : GND - Synchronous master mode 
              *        - 01 : VDD - Asynchronous slave mode 
              *        - 10 : NC  - Synchronous slave mode  
              *        - 11 : Reserved
              * 
              *          Can take the values of @ref ADS_SPI_MODE
              */
            uint8_t m0Pin : 2;
        
            /** @brief  Status of hardware M1 pin:    
             *        - 00 : GND - 24-bits device word
             *        - 01 : VDD - 32-bits device word
             *        - 10 : NC  - 16-bits device word
             *        - 11 : Reserved
             * 
             *          Can take the values of @ref ADS_WORDSIZE
             */
            uint8_t m1Pin : 2;

            /** @brief  Status of hardware M2 pin:
             *        - 00 : GND - Hamming off    
             *        - 01 : VDD - Hamming on     
             *        - 10 : No Connect - reserved
             *        - 11 : Reserved
             * 
             *          Can take the values of @ref ADS_HAMMING
             */
            uint8_t m2Pin : 2;
                
            uint8_t: 2;
        };
        uint8_t value = 0;
    };
};

class GenericErrors
{
public:
    GenericErrors() = default;
    
    static const uint8_t Address = 0x02;
    
    union
    {
        struct
        {
            uint8_t crcInputError : 1;
            uint8_t dataDropped : 1;
            uint8_t spiSyncError : 1;
            uint8_t watchdogError : 1;
            /** View Positive/Negative Threshold Error Register if set */
            uint8_t adcInputError : 1;  
            /** View SPI Error Register if set */
            uint8_t spiError : 1;
            uint8_t invalidCmd : 1;
        };
        uint8_t value = 0;
    };
};

class PosThresholdError
{
public:
    PosThresholdError() = default;
    
    static const uint8_t Address = 0x03;
    
    union
    {
        struct
        {
            uint8_t channel1OverThreshold : 1;
            uint8_t channel2OverThreshold : 1;
            uint8_t channel3OverThreshold : 1;
            uint8_t channel4OverThreshold : 1;
            uint8_t:4;
        };
        uint8_t value = 0;
    };
};

class NegThresholdError
{
public:
    NegThresholdError() = default;
    
    static const uint8_t Address = 0x04;
    
    union
    {
        struct
        {
            uint8_t channel1UnderThreshold : 1;
            uint8_t channel2UnderThreshold : 1;
            uint8_t channel3UnderThreshold : 1;
            uint8_t channel4UnderThreshold : 1;
            uint8_t:4;
        };
        uint8_t value = 0;
    };
};

class SpiError
{
public:
    SpiError() = default;
    
    static const uint8_t Address = 0x05;
    
    union
    {
        struct
        {
            uint8_t missingSclkCycles : 1;
            uint8_t invalidChipSelectTransition : 1;
            uint8_t powerUpError : 1;
            uint8_t:5;
        };
        uint8_t value = 0;
    };
};

class ErrorCount
{
public:
    ErrorCount() = default;
    
    static const uint8_t Address = 0x06;
    
    uint8_t errorCount = 0;
};

class AnalogSysConfig
{
public:
    AnalogSysConfig() = default;
    AnalogSysConfig(const Config& config)
    {
        comparatorThreshold = (uint8_t)config.threshold;
        internalRef = (uint8_t)config.internalReferenceEnable;
        referenceVoltage = (uint8_t)config.referenceVoltage;
        highResMode = (uint8_t)config.resolution;
        negativeChargePumpEnable = (uint8_t)config.negativeChargePumpEnable;
    }
    
    static const uint8_t Address = 0x0B;
    
    union
    {
        struct
        {
            /** @brief  Comparator threshold select:          
             *        - 000 : 95%   VDD   | 5%    VSS / -1.5V 
             *        - 001 : 92.5% VDD   | 7.5%  VSS / -1.5V 
             *        - 010 : 90%   VDD   | 10%   VSS / -1.5V 
             *        - 011 : 87.5% VDD   | 12.5% VSS / -1.5V 
             *        - 100 : 85%   VDD   | 15%   VSS / -1.5V 
             *        - 101 : 80%   VDD   | 20%   VSS / -1.5V 
             *        - 110 : 75%   VDD   | 25%   VSS / -1.5V 
             *        - 111 : 70%   VDD   | 30%   VSS / -1.5V
             * 
             *          Can take the values of @ref ADS_VOLTAGE_THRESHOLD
             */
            uint8_t comparatorThreshold : 3;
        
            /** @brief  Internal reference enable.
             * 
             *          Can take the values of @ref ADS_INTERNAL_REFERENCE_ENABLE
             */
            uint8_t internalRef : 1;
        
            /** @brief  Determines the reference voltage:
             *        - 0 : REFP set to 2.442V
             *        - 1 : REFP set to 4.0V
             * 
             *          Can take the values of @ref ADS_REFERENCE_VOLTAGE
             */
            uint8_t referenceVoltage : 1;
        
            uint8_t: 1;
        
            /** @brief  High-resolution mode.
             * 
             *          Can take the values of @ref ADS_RESOLUTION
             */
            uint8_t highResMode : 1;
        
            /** @brief  Negative charge pump enable:
             *        - 0 : VDD & VSS used for threshold
             *        - 1 : VDD and internal -1.5V used
             *  @note   When using 3.0V to 3.45V supply
             */
            uint8_t negativeChargePumpEnable : 1;
        };
        uint8_t value = 0;
    };
};

class DigitalSysConfig
{
public:
    DigitalSysConfig() = default;
    DigitalSysConfig(const Config& config)
    {
        crcEnable = (uint8_t)config.crcEnable;
        frameMode = (uint8_t)config.frameMode;
        highZDelay = (uint8_t)config.highZDelay;
        doneDelay = (uint8_t)config.doneDelay;
        crcMode = (uint8_t)config.crcMode;
        watchdogEnable = (uint8_t)config.watchdogEnable;
    }
    
    static const uint8_t Address = 0x0C;
    
    union
    {
        struct
        {
            
            uint8_t crcEnable : 1; /** @brief  CRC enable */

            /** @brief  Fixed word size enabled:
             *        - 0 : Lenght of frame depends on enable state of
             *              CRC and ADC
             *        - 1 : Fixed 6-words long data frame 
             *              (when using ADS131A04)
             * 
             *          Can take the values of @ref ADS_FRAME_MODE
             */
            uint8_t frameMode : 1; 

            /** @brief  High impedance delay. Configure time
             *          before Hi-Z on DOUT :
             *        - 00 : >= 6ns delay
             *        - 01 : >= 8ns delay
             *        - 10 : >= 10ns delay
             *        - 11 : >= 12ns delay
             * 
             *          Can take the values of @ref ADS_DELAY
             * 
             * @note    after LSB shifted out
             */
            uint8_t highZDelay : 2;

            /** @brief  DONE delay. Configure time before DONE :
             *        - 00 : >= 6ns delay
             *        - 01 : >= 8ns delay
             *        - 10 : >= 10ns delay
             *        - 11 : >= 12ns delay
             * 
             *          Can take the values of @ref ADS_DELAY
             * 
             * @note    after LSB shifted out
             */
            uint8_t doneDelay : 2; 
        
            /** @brief  Determines which bits the CRC is valid for:
             *        - 0 : Only device words sent & received
             *        - 1 : All bits sent & received
             * 
             *          Can take the values of @ref ADS_CRC_MODE
             */
            uint8_t crcMode : 1;
        
            /** @brief  Watchdog timer enable.
             * 
             *          Can take the values of @ref ADS_WATCHDOG_TIMER_ENABLE
             */
            uint8_t watchdogEnable : 1;
        };
        uint8_t value = 0;
    };
};

class ClockConfig1
{
public:
    ClockConfig1() = default;
    ClockConfig1(const Config& config)
    {
        clockDivider = (uint8_t)config.clockInDivision;
        clockSource = (uint8_t)config.clockSource;
    }
    
    static const uint8_t Address = 0x0D;
    
    union
    {
        struct
        {
            uint8_t:1;
            uint8_t clockDivider : 3;
            uint8_t:3;
            uint8_t clockSource : 1;
        };
        uint8_t value = 0;
    };
};

class ClockConfig2
{
public:
    ClockConfig2() = default;
    ClockConfig2(const Config& config)
    {
        oversampling = (uint8_t)config.oversampling;
        modClockDivider = (uint8_t)config.modulatorClockDivision;
    }
    
    static const uint8_t Address = 0x0E;
    
    union
    {
        struct
        {
            uint8_t oversampling : 4;
            uint8_t:1;
            uint8_t modClockDivider : 3;
        };
        uint8_t value = 0;
    };
};

class ChannelEnable
{
public:
    ChannelEnable() = default;
    ChannelEnable(const Config& config)
    {
        channelEnable = (uint8_t)config.enable;
    }
    
    static const uint8_t Address = 0x0F;
    
    union
    {
        struct
        {
            uint8_t channelEnable : 4;
            uint8_t:4;
        };
        uint8_t value = 0;
    };
};

class Ch1DigitalGain
{
public:
    Ch1DigitalGain() = default;
    Ch1DigitalGain(const Config& config)
    {
        gain = (uint8_t)config.ch1DigitalGain;
    }
    
    static const uint8_t Address = 0x11;
    
    union
    {
        struct
        {
            uint8_t gain : 3;
            uint8_t:5;
        };
        uint8_t value = 0;
    };
};

class Ch2DigitalGain
{
public:
    Ch2DigitalGain() = default;
    Ch2DigitalGain(const Config& config)
    {
        gain = (uint8_t)config.ch2DigitalGain;
    }
    
    static const uint8_t Address = 0x12;
    
    union
    {
        struct
        {
            uint8_t gain : 3;
            uint8_t:5;
        };
        uint8_t value = 0;
    };
};

class Ch3DigitalGain
{
public:
    Ch3DigitalGain() = default;
    Ch3DigitalGain(const Config& config)
    {
        gain = (uint8_t)config.ch3DigitalGain;
    }
    
    static const uint8_t Address = 0x13;
    
    union
    {
        struct
        {
            uint8_t gain : 3;
            uint8_t:5;
        };
        uint8_t value = 0;
    };
};

class Ch4DigitalGain
{
public:
    Ch4DigitalGain() = default;
    Ch4DigitalGain(const Config& config)
    {
        gain = (uint8_t)config.ch4DigitalGain;
    }
    
    static const uint8_t Address = 0x14;
    
    union
    {
        struct
        {
            uint8_t gain : 3;
            uint8_t:5;
        };
        uint8_t value = 0;
    };
};
#pragma endregion
}   // Registers


/**
 ** @enum   Acknowledges
 ** @brief  Acknowledge messages from the ADS
 ** */
namespace Acknowledges
{
    // Disable GCC's "unused variable" warning.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused"
constexpr uint16_t Ready  = 0xFF04;
constexpr uint16_t Lock   = 0x0555;
constexpr uint16_t Unlock = 0x0655;
constexpr uint16_t Wakeup = 0x0033;
// Restore GCC's diagnostic options.
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
}


/**
 ** @enum   SysCommands
 ** @brief  System commands to the ADS
 ** */
namespace SysCommands
{
// Disable GCC's "unused variable" warning.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused"
constexpr uint16_t Null = 0x0000;
/*!< Null command */
constexpr uint16_t Reset = 0x0011;
/*!< Software power-on reset */
constexpr uint16_t Standby = 0x0022;
/*!< Enter low-power standby mode */
constexpr uint16_t Wakeup = 0x0033;
/*!< Exit low-power standby mode */
constexpr uint16_t Lock = 0x0555;
/*!< Enter locked state (ignore all commands except NULL, RREGS & UNLOCK) */
constexpr uint16_t Unlock = 0x0655;
/*!< Exit locked state */
// Restore GCC's diagnostic options.
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
}


namespace Commands
{
// Disable GCC's "unused variable" warning.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused"
/**
 ** @brief   Read a single register
 ** */
constexpr uint16_t ReadSingleRegisterMask = 0x2000;
/**
 ** @brief  Read (`nnnn nnnn` + 1) registers starting at address `a aaaa`
 **         Command: 0b001a aaaa nnnn nnnn
 ** */
constexpr uint16_t ReadRegistersMask = 0x2000;
/**
 ** @brief  Write a single register at address `a aaaa` with data `dddd dddd`
 **         Command: 0b010a aaaa dddd dddd
 ** */
constexpr uint16_t WriteSingleRegisterMask = 0x4000;
/**
 ** @brief  Write (`nnnn nnnn` + 1) registers beginning at address `a aaaa`.
 **         Addition device words are required to send data (`dddd dddd`) to
 **         register address `a` and data (`eeee eeee`) to register address
 **         `a` + 1. 
 **         Each device word contains data for two registers.
 **         The data frame size is extended by (`n` / 2) device words to allow
 **         for command completion.
 **         Command: 0b011a aaaa nnnn nnnn
 **         Additional data: 0bdddd dddd eeee eeee
 ** */
constexpr uint16_t WriteRegistersMask = 0x6000;
// Restore GCC's diagnostic options.
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
}
}

/*****************************************************************************/
/* Exported functions */


/* Have a wonderful day :) */
#endif /* _adsModuleConfig */
/**
 * @}
 */
/****** END OF FILE ******/
