/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  I2C
 * @{
 * @file        i2cModule.cpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 * @date        2020/08/24  -  17:00
 *
 * @brief       i2c communication module
 */
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#include "drivers/i2cModule.hpp"
#include "services/logger.hpp"

void I2cModule::Run()
{
}

void I2cModule::TransmitFrame(uint8_t addr, const uint8_t *data, size_t len)
{
    if (HAL_I2C_Master_Transmit(m_handle, addr, const_cast<uint8_t*>(data), len,
                                I2cModule::TIMEOUT) != HAL_OK)
    {
        LOG_ERROR("In {}::TransmitFrame, unable to transmit frame", m_label);
    }
}

void I2cModule::TransmitFrameToRegister(uint8_t addr, uint8_t regAddr,
                                        const uint8_t *data, size_t len)
{
    if (HAL_I2C_Mem_Write(m_handle, addr, regAddr, sizeof(regAddr),
                          const_cast<uint8_t*>(data), len, I2cModule::TIMEOUT)
            != HAL_OK)
    {
        // #TODO Fix the fucking logging module
        // LOG_ERROR("In {}::TransmitFrameToRegister, unable to transmit frame", m_label);
    }
}

I2C::Frame I2cModule::ReceiveFrame(uint8_t addr, size_t len)
{
    I2C::Frame frame;

    frame.deviceAddress = addr;
    // Allocate memory for the data.
    frame.data.resize(len);

    if (HAL_I2C_Master_Receive(m_handle, addr, frame.data.data(),
                               frame.data.size(), I2cModule::TIMEOUT) != HAL_OK)
    {
        LOG_ERROR("In {}::ReceiveFrame, unable to receive frame", m_label);
    }

    return frame;
}

I2C::Frame I2cModule::ReceiveFrameFromRegister(uint8_t addr, uint8_t regAddr,
                                               size_t len)
{
    I2C::Frame frame;

    frame.deviceAddress = addr;
    frame.registerAddress = regAddr;
    frame.data.resize(len);

    if (HAL_I2C_Mem_Read(m_handle, frame.deviceAddress, frame.registerAddress,
                         sizeof(frame.registerAddress), frame.data.data(),
                         frame.data.size(), I2cModule::TIMEOUT) != HAL_OK)
    {
        // #TODO Fucking logging module doesn't work *AND* causes a crash
        // LOG_ERROR("In {}::ReceiveFrameFromRegister, unable to receive frame", m_label);
    }

    return frame;
}

/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
