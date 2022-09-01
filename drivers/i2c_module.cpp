/**
 * @file        i2c_module.cpp
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
#include "i2c_module.h"

#if defined(NILAI_USE_I2C)
#    include "../services/logger.h"
#    include <utility>

#    define I2C_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.data(), ##__VA_ARGS__)
#    define I2C_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.data(), ##__VA_ARGS__)


namespace Nilai::Drivers
{
/**
 * If the initialization passed, the POST passes.
 * @return
 */
bool I2cModule::DoPost()
{
    I2C_INFO("POST OK");
    return true;
}

void I2cModule::Run()
{
}

void I2cModule::TransmitFrame(uint8_t addr, const uint8_t* data, size_t len)
{
    if (HAL_I2C_Master_Transmit(m_handle,
                                addr,
                                const_cast<uint8_t*>(data),
                                static_cast<uint16_t>(len),
                                I2cModule::TIMEOUT) != HAL_OK)
    {
        I2C_ERROR("Unable to transmit frame");
    }
}

void I2cModule::TransmitFrameToRegister(uint8_t        addr,
                                        uint8_t        regAddr,
                                        const uint8_t* data,
                                        size_t         len)
{
    if (HAL_I2C_Mem_Write(m_handle,
                          addr,
                          regAddr,
                          sizeof(regAddr),
                          const_cast<uint8_t*>(data),
                          static_cast<uint16_t>(len),
                          I2cModule::TIMEOUT) != HAL_OK)
    {
        I2C_ERROR("Unable to transmit frame to register");
    }
}

I2C::Frame I2cModule::ReceiveFrame(uint8_t addr, size_t len)
{
    I2C::Frame frame;

    frame.deviceAddress = addr;
    // Allocate memory for the data.
    frame.data.resize(len);

    if (HAL_I2C_Master_Receive(m_handle,
                               addr,
                               frame.data.data(),
                               static_cast<uint16_t>(frame.data.size()),
                               I2cModule::TIMEOUT) != HAL_OK)
    {
        I2C_ERROR("Unable to receive frame");
    }

    return frame;
}

I2C::Frame I2cModule::ReceiveFrameFromRegister(uint8_t addr, uint8_t regAddr, size_t len)
{
    I2C::Frame frame;

    frame.deviceAddress   = addr;
    frame.registerAddress = regAddr;
    frame.data.resize(len);

    if (HAL_I2C_Mem_Read(m_handle,
                         frame.deviceAddress,
                         frame.registerAddress,
                         sizeof(frame.registerAddress),
                         frame.data.data(),
                         static_cast<uint16_t>(frame.data.size()),
                         I2cModule::TIMEOUT) != HAL_OK)
    {
        I2C_ERROR("Unable to receive frame from register");
    }

    return frame;
}

bool I2cModule::CheckIfDevOnBus(uint8_t addr, size_t attempts, size_t timeout)
{
    return HAL_I2C_IsDeviceReady(m_handle, addr, attempts, timeout) == HAL_OK;
}
}    // namespace Nilai::Drivers

#endif
/* ----- END OF FILE ----- */
