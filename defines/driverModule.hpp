/**
 * @addtogroup  defines
 * @{
 * @addtogroup  module
 * @{
 * @file        driverModule.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/18  -  09:31
 *
 * @brief       Base module for all drivers
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#if 0
#include "shared/defines/pch.hpp"
#include "shared/defines/module.hpp"

#include <functional>
#include <string_view>
#include <vector>


namespace cep
{


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define DRIVER_MODULE_TEMPLATE                                                                     \
    template<typename HandlerType, typename StatusType, typename RxPacket, typename TxPacket>

#define DRIVER_MODULE_DEC DriverModule<HandlerType, StatusType, RxPacket, TxPacket>


/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */

/**
 * @def     PRINT_ERROR
 * @brief   Prints an error from the error handler.
 *
 * @param   msg: The error message to print
 *               This parameter must be a `const char*`, or it won't automatically concatenate at
 *               compile-time with the automatically inserted string, and won't compile.
 *
 * @warning This macro should *only* be called from the `<myDriver>Module::ErrorHandler` function.
 *
 * @example:
 *  PRINT_ERROR("CAN Message Aborted");
 *
 *  Result:
 *  [Master][E] - [canModule.cpp:221 (TransmitPacket)] CAN Message Aborted in can2 module.\r\n
 */
#define PRINT_ERROR(msg) LOG_ERROR_HELPER(msg " in {} module.", file, func, line, m_moduleName)

/**
 * @def     CALL_ERROR_HANDLER
 * @brief   Call the CanModule::ErrorHandler, passing it the current program location as params.
 */
#define CALL_ERROR_HANDLER() ErrorHandler(LOG_CURRENT_LOCATION)

/**
 * @def     CHECK_HANDLE_IS_MODULE
 * @brief   Check if the received handle (named handle) is contained within module `moduleNum`.
 *
 * @note    This macro should only be called from within the `FindModuleFromHandle` function
 * @warning Calling this macro can exit the calling function.
 */
#define CHECK_HANDLE_IS_MODULE(handle, moduleNum)                                                  \
    do                                                                                             \
    {                                                                                              \
        auto* module = (moduleNum);                                                                \
        if (module != nullptr)                                                                     \
        {                                                                                          \
            if ((handle)->Instance == module->GetHandle()->Instance)                               \
            {                                                                                      \
                return module;                                                                     \
            }                                                                                      \
        }                                                                                          \
    } while (false)


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
template<typename HandlerType, typename StatusType, typename RxPacket, typename TxPacket = RxPacket>
class DriverModule : public cep::Module
{
public:
    using RxPacket_t = RxPacket;
    using TxPacket_t = TxPacket;
    using Callback_t = std::function<void(const RxPacket&)>;

    /*********************************************************************************************/
    /* Protected member variables -------------------------------------------------------------- */
protected:
    HandlerType*            m_handle = nullptr;
    StatusType              m_status{};
    std::vector<RxPacket>   m_rxBuffer{};
    std::vector<TxPacket>   m_txBuffer{};
    std::vector<Callback_t> m_callbacks{};


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    DriverModule(HandlerType* handler, const std::string_view moduleName)
    : cep::Module{moduleName}, m_handle{handler}
    {
    }


    /*********************************************************************************************/
    /* Public member functions declarations ---------------------------------------------------- */
public:
    [[nodiscard]] virtual RxPacket ReceivePacket() noexcept;
    virtual void                   TransmitPacket(const TxPacket& packet);
    virtual void                   TransmitPacket(const TxPacket&& packet);


    virtual void ErrorHandler(const std::string_view file,
                              const std::string_view func,
                              size_t                 line) noexcept = 0;

    void AddCallback(const Callback_t& callbackFunc) noexcept;
    void RemoveCallback(const Callback_t& callbackFunc);


    /*********************************************************************************************/
    /* Handler --------------------------------------------------------------------------------- */
public:
    virtual void TaskHandler() override
    {
        TransmissionHandler();
        ReceptionHandler();
    }

private:
    virtual ALWAYS_INLINE void ReceptionHandler() noexcept;
    virtual ALWAYS_INLINE void TransmissionHandler() noexcept = 0;


    /*********************************************************************************************/
    /* Accessors ------------------------------------------------------------------------------- */
public:
#pragma region Accessors
    GETTER HandlerType* GetHandle() const noexcept { return m_handle; }
    GETTER StatusType& CurrentStatus() noexcept { return m_status; }
    GETTER std::vector<RxPacket>& GetRxBuffer() noexcept { return m_rxBuffer; }
    GETTER std::vector<TxPacket>& GetTxBuffer() noexcept { return m_txBuffer; }

private:
    ALWAYS_INLINE void   SetInstance(size_t instanceIndex) override  = 0;
    ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override = 0;
#pragma endregion

    /*********************************************************************************************/
    /* Private & protected member functions declarations --------------------------------------- */
protected:
    [[nodiscard]] constexpr bool CheckError(StatusType errorCode) const noexcept;
};


/*************************************************************************************************/
/* Member functions definitions ---------------------------------------------------------------- */

DRIVER_MODULE_TEMPLATE
void DRIVER_MODULE_DEC::ReceptionHandler() noexcept
{
    /* Check if there are new received messages */
    if (m_rxBuffer.empty() == true)
    {
        return;
    }

    /* Call the callback functions */
    for (Callback_t& callback : m_callbacks)
    {
        callback(m_rxBuffer.back());
    }
    m_rxBuffer.pop_back();
}

/**
 * @brief   Get the latest packet
 * @return  The latest packet received.
 *          If no packets were in the reception buffer and this function was called, an empty,
 *          invalid, packet struct will be returned instead.
 */
DRIVER_MODULE_TEMPLATE
RxPacket DRIVER_MODULE_DEC::ReceivePacket() noexcept
{
    if (m_rxBuffer.empty() == true)
    {
        // Return empty packet if there are no new received messages
        return RxPacket{};
    }

    /* Moves out the last value on top of the buffer stack */
    RxPacket&& temp = std::move(m_rxBuffer.back());
    m_rxBuffer.pop_back();

    return temp;
}

/**
 * @brief   Add a new packet to the transmit stack, to be transmitted when possible.
 * @param   packet: Packet to transmit
 */
DRIVER_MODULE_TEMPLATE
void DRIVER_MODULE_DEC::TransmitPacket(const TxPacket& packet)
{
    /* Copies the value at the end of the buffer */
    m_txBuffer.push_back(packet);
}
DRIVER_MODULE_TEMPLATE
void DRIVER_MODULE_DEC::TransmitPacket(const TxPacket&& packet)
{
    /* Moves the value at the end of the buffer */
    m_txBuffer.insert(m_txBuffer.end(), std::move(packet));
}

/**
 * @brief   Add a new callback function to the callback function stack.
 *          The callback function will be called whenever there is a new CAN message being handled.
 */
DRIVER_MODULE_TEMPLATE
void DRIVER_MODULE_DEC::AddCallback(const Callback_t& callbackFunc) noexcept
{
    m_callbacks.push_back(callbackFunc);
}

/**
 * @brief   Remove a specific callback function from the callback function stack.
 */
DRIVER_MODULE_TEMPLATE
void DRIVER_MODULE_DEC::RemoveCallback(const Callback_t& callbackFunc)
{
    /* Comparing std::functions is hard (^-^;) */

    /* clang-format off */
    auto comparison = [&](const Callback_t& compare)
                      {
                         return callbackFunc.template target<Callback_t>() ==
                                compare.template target<Callback_t>();
                      };
    /* clang-format on */


    m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(), comparison),
                      m_callbacks.end());
}


/**
 * @brief   Check if the status flag of the current module has the specified
 *          error code masked within.
 *          Returns true if the error code is part of the status
 */
DRIVER_MODULE_TEMPLATE [[nodiscard]] constexpr bool
DRIVER_MODULE_DEC::CheckError(StatusType errorCode) const noexcept
{
    return (m_status & errorCode) == errorCode;
}


/*************************************************************************************************/
}; /* namespace cep */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
#endif
