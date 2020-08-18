/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  uart
 * @{
 * @file        uartModule.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/13  -  09:25
 *
 * @brief       UART communication module
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"
#include "shared/defines/driverModule.hpp"
#include "shared/defines/misc.hpp"
#include "shared/services/ticks.hpp"

#ifdef HAL_UART_MODULE_ENABLED
#include "Core/Inc/usart.h"
#endif


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define UART1_MODULE (UART::UARTModule::GetInstance(0))
#define UART2_MODULE (UART::UARTModule::GetInstance(1))
#define UART3_MODULE (UART::UARTModule::GetInstance(2))
#define UART4_MODULE (UART::UARTModule::GetInstance(3))


namespace UART
{


/*************************************************************************************************/
/* Enumerated Types ---------------------------------------------------------------------------- */

#pragma region Status
/**
 * @addtogroup  UART_Status
 * @brief       UART module status, mostly describing error states.
 */
enum class Status
{
    OK                 = 0x0000,    //!< UART_OK.
    NOT_ENABLED        = 0x0001,    //!< UART_NOT_ENABLED.
    TIMEOUT            = 0x0002,    //!< UART_TIMEOUT.
    BUSY               = 0x0004,    //!< UART_BUSY.
    ERROR              = 0x0008,    //!< UART_ERROR.
    DROPPED_BYTE       = 0x0010,    //!< UART_DROPPED_BYTE.
    RX_BUFF_OVERFLOW   = 0x0020,    //!< UART_RX_BUFF_OVERFLOW.
    NOT_DONE_RECEIVING = 0x0040,    //!< UART_NOT_DONE_RECEIVING.
    BAD_START_OF_FRAME = 0x0080,    //!< UART_BAD_START_OF_FRAME.
};

/** From: https://stackoverflow.com/a/15889501 */
[[nodiscard]] constexpr inline Status operator|(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                               static_cast<std::underlying_type_t<Status>>(b));
}
[[nodiscard]] constexpr inline Status operator&(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) noexcept
{
    return a = a | b;
}
/**
 * @}
 */
#pragma endregion


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */

template<typename DataType = uint8_t>
class Sequence
{
private:
    bool                  m_useSof = false;
    bool                  m_useEof = false;
    std::vector<DataType> m_startOfFrame{};
    std::vector<DataType> m_endOfFrame{};

public:
    Sequence() = default;

    ALWAYS_INLINE void SetSOF(std::initializer_list<DataType> startOfFrameSequence)
    {
        m_startOfFrame = startOfFrameSequence;
        m_useSof       = true;
    }
    ALWAYS_INLINE void SetEOF(std::initializer_list<DataType> endOfFrameSequence)
    {
        m_endOfFrame = endOfFrameSequence;
        m_useEof     = true;
    }
};

template<typename DataType>
using DriverModuleType = cep::DriverModule<UART_HandleTypeDef, Status, std::vector<DataType>>;

template<typename DataType = uint8_t>
class UartModule : public DriverModuleType<DataType>
{
public:
    using DriverModule = DriverModuleType<DataType>;
    using RxPacket     = typename DriverModule::RxPacket;
    using TxPacket     = typename DriverModule::TxPacket;
    using Callback_t   = typename DriverModule::Callback_t;


private:
    friend class Sequence<DataType>;

    Sequence<DataType> m_sequence;

public:
    UartModule(UART_HandleTypeDef* handle, const std::string_view name) : DriverModule(handle, name)
    {
        /* MX_USARTx_UART_Init must be called in `application.cpp` */

        this->m_status = Status::OK;
    }
    ~UartModule()
    {
        HAL_UART_Abort(this->m_handle);
        HAL_UART_DeInit(this->m_handle);
    }


    /*********************************************************************************************/
    /* Public member functions declarations ---------------------------------------------------- */
    [[nodiscard]] RxPacket ReceivePacket() noexcept override;
    void                   TransmitPacket(const TxPacket& packet) override;

    void AddCallback(const Callback_t& callbackFunc) noexcept override;
    void RemoveCallback(const Callback_t& callbackFunc) override;

    void ErrorHandler(const std::string_view file,
                      const std::string_view func,
                      std::size_t            line) noexcept override;


    /*********************************************************************************************/
    /* Handler --------------------------------------------------------------------------------- */
public:
    void TaskHandler() override;

private:
    ALWAYS_INLINE void ReceptionHandler() noexcept;
    ALWAYS_INLINE void TransmissionHandler() noexcept;


    /*********************************************************************************************/
    /* Accessors ------------------------------------------------------------------------------- */

#pragma region Accessors
public:
    [[nodiscard]] ALWAYS_INLINE static UartModule* GetInstance(size_t moduleIndex = 0);

private:
    ALWAYS_INLINE void   SetInstance(size_t instanceIndex) override;
    ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override;
#pragma endregion
};


/*************************************************************************************************/
}; /* namespace UART */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
