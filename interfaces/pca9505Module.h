/**
 ******************************************************************************
 * @addtogroup pca9505Module
 * @{
 * @file    pca9505Module
 * @author  Samuel Martel
 * @brief   Header for the pca9505Module module. (IO Extender)
 *
 * @date 9/11/2020 2:55:27 PM
 *
 ******************************************************************************
 */
#ifndef _pca9505Module
#define _pca9505Module

/*****************************************************************************/
/* Includes */
#include "defines/module.hpp"
#include "defines/pin.h"

#include "drivers/i2cModule.hpp"

/*****************************************************************************/
/* Exported defines */


/*****************************************************************************/
/* Exported macro */


/*****************************************************************************/
/* Exported types */
namespace PCA9505
{
enum class Pins
{
    p0 = 0x0000,
    p1 = 0x0001,
    p2 = 0x0002,
    p3 = 0x0003,
    p4 = 0x0004,
    p5 = 0x0005,
    p6 = 0x0006,
    p7 = 0x0007
};

enum class Ports
{
    p0 = 0,
    p1 = 1,
    p2 = 2,
    p3 = 3,
    p4 = 4,
};

enum class Polarity
{
    //! The input value is not inverted.
    Normal = 0,
    //! The input value is inverted.
    Inverted,
};

/**
 ** @enum   Direction
 ** @brief  The direction of the pin
 **/
enum class Direction
{
    //! The pin is an output.
    Output = 0,
    //! The pin is an input.
    Input = 1,
};

enum class Interrupt
{
    //! A level change at the pin will trigger an interrupt if the pin is an input.
    Enable = 0,
    //! No interrupts will be triggered.
    Disable = 1,
};

struct PinConfig
{
    Pins pin = Pins::p0;
    Ports port = Ports::p0;
    Polarity polarity = Polarity::Normal;
    Direction direction = Direction::Input;
    Interrupt interrupt = Interrupt::Disable;
    bool state = false;
};


struct Config
{
    I2cModule* i2c = nullptr;
    //! Set by hardware, between 0x40 and 0x4E.
    uint8_t address = 0x40;
    Pin outputEnable = { };
    Pin interrupt = { };
    Pin reset = { };
    std::vector<PinConfig> pinConfig = { };
};

union PortState
{
    struct
    {
        uint8_t pin0 : 1;
        uint8_t pin1 : 1;
        uint8_t pin2 : 1;
        uint8_t pin3 : 1;
        uint8_t pin4 : 1;
        uint8_t pin5 : 1;
        uint8_t pin6 : 1;
        uint8_t pin7 : 1;
    };
    uint8_t port = 0;
};
}

class Pca9505Module : public cep::Module
{
public:
    // Don't allow default construction.
    Pca9505Module() = delete;
    Pca9505Module(const PCA9505::Config& config, const std::string& label);
    virtual ~Pca9505Module() override = default;
    
    virtual void Run() override;
    virtual const std::string& GetLabel() const override
    {
        return m_label;
    }
    
    void EnableOutput();
    void DisableOutput();
    
    void Reset();
    void HoldReset();
    void ReleaseReset();
    
    void ConfigurePin(const PCA9505::PinConfig& config);
    void ConfigurePort(PCA9505::Ports, 
        uint8_t directions, 
        uint8_t polarities, 
        uint8_t interrupts, 
        uint8_t states = 0);
    
    // ReadPin doesn't actually reads the pin on the PCA but only returns the cached value.
    bool ReadPin(PCA9505::Ports port, PCA9505::Pins pin);
    PCA9505::PortState ReadPort(PCA9505::Ports port);
    
	// #TODO: Optimize WritePin to Bank Write requests and WritePort once each loop
    void WritePin(PCA9505::Ports port, PCA9505::Pins pin, bool state);
    void WritePort(PCA9505::Ports port, uint8_t state);
    
private:
    I2cModule* m_i2c;
    uint8_t m_address;
    std::string m_label;
    
    Pin m_outputEnable;
    Pin m_interrupt;
    Pin m_reset;
    std::array<PCA9505::PortState, 5> m_ports;
    std::array<uint8_t, 5> m_directions;
    std::array<uint8_t, 5> m_polarities;
    std::array<uint8_t, 5> m_interrupts;
};



/*****************************************************************************/
/* Exported functions */


/* Have a wonderful day :) */
#endif /* _pca9505Module */
/**
 * @}
 */
/****** END OF FILE ******/
