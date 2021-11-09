/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  i2c
 * @{
 * @file        i2cModule.hpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 * @date        2020/08/13  -  09:25
 *
 * @brief       I2C communication module
 */
#ifndef GUARD_I2CMODULE_HPP
#    define GUARD_I2CMODULE_HPP
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#    if defined(NILAI_USE_I2C)
#        include "defines/internalConfig.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_I2C_MODULE_ENABLED)
#            include "defines/macros.hpp"
#            include "defines/misc.hpp"
#            include "defines/module.hpp"

#            include <string>
#            include <utility>
#            include <vector>

namespace CEP_I2C
{
/**
 ** @enum   Status
 ** @brief  I2C status enum, indicating mostly error states
 **/
enum class Status
{
    //!< No error.
    Ok = 0x00000000,
    /** Bus Error.
     ** This error occures when the I2C interface detects an external Stop or
     ** Start condition during an address or a data transfer.
     **/
    BusError = 0x00000001,
    /** Arbitration lost.
     ** This error occurs when the I2C interface detects an arbitration lost
     ** condition.
     **/
    ArbitrationLost = 0x00000002,
    /** Acknowledge Failure.
     ** This error occurs when the interface detects a nonacknowledge bit.
     **/
    AckFailure = 0x00000004,
    /** Overrun/underrun error.
     ** An overrun error can occur in slave mode when clock stretching is
     ** disabled and the I2C interface is receiving data.
     ** The interface has received a byte and the data in the data register
     ** has not been read before the next byte is received by the interface.
     **
     ** Underrun error can occur in slave mode when clock stretching is
     ** disabled and the I2C interface is transmitting data.
     ** The interface has not updated the data register with the next byte
     ** before the clock comes for the next byte.
     **/
    OverrunError     = 0x00000008,
    DmaTransferError = 0x00000010,
    TimeoutError     = 0x00000020,
    SizeError        = 0x00000040,
    DmaParamError    = 0x00000080,
    InvalidCallback  = 0x00000100,
    WrongStart       = 0x00000200,
    NotInit          = 0x00000400,
    BadInit          = 0x00000800,
};

struct Frame
{
    uint8_t              deviceAddress   = 0;
    uint8_t              registerAddress = 0;
    std::vector<uint8_t> data            = {};

    Frame() = default;
    Frame(uint8_t devAddr, std::vector<uint8_t> pData = std::vector<uint8_t>())
    : deviceAddress(devAddr), data(std::move(pData))
    {
    }
    Frame(uint8_t devAddr, uint8_t regAddr, std::vector<uint8_t> pData = std::vector<uint8_t>())
    : deviceAddress(devAddr), registerAddress(regAddr), data(std::move(pData))
    {
    }
};
}    // namespace CEP_I2C

class I2cModule : public cep::Module
{
public:
    I2cModule(I2C_HandleTypeDef* handle, std::string label);
    ~I2cModule() override = default;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    void TransmitFrame(uint8_t addr, const uint8_t* data, size_t len);
    void TransmitFrame(uint8_t addr, const std::vector<uint8_t>& data)
    {
        TransmitFrame(addr, data.data(), data.size());
    }
    void TransmitFrame(const CEP_I2C::Frame& frame)
    {
        TransmitFrame(frame.deviceAddress, frame.data.data(), frame.data.size());
    }

    void TransmitFrameToRegister(uint8_t addr, uint8_t regAddr, const uint8_t* data, size_t len);
    void TransmitFrameToRegister(uint8_t addr, uint8_t regAddr, const std::vector<uint8_t>& data)
    {
        TransmitFrameToRegister(addr, regAddr, data.data(), data.size());
    }
    void TransmitFrameToRegister(const CEP_I2C::Frame& frame)
    {
        TransmitFrameToRegister(
          frame.deviceAddress, frame.registerAddress, frame.data.data(), frame.data.size());
    }

    CEP_I2C::Frame ReceiveFrame(uint8_t addr, size_t len);
    CEP_I2C::Frame ReceiveFrameFromRegister(uint8_t addr, uint8_t regAddr, size_t len);

protected:
    I2C_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;

    static constexpr uint16_t TIMEOUT = 200;
};
#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO I2C Module enabled, but HAL_I2C_MODULE_ENABLED is not defined!
#            endif
#        endif
#    endif
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
