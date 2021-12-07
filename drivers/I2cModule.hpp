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

#            if USE_HAL_I2C_REGISTER_CALLBACKS == 1
#                include <map>
class I2cModule;
#            endif

namespace CEP_I2C
{
#            if USE_HAL_I2C_REGISTER_CALLBACKS == 1
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

#            endif

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
;

    // Interrupt-based functionalities is only available when register callbacks are enabled.
#            if USE_HAL_I2C_REGISTER_CALLBACKS == 1
    /**
     *
     * @param addr
     * @param data
     * @param len
     */
    bool                  TransmitFrameIt(uint8_t addr, const uint8_t* data, size_t len);
    [[maybe_unused]] bool TransmitFrameIt(uint8_t addr, const std::vector<uint8_t>& data)
    {
        return TransmitFrameIt(addr, data.data(), data.size());
    }
    [[maybe_unused]] bool TransmitFrameIt(const CEP_I2C::Frame& frame)
    {
        return TransmitFrameIt(frame.deviceAddress, frame.data.data(), frame.data.size());
    }

    /**
     *
     * @param addr
     * @param regAddr
     * @param data
     * @param len
     */
    bool TransmitFrameToRegisterIt(uint8_t addr, uint8_t regAddr, const uint8_t* data, size_t len);
    [[maybe_unused]] bool TransmitFrameToRegisterIt(uint8_t                     addr,
                                                    uint8_t                     regAddr,
                                                    const std::vector<uint8_t>& data)
    {
        return TransmitFrameToRegisterIt(addr, regAddr, data.data(), data.size());
    }
    [[maybe_unused]] bool TransmitFrameToRegisterIt(const CEP_I2C::Frame& frame)
    {
        return TransmitFrameToRegisterIt(
          frame.deviceAddress, frame.registerAddress, frame.data.data(), frame.data.size());
    }

    /**
     * Asks the I2C device at address @c addr to send @c len bytes of data in non-blocking mode.
     * @param addr The address of the device.
     * @param len The number of bytes to receive
     * @returns True if the reception has successfully been started
     * @returns False if the reception has not been started
     */
    [[maybe_unused]] bool ReceiveFrameIt(uint8_t addr, size_t len);

    /**
     * Asks the I2C device at address @c addr to send @c len bytes of data from register @c
     * regAddr in non-blocking mode.
     * @param addr  The address of the device
     * @param regAddr   The register to read data from
     * @param len   The number of bytes to read
     * @returns True if the reception has successfully been started
     * @returns False if the reception has not been started
     */
    [[maybe_unused]] bool ReceiveFrameFromRegisterIt(uint8_t addr, uint8_t regAddr, size_t len);

    /**
     *
     * @return
     */
    [[maybe_unused]] [[nodiscard]] const CEP_I2C::Frame& GetLatestFrame() const
    {
        return m_latestFrame;
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnMasterTxCpltCallback(const std::string&           name,
                                                    const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::MasterTxCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnMasterRxCpltCallback(const std::string&           name,
                                                    const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::MasterRxCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnSlaveTxCpltCallback(const std::string&           name,
                                                   const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::SlaveTxCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnSlaveRxCpltCallback(const std::string&           name,
                                                   const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::SlaveRxCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnAddrCallback(const std::string&           name,
                                            const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::Addr, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnListenCpltCallback(const std::string&           name,
                                                  const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::ListenCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnMemTxCpltCallback(const std::string&           name,
                                                 const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::MemTxCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnMemRxCpltCallback(const std::string&           name,
                                                 const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::MemRxCplt, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnErrorCallback(const std::string&           name,
                                             const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::Error, name, cb);
    }

    /**
     *
     * @param name
     * @param cb
     */
    [[maybe_unused]] void AddOnAbortCallback(const std::string&           name,
                                             const CEP_I2C::CallbackFunc& cb)
    {
        AddCallback(CEP_I2C::CallbackTypes::Abort, name, cb);
    }

    void AddCallback(CEP_I2C::CallbackTypes       type,
                     const std::string&           name,
                     const CEP_I2C::CallbackFunc& cb)
    {
        m_callbacks[static_cast<size_t>(type)][name] = cb;
    }

    [[maybe_unused]] void RemoveCallback(CEP_I2C::CallbackTypes type, const std::string& id);


protected:
    [[nodiscard]] static inline constexpr const char* CallbackTypeToStr(
      CEP_I2C::CallbackTypes types);

    [[nodiscard]] static inline constexpr const char* I2cStatusToStr(size_t code);

    [[nodiscard]] inline bool        WaitForTxCplt(uint32_t timeout = TIMEOUT) const;
    [[nodiscard]] inline bool        WaitForRxCplt(uint32_t timeout = TIMEOUT) const;
    [[nodiscard]] static inline bool WaitForFlag(const bool& flag, uint32_t timeout = TIMEOUT);

    [[nodiscard]] static inline I2cModule* GetModuleFromHandle(I2C_HandleTypeDef* i2c);

    void PropagateCallback(CEP_I2C::CallbackTypes t);
    // Hardware callbacks for the HAL.
    static void I2CMasterTxCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CMasterRxCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CSlaveTxCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CSlaveRxCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CAddrCallback(I2C_HandleTypeDef* i2c);
    static void I2CListenCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CMemTxCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CMemRxCpltCallback(I2C_HandleTypeDef* i2c);
    static void I2CErrorCallback(I2C_HandleTypeDef* i2c);
    static void I2CAbortCallback(I2C_HandleTypeDef* i2c);

protected:
    CEP_I2C::Frame m_currentFrame;
    CEP_I2C::Frame m_latestFrame;

    bool m_isTransmitting = false;
    bool m_isReceiving    = false;

    bool m_hasError = false;
    uint32_t m_lastError = 0;

    std::vector<std::map<std::string, CEP_I2C::CallbackFunc>> m_callbacks;

    static std::map<I2C_HandleTypeDef*, I2cModule*> s_modules;
#            endif

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

#    if defined(NILAI_TEST)
#        include "test/Mocks/drivers/I2cModule.h"
#    endif
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
