/**
 ******************************************************************************
 * @addtogroup pca9505Module
 * @{
 * @file    pca9505Module
 * @author  Samuel Martel
 * @brief   Source for the pca9505Module module. (IO Extender)
 *
 * @date 9/11/2020 2:55:27 PM
 *
 ******************************************************************************
 */
#include "pca9505Module.h"
#if defined(NILAI_USE_PCA9505)
Pca9505Module::Pca9505Module(const PCA9505::Config& config, const std::string& label)
: m_i2c(config.i2c),
  m_address(config.address),
  m_label(label),
  m_outputEnable(config.outputEnable),
  m_interrupt(config.interrupt),
  m_reset(config.reset)
{
    CEP_ASSERT(m_i2c != nullptr, "I2C module is NULL in PCA9505Module ctor!");

    HAL_GPIO_WritePin(m_reset.port, m_reset.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(m_outputEnable.port, m_outputEnable.pin, GPIO_PIN_SET);

    // For each pin in the configuration:
    for (const auto& pin : config.pinConfig)
    {
        // Update the pin's port information.
        uint8_t port = (uint8_t)pin.port;
        m_directions[port] |= ((uint8_t)pin.direction << (uint8_t)pin.pin);
        m_polarities[port] |= ((uint8_t)pin.polarity << (uint8_t)pin.pin);
        m_interrupts[port] |= ((uint8_t)pin.interrupt << (uint8_t)pin.pin);
        m_ports[port].port |= ((uint8_t)pin.state << (uint8_t)pin.pin);
    }

    // Copy the ports' states in a temporary vector.
    std::vector<uint8_t> states = std::vector<uint8_t>(5);
    for (const auto& p : m_ports)
    {
        states.emplace_back(p.port);
    }

    // Send the configuration to the chip.
    m_i2c->TransmitFrameToRegister(m_address, 0x88, states);
    m_i2c->TransmitFrameToRegister(m_address, 0x90, m_polarities.data(), m_polarities.size());
    m_i2c->TransmitFrameToRegister(m_address, 0x98, m_directions.data(), m_directions.size());
    m_i2c->TransmitFrameToRegister(m_address, 0xA0, m_interrupts.data(), m_interrupts.size());

    LOG_INFO("[%s]: Initialized", m_label.c_str());
}

bool Pca9505Module::DoPost()
{
#    warning No POSTs have been written for this module!
    return false;
}

void Pca9505Module::Run()
{
    //    static uint32_t lastRead = 0;
    //
    //    if (HAL_GetTick( ) > lastRead + 10)
    //    {
    //        lastRead = HAL_GetTick( );
    //        ReadPort(PCA9505::Ports::p0);
    //    }
}

void Pca9505Module::EnableOutput()
{
    // OE is active LOW.
    HAL_GPIO_WritePin(m_outputEnable.port, m_outputEnable.pin, GPIO_PIN_RESET);
}

void Pca9505Module::DisableOutput()
{
    // OE is active LOW.
    HAL_GPIO_WritePin(m_outputEnable.port, m_outputEnable.pin, GPIO_PIN_SET);
}

void Pca9505Module::Reset()
{
    // RESET is active LOW.
    HAL_GPIO_WritePin(m_reset.port, m_reset.pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(m_reset.port, m_reset.pin, GPIO_PIN_SET);
}

void Pca9505Module::HoldReset()
{
    // RESET is active LOW.
    HAL_GPIO_WritePin(m_reset.port, m_reset.pin, GPIO_PIN_RESET);
}

void Pca9505Module::ReleaseReset()
{
    // RESET is active LOW.
    HAL_GPIO_WritePin(m_reset.port, m_reset.pin, GPIO_PIN_SET);
}

void Pca9505Module::ConfigurePin(const PCA9505::PinConfig& config)
{
    uint8_t offset = (uint8_t)config.port;
    uint8_t pos    = (uint8_t)config.pin;

    uint8_t state = (uint8_t)config.state << pos;
    if (config.state == false)
    {
        m_ports[offset].port &= ~state;
    }
    else
    {
        m_ports[offset].port |= state;
    }

    uint8_t direction = (uint8_t)config.direction << pos;
    if (config.direction == PCA9505::Direction::Output)
    {
        m_directions[offset] &= ~direction;
    }
    else
    {
        m_directions[offset] |= direction;
    }

    uint8_t polarity = (uint8_t)config.polarity << pos;
    if (config.polarity == PCA9505::Polarity::Normal)
    {
        m_polarities[offset] &= ~polarity;
    }
    else
    {
        m_polarities[offset] |= polarity;
    }

    uint8_t interrupt = (uint8_t)config.interrupt << pos;
    if (config.interrupt == PCA9505::Interrupt::Disable)
    {
        m_interrupts[offset] &= ~interrupt;
    }
    else
    {
        m_interrupts[offset] |= interrupt;
    }

    m_i2c->TransmitFrameToRegister(m_address, 0x08 + offset, {m_ports[offset].port});
    m_i2c->TransmitFrameToRegister(m_address, 0x10 + offset, {m_polarities[offset]});
    m_i2c->TransmitFrameToRegister(m_address, 0x18 + offset, {m_directions[offset]});
    m_i2c->TransmitFrameToRegister(m_address, 0x20 + offset, {m_interrupts[offset]});
}

void Pca9505Module::ConfigurePort(
  PCA9505::Ports port, uint8_t directions, uint8_t polarities, uint8_t interrupts, uint8_t states)
{
    uint8_t portId = (uint8_t)port;

    m_ports[portId].port = states;
    m_directions[portId] = directions;
    m_polarities[portId] = polarities;
    m_interrupts[portId] = interrupts;

    m_i2c->TransmitFrameToRegister(m_address, 0x08 + portId, {m_ports[portId].port});
    m_i2c->TransmitFrameToRegister(m_address, 0x10 + portId, {m_polarities[portId]});
    m_i2c->TransmitFrameToRegister(m_address, 0x18 + portId, {m_directions[portId]});
    m_i2c->TransmitFrameToRegister(m_address, 0x20 + portId, {m_interrupts[portId]});
}

bool Pca9505Module::ReadPin(PCA9505::Ports port, PCA9505::Pins pin)
{
    // Just return the cached value.
    uint8_t mask = 0xFF ^ (0x01 << (uint8_t)pin);

    return (bool)(ReadPort(port).port & mask);
}

PCA9505::PortState Pca9505Module::ReadPort(PCA9505::Ports port)
{
    auto frame = m_i2c->ReceiveFrameFromRegister(m_address, 0x80, 5);

    auto frameData = frame.data.begin();
    for (auto& p : m_ports)
    {
        p.port = *frameData++;
    }

    return m_ports[(uint8_t)port];
}

void Pca9505Module::WritePin(PCA9505::Ports port, PCA9505::Pins pin, bool state)
{
    uint8_t portState = 0x01 << (uint8_t)pin;

    if (state == false)
    {
        m_ports[(uint8_t)port].port &= ~portState;
    }
    else
    {
        m_ports[(uint8_t)port].port |= portState;
    }

    m_i2c->TransmitFrameToRegister(m_address, 0x08 + (uint8_t)port, {m_ports[(uint8_t)port].port});
}

void Pca9505Module::WritePort(PCA9505::Ports port, uint8_t state)
{
    m_ports[(uint8_t)port].port = state;

    m_i2c->TransmitFrameToRegister(m_address, 0x08 + (uint8_t)port, {m_ports[(uint8_t)port].port});
}
#endif
