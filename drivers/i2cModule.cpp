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
#include "i2cModule.hpp"

#if defined(NILAI_USE_I2C) && defined(HAL_I2C_MODULE_ENABLED)
#include <utility>
#    include "../services/logger.hpp"

I2cModule::I2cModule(I2C_HandleTypeDef* handle, std::string label)
: m_handle(handle), m_label(std::move(label))
{
    CEP_ASSERT(handle != nullptr, "In I2cModule: handle is NULL!");
    LOG_INFO("[%s]: Initialized", m_label.c_str());
}

/**
 * If the initialization passed, the POST passes.
 * @return
 */
bool I2cModule::DoPost()
{
    LOG_INFO("[%s]: POST OK", m_label.c_str());
    return true;
}

void I2cModule::Run()
{
}

void I2cModule::TransmitFrame(uint8_t addr, const uint8_t* data, size_t len)
{
    if (HAL_I2C_Master_Transmit(
          m_handle, addr, const_cast<uint8_t*>(data), (uint16_t)len, I2cModule::TIMEOUT) != HAL_OK)
    {
        LOG_ERROR("[%s]: In TransmitFrame, unable to transmit frame", m_label.c_str());
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
                          (uint16_t)len,
                          I2cModule::TIMEOUT) != HAL_OK)
    {
        LOG_ERROR("[%s]: In TransmitFrameToRegister, unable to transmit frame", m_label.c_str());
    }
}

CEP_I2C::Frame I2cModule::ReceiveFrame(uint8_t addr, size_t len)
{
    CEP_I2C::Frame frame;

    frame.deviceAddress = addr;
    // Allocate memory for the data.
    frame.data.resize(len);

    if (HAL_I2C_Master_Receive(
          m_handle, addr, frame.data.data(), (uint16_t)frame.data.size(), I2cModule::TIMEOUT) !=
        HAL_OK)
    {
        LOG_ERROR("[%s]: In ReceiveFrame, unable to receive frame", m_label.c_str());
    }

    return frame;
}

CEP_I2C::Frame I2cModule::ReceiveFrameFromRegister(uint8_t addr, uint8_t regAddr, size_t len)
{
    CEP_I2C::Frame frame;

    frame.deviceAddress   = addr;
    frame.registerAddress = regAddr;
    frame.data.resize(len);

    if (HAL_I2C_Mem_Read(m_handle,
                         frame.deviceAddress,
                         frame.registerAddress,
                         sizeof(frame.registerAddress),
                         frame.data.data(),
                         (uint16_t)frame.data.size(),
                         I2cModule::TIMEOUT) != HAL_OK)
    {
        LOG_ERROR("[%s]: In ReceiveFrameFromRegister, unable to receive frame", m_label.c_str());
    }

    return frame;
}
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
