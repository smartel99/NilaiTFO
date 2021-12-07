/**
 ******************************************************************************
 * @file    I2CModule.h
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-12-02
 *
 ******************************************************************************
 */
#ifndef GUARD_I2CMODULE_H
#define GUARD_I2CMODULE_H

#include "defines/module.hpp"

#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace CEP_I2C
{
using CallbackFunc = std::function<void(I2cModule*)>;

enum class CallbackTypes
{
    MasterTxCplt = 0,
    MasterRxCplt,
    SlaveTxCplt,
    SlaveRxCplt,
    Addr,
    ListenCplt,
    MemTxCplt,
    MemRxCplt,
    Error,
    Abort,
    CallbackTypesCount,    //! Not a valid callback type, only used for iterating through the enum.
};
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
    explicit Frame(uint8_t devAddr, std::vector<uint8_t> pData = std::vector<uint8_t>())
    : deviceAddress(devAddr), data(std::move(pData))
    {
    }
    Frame(uint8_t devAddr, uint8_t regAddr, std::vector<uint8_t> pData = std::vector<uint8_t>())
    : deviceAddress(devAddr), registerAddress(regAddr), data(std::move(pData))
    {
    }

    bool operator==(const Frame& o) const
    {
        return (deviceAddress == o.deviceAddress) && (registerAddress == o.registerAddress) &&
               (data == o.data);
    }
};
}    // namespace CEP_I2C

class FrameHash
{
public:
    size_t operator()(const CEP_I2C::Frame& f) const
    {
        return (std::hash<uint8_t> {}(f.deviceAddress) + std::hash<uint8_t> {}(f.registerAddress) +
                std::hash<uint8_t> {}(std::accumulate(f.data.begin(), f.data.end(), 0)));
    }
};

class I2cModule : cep::Module
{
public:
    I2cModule([[maybe_unused]] void* p, std::string label) : m_label(std::move(label)) {}

    ~I2cModule() override = default;
    bool                             DoPost() override { return true; }
    void                             Run() override {}
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    void TransmitFrame(uint8_t addr, const uint8_t* data, size_t len)
    {
        TransmitFrame(addr, std::vector<uint8_t>(data, data + len));
    }
    void TransmitFrame(uint8_t addr, const std::vector<uint8_t>& data)
    {
        TransmitFrame(CEP_I2C::Frame {addr, 0x00, data});
    }
    void TransmitFrame(const CEP_I2C::Frame& frame) { TransmitFrameToRegister(frame); }

    void TransmitFrameToRegister(uint8_t addr, uint8_t regAddr, const uint8_t* data, size_t len)
    {
        TransmitFrameToRegister(addr, regAddr, std::vector<uint8_t> {data, data + len});
    }
    void TransmitFrameToRegister(uint8_t addr, uint8_t regAddr, const std::vector<uint8_t>& data)
    {
        TransmitFrameToRegister(CEP_I2C::Frame {addr, regAddr, data});
    }
    void TransmitFrameToRegister(const CEP_I2C::Frame& frame)
    {
        m_framesTransmitted.push_back(frame);
    }

    CEP_I2C::Frame ReceiveFrame(uint8_t addr, size_t len)
    {
        return ReceiveFrameFromRegister(addr, 0, len);
    }
    CEP_I2C::Frame ReceiveFrameFromRegister(uint8_t addr, uint8_t regAddr, size_t len)
    {
        // Look if we have a response queued for the last transmitted frame.
        if (m_respLut.find(m_framesTransmitted.back()) != m_respLut.end())
        {
            // Return it.
            return m_respLut[m_framesTransmitted.back()];
        }
        else
        {
            // We don't, return a default frame.
            return {};
        }
    }

    /**
     * @note For the TESTs only, not part of normal API.
     * @brief Adds a response Frame that will be "received" when the specified Frame is transmitted.
     * @param trans The frame that will trigger the "reception"
     * @param resp  The frame that will be "received"
     */
    [[maybe_unused]] void AddResponseForTransmission(const CEP_I2C::Frame& trans,
                                                     const CEP_I2C::Frame& resp)
    {
        m_respLut[trans] = resp;
    }

    [[maybe_unused]] [[nodiscard]] const std::vector<CEP_I2C::Frame>& GetTransmittedFrames() const
    {
        return m_framesTransmitted;
    }

    [[maybe_unused]] void ClearTransmittedFrames() { m_framesTransmitted.clear(); }

private:
    std::string m_label;

    std::vector<CEP_I2C::Frame> m_framesTransmitted;

    // Map containing pairs of received frames and frames to send in response.
    std::unordered_map<CEP_I2C::Frame, CEP_I2C::Frame, FrameHash> m_respLut;
};
#endif    // GUARD_I2CMODULE_H
