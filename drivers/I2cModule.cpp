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
#include "drivers/I2cModule.hpp"

#if defined(NILAI_USE_I2C) && defined(HAL_I2C_MODULE_ENABLED)
#    include "services/logger.hpp"
#    include <utility>

#    if USE_HAL_I2C_REGISTER_CALLBACKS == 1
std::map<I2C_HandleTypeDef*, I2cModule*> I2cModule::s_modules = {};

#        define REGISTER_CALLBACK(cbId, cb)                                                        \
            do                                                                                     \
            {                                                                                      \
                HAL_StatusTypeDef s = HAL_I2C_RegisterCallback(m_handle, cbId, cb);                \
                                                                                                   \
                                                                                                   \
                                                                                                   \
                if (s != HAL_OK)                                                                   \
                {                                                                                  \
                    I2C_ERROR("Unable to register callback '%s': %i", #cb, s);                     \
                }                                                                                  \
            } while (0)
#    endif

#    define I2C_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define I2C_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define I2C_CRITICAL(msg, ...) LOG_CRITICAL("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)


I2cModule::I2cModule(I2C_HandleTypeDef* handle, std::string label)
: m_handle(handle), m_label(std::move(label))
{
    CEP_ASSERT(handle != nullptr, "In I2cModule: handle is NULL!");

#    if USE_HAL_I2C_REGISTER_CALLBACKS == 1
    m_callbacks.resize(static_cast<size_t>(CEP_I2C::CallbackTypes::CallbackTypesCount));

    RegisterCallbacks();
    s_modules[handle] = this;
#    endif

    I2C_INFO("Initialized");
}


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
#    if USE_HAL_I2C_REGISTER_CALLBACKS == 1
    if (m_hasError)
    {
        m_hasError = false;
        // Iterate through the error code, in case multiple ones occurred simultaneously
        for (uint8_t i = 0; i < 32; i++)
        {
            uint32_t bit = (m_lastError & (1 << i));
            if (bit != 0)
            {
                I2C_ERROR(
                  "An error occurred (0x%08X): %s", bit, I2cStatusToStr(bit));
            }
        }

        // Restart the peripheral.
        if(HAL_I2C_DeInit(m_handle) == HAL_OK)
        {
            if(HAL_I2C_Init(m_handle) != HAL_OK)
            {
                I2C_CRITICAL("Unable to re-initialize the I2C peripheral!");
            }
            else{
                RegisterCallbacks();
                I2C_INFO("I2C Re-initialized");
            }
        }
        else{
            I2C_CRITICAL("Unable to de-initialize the I2C peripheral!");
        }
    }
#endif
}

void I2cModule::TransmitFrame(uint8_t addr, const uint8_t* data, size_t len)
{
    if (HAL_I2C_Master_Transmit(
          m_handle, addr, const_cast<uint8_t*>(data), (uint16_t)len, I2cModule::TIMEOUT) != HAL_OK)
    {
        I2C_ERROR("In TransmitFrame, unable to transmit frame");
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
        I2C_ERROR("In TransmitFrameToRegister, unable to transmit frame");
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
        I2C_ERROR("In ReceiveFrame, unable to receive frame");
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
        I2C_ERROR("In ReceiveFrameFromRegister, unable to receive frame");
    }

    return frame;
}

#    if USE_HAL_I2C_REGISTER_CALLBACKS == 1
//<editor-fold desc="Non-blocking Tx/Rx">
bool I2cModule::TransmitFrameIt(uint8_t addr, const uint8_t* data, size_t len)
{
    if (!WaitForTxCplt())
    {
        I2C_ERROR(
          "In TransmitFrameToRegister, timed out while waiting for current "
          "transmission, aborting.");
        return false;
    }

    if (HAL_I2C_Master_Transmit_IT(m_handle, addr, const_cast<uint8_t*>(data), (uint16_t)len) !=
        HAL_OK)
    {
        I2C_ERROR("In TransmitFrameIt, unable to transmit frame (0x%04X): %s",
                  m_handle->ErrorCode,
                  I2cStatusToStr(m_handle->ErrorCode));
        return false;
    }

    m_isTransmitting = true;
    return true;
}

bool I2cModule::TransmitFrameToRegisterIt(uint8_t        addr,
                                          uint8_t        regAddr,
                                          const uint8_t* data,
                                          size_t         len)
{
    if (!WaitForTxCplt())
    {
        I2C_ERROR(
          "In TransmitFrameToRegisterIt, timed out while waiting for current "
          "transmission, aborting.");
        return false;
    }

    if (HAL_I2C_Mem_Write_IT(
          m_handle, addr, regAddr, sizeof(regAddr), const_cast<uint8_t*>(data), (uint16_t)len) !=
        HAL_OK)
    {
        I2C_ERROR("In TransmitFrameToRegisterIt, unable to transmit frame (0x%04X): %s",
                  m_handle->ErrorCode,
                  I2cStatusToStr(m_handle->ErrorCode));
        return false;
    }

    m_isTransmitting = true;
    return true;
}

bool I2cModule::ReceiveFrameIt(uint8_t addr, size_t len)
{
    if (!WaitForRxCplt())
    {
        I2C_ERROR("In ReceiveFrameIt, timed out while waiting for current reception, aborting.");
        return false;
    }

    m_currentFrame.deviceAddress   = addr;
    m_currentFrame.registerAddress = 0;
    m_currentFrame.data.resize(len);

    if (HAL_I2C_Master_Receive_IT(
          m_handle, addr, m_currentFrame.data.data(), m_currentFrame.data.size()) != HAL_OK)
    {
        I2C_ERROR("In ReceiveFrameIt, unable to receive frame (0x%04X): %s",
                  m_handle->ErrorCode,
                  I2cStatusToStr(m_handle->ErrorCode));
        return false;
    }

    m_isReceiving = true;
    return true;
}

bool I2cModule::ReceiveFrameFromRegisterIt(uint8_t addr, uint8_t regAddr, size_t len)
{
    if (!WaitForRxCplt())
    {
        I2C_ERROR(
          "In ReceiveFrameFromRegisterIt, timed out while waiting for current reception, "
          "aborting.");
        return false;
    }

    m_currentFrame.deviceAddress   = addr;
    m_currentFrame.registerAddress = regAddr;
    m_currentFrame.data.resize(len);

    if (HAL_I2C_Mem_Read_IT(
          m_handle, addr, regAddr, sizeof(regAddr), m_currentFrame.data.data(), len) != HAL_OK)
    {
        I2C_ERROR("In ReceiveFrameFromRegisterIt, unable to receive frame (0x%04X): %s",
                  m_handle->ErrorCode,
                  I2cStatusToStr(m_handle->ErrorCode));
        return false;
    }

    m_isReceiving = true;
    return true;
}
//</editor-fold>

//<editor-fold desc="Flag waiting">
/**
 * Waits up to @c timeout milliseconds for the on-going transmission to complete.
 * @param timeout The maximum time to wait, in milliseconds. Defaults to 200 ms.
 * @returns True if the transmission completed within the allowed time
 * @returns False if the function timed out before the transmission completed
 */
bool I2cModule::WaitForTxCplt(uint32_t timeout) const
{
    return WaitForFlag(m_isTransmitting, timeout);
}

bool I2cModule::WaitForRxCplt(uint32_t timeout) const
{
    return WaitForFlag(m_isReceiving, timeout);
}

bool I2cModule::WaitForFlag(const bool& flag, uint32_t timeout)
{
    uint32_t timeoutTime = HAL_GetTick() + timeout;
    while (HAL_GetTick() < timeoutTime)
    {
        if (!flag)
        {
            return true;
        }
    }

    return false;
}
//</editor-fold>


//<editor-fold desc="Helpers">
constexpr const char* I2cModule::CallbackTypeToStr(CEP_I2C::CallbackTypes types)
{
    switch (types)
    {
        case CEP_I2C::CallbackTypes::MasterTxCplt: return "MasterTxCplt";
        case CEP_I2C::CallbackTypes::MasterRxCplt: return "MasterRxCplt";
        case CEP_I2C::CallbackTypes::SlaveTxCplt: return "SlaveTxCplt";
        case CEP_I2C::CallbackTypes::SlaveRxCplt: return "SlaveRxCplt";
        case CEP_I2C::CallbackTypes::Addr: return "Addr";
        case CEP_I2C::CallbackTypes::ListenCplt: return "ListenCplt";
        case CEP_I2C::CallbackTypes::MemTxCplt: return "MemTxCplt";
        case CEP_I2C::CallbackTypes::MemRxCplt: return "MemRxCplt";
        case CEP_I2C::CallbackTypes::Error: return "Error";
        case CEP_I2C::CallbackTypes::Abort: return "Abort";
        default: return "<UNKNOWN>";
    }
}

constexpr const char* I2cModule::I2cStatusToStr(size_t code)
{
    switch (code)
    {
        case HAL_I2C_ERROR_NONE: return "No error";
        case HAL_I2C_ERROR_BERR: return "Bus Error";
        case HAL_I2C_ERROR_ARLO: return "Arbitration Lost";
        case HAL_I2C_ERROR_AF: return "Acknowledge Failure";
        case HAL_I2C_ERROR_OVR: return "Overrun Error";
        case HAL_I2C_ERROR_DMA: return "DMA Error";
        case HAL_I2C_ERROR_TIMEOUT: return "Timed Out";
        case HAL_I2C_ERROR_SIZE: return "Size Error";
        case HAL_I2C_ERROR_DMA_PARAM: return "DMA Param Error";
        case HAL_I2C_WRONG_START: return "Wrong Start";
#        if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        case HAL_I2C_ERROR_INVALID_CALLBACK: return "Invalid Callback";
#        endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        default: return "<UNKNOWN>";
    }
}

I2cModule* I2cModule::GetModuleFromHandle(I2C_HandleTypeDef* i2c)
{
    // No need to check if the module is present in the map, since this function will only be
    // called by a peripheral that is in the map.
    return s_modules[i2c];
}
//</editor-fold>


//<editor-fold desc="Callbacks">
void I2cModule::RegisterCallbacks() const
{
    REGISTER_CALLBACK(HAL_I2C_MASTER_TX_COMPLETE_CB_ID, &I2CMasterTxCpltCallback);
    REGISTER_CALLBACK(HAL_I2C_MASTER_RX_COMPLETE_CB_ID, &I2CMasterRxCpltCallback);
    REGISTER_CALLBACK(HAL_I2C_SLAVE_TX_COMPLETE_CB_ID, &I2CSlaveTxCpltCallback);
    REGISTER_CALLBACK(HAL_I2C_SLAVE_RX_COMPLETE_CB_ID, &I2CSlaveRxCpltCallback);
    // No callback for Addr, apparently.
    REGISTER_CALLBACK(HAL_I2C_LISTEN_COMPLETE_CB_ID, &I2CListenCpltCallback);
    REGISTER_CALLBACK(HAL_I2C_MEM_TX_COMPLETE_CB_ID, &I2CMemTxCpltCallback);
    REGISTER_CALLBACK(HAL_I2C_MEM_RX_COMPLETE_CB_ID, &I2CMemRxCpltCallback);
    REGISTER_CALLBACK(HAL_I2C_ERROR_CB_ID, &I2CErrorCallback);
    REGISTER_CALLBACK(HAL_I2C_ABORT_CB_ID, &I2CAbortCallback);
}

[[maybe_unused]] void I2cModule::RemoveCallback(CEP_I2C::CallbackTypes type, const std::string& id)
{
    auto it = m_callbacks[static_cast<size_t>(type)].find(id);
    if (it != m_callbacks[static_cast<size_t>(type)].end())
    {
        m_callbacks[static_cast<size_t>(type)].erase(it);
    }
    else
    {
        LOG_WARNING("[%s]: Unable to find a callback of type '%s' called '%s'!",
                    m_label.c_str(),
                    CallbackTypeToStr(type),
                    id.c_str());
    }
}

void I2cModule::PropagateCallback(CEP_I2C::CallbackTypes t)
{
    for (const auto& [id, cb] : m_callbacks[static_cast<size_t>(t)])
    {
        if (cb)
        {
            cb(this);
        }
    }
}

void I2cModule::I2CMasterTxCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module        = GetModuleFromHandle(i2c);
    module->m_isTransmitting = false;
    module->PropagateCallback(CEP_I2C::CallbackTypes::MasterTxCplt);
}

void I2cModule::I2CMasterRxCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module     = GetModuleFromHandle(i2c);
    module->m_isReceiving = false;
    module->m_latestFrame = std::move(module->m_currentFrame);
    module->PropagateCallback(CEP_I2C::CallbackTypes::MasterRxCplt);
}

void I2cModule::I2CSlaveTxCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module        = GetModuleFromHandle(i2c);
    module->m_isTransmitting = false;
    module->PropagateCallback(CEP_I2C::CallbackTypes::SlaveTxCplt);
}

void I2cModule::I2CSlaveRxCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module     = GetModuleFromHandle(i2c);
    module->m_isReceiving = false;
    module->m_latestFrame = std::move(module->m_currentFrame);
    module->PropagateCallback(CEP_I2C::CallbackTypes::SlaveRxCplt);
}

void I2cModule::I2CAddrCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module = GetModuleFromHandle(i2c);
    module->PropagateCallback(CEP_I2C::CallbackTypes::Addr);
}

void I2cModule::I2CListenCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module = GetModuleFromHandle(i2c);
    module->PropagateCallback(CEP_I2C::CallbackTypes::ListenCplt);
}

void I2cModule::I2CMemTxCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module        = GetModuleFromHandle(i2c);
    module->m_isTransmitting = false;
    module->PropagateCallback(CEP_I2C::CallbackTypes::MemTxCplt);
}

void I2cModule::I2CMemRxCpltCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module     = GetModuleFromHandle(i2c);
    module->m_isReceiving = false;
    module->m_latestFrame = std::move(module->m_currentFrame);
    module->PropagateCallback(CEP_I2C::CallbackTypes::MemRxCplt);
}

void I2cModule::I2CErrorCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module        = GetModuleFromHandle(i2c);
    module->m_isTransmitting = false;
    module->m_isReceiving    = false;
    module->m_hasError       = true;
    module->m_lastError      = i2c->ErrorCode;
    module->PropagateCallback(CEP_I2C::CallbackTypes::Error);
}

void I2cModule::I2CAbortCallback(I2C_HandleTypeDef* i2c)
{
    I2cModule* module = GetModuleFromHandle(i2c);
    module->PropagateCallback(CEP_I2C::CallbackTypes::Abort);
}
//</editor-fold>




#    endif
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
