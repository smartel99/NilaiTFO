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
#define UART1_MODULE (UART::UartModule::GetInstance(0))
#define UART2_MODULE (UART::UartModule::GetInstance(1))
#define UART3_MODULE (UART::UartModule::GetInstance(2))
#define UART4_MODULE (UART::UartModule::GetInstance(3))


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

enum class SectionState
{
    NOT_COMPLETE,
    COMPLETE,
};


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class UartModule;

class Sequence_t
{
    friend class UartModule;
    using DataType = uint8_t;

private:
    bool                  m_useSof = false;
    std::vector<DataType> m_startOfFrame{};

    bool                  m_useEof = false;
    std::vector<DataType> m_endOfFrame{};

    bool   m_useLength  = false;
    size_t m_lengthSize = 0;

    size_t m_maxLength = 127;

public:
    Sequence_t() = default;

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

    ALWAYS_INLINE void SetLengthSize(size_t packetLengthSize)
    {
        CEP_ASSERT(packetLengthSize <= 8, "Packet length must fit in a uint64_t");
        m_lengthSize = packetLengthSize;
        m_useLength  = true;
    }

    ALWAYS_INLINE void SetMaxLength(size_t maxLength) { m_maxLength = maxLength; }
};

using DriverModuleType = cep::DriverModule<UART_HandleTypeDef, Status, std::vector<uint8_t>>;
class UartModule : public DriverModuleType
{
public:
    using RxPacket   = typename DriverModuleType::RxPacket_t;
    using TxPacket   = typename DriverModuleType::TxPacket_t;
    using Callback_t = typename DriverModuleType::Callback_t;


    /*********************************************************************************************/
    /* Private member variables ---------------------------------------------------------------- */
private:
    Sequence_t m_sequence;

    struct CurrentPacket_t
    {
        RxPacket data;
        uint64_t length;
    }m_currentPacket;

    public : UartModule(UART_HandleTypeDef* handle, const std::string_view name)
    : DriverModuleType{handle, name}
    {
        /* MX_USARTx_UART_Init must be called in `application.cpp` */

        m_status = Status::OK;
    }
    ~UartModule()
    {
        HAL_UART_Abort(m_handle);
        HAL_UART_DeInit(m_handle);
    }


    /*********************************************************************************************/
    /* Public member functions declarations ---------------------------------------------------- */
    void HandleMessageReception();


    /*********************************************************************************************/
    /* Handlers -------------------------------------------------------------------------------- */
private:
    ALWAYS_INLINE void TransmissionHandler() noexcept override;


    /*********************************************************************************************/
    /* Accessors ------------------------------------------------------------------------------- */

#pragma region Accessors
public:
    GETTER static UartModule* GetInstance(size_t moduleIndex = 0);
    GETTER Sequence_t& Sequence() { return m_sequence; }

private:
    ALWAYS_INLINE void   SetInstance(size_t instanceIndex) override;
    ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override;
#pragma endregion


    /*********************************************************************************************/
    /* Private member function declarations ---------------------------------------------------- */
    void SendPacket(TxPacket& packet) noexcept;
    void ErrorHandler(const std::string_view file,
                      const std::string_view func,
                      size_t                 line) noexcept override;

    GETTER uint8_t ReadSingleByte();

    [[nodiscard]] bool         IsSOFComplete();
    [[nodiscard]] bool         IsEOFComplete();
    [[nodiscard]] SectionState HandleSOF(uint8_t newData);
    [[nodiscard]] SectionState HandleLength(uint8_t newData);
};


/*************************************************************************************************/
}; /* namespace UART */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
