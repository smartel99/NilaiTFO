/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  can
 * @{
 * @file        canModule.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/10  -  13:27
 *
 * @brief       CAN communication module
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#if 0
#include "shared/defines/pch.hpp"
#include "shared/defines/misc.hpp"
#include "shared/defines/driverModule.hpp"
#include "shared/services/ticks.hpp"

#ifdef HAL_CAN_MODULE_ENABLED
#include "Core/Inc/can.h"
#endif

#include <array>
#include <functional>
#include <vector>


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
namespace CAN
{


/*************************************************************************************************/
/* Enumerated Types ---------------------------------------------------------------------------- */
#pragma region Enumerated types

#pragma region Status
/**
 * @addtogroup  CAN_Status
 * @brief       Enum listing all the possible CAN status
 */
enum class Status
{
    ERROR_NONE = 0x00000000U,    //!< No error.
    ERROR_EWG  = 0x00000001U,    //!< Protocol Error Warning.
    ERROR_EPV  = 0x00000002U,    //!< Error Passive.
    ERROR_BOF  = 0x00000004U,    //!< Bus-off error.
    ERROR_STF  = 0x00000008U,    //!< Stuff error.
    ERROR_FOR  = 0x00000010U,    //!< Form error.
    ERROR_ACK  = 0x00000020U,    //!< Acknowledgment error.
    ERROR_BR   = 0x00000040U,    //!< Bit recessive error.
    ERROR_BD   = 0x00000080U,    //!< Bit dominant error.
    ERROR_CRC  = 0x00000100U,    //!< CRC error.

    ERROR_RX_FOV0 = 0x00000200U,    //!< Rx FIFO0 overrun error.
    ERROR_RX_FOV1 = 0x00000400U,    //!< Rx FIFO1 overrun error.

    ERROR_TX_ALST0 = 0x00000800U,    //!< TxMailbox 0 transmit failure due to arbitration lost.
    ERROR_TX_TERR0 = 0x00001000U,    //!< TxMailbox 0 transmit failure due to transmit error.
    ERROR_TX_ALST1 = 0x00002000U,    //!< TxMailbox 1 transmit failure due to arbitration lost.
    ERROR_TX_TERR1 = 0x00004000U,    //!< TxMailbox 1 transmit failure due to transmit error.
    ERROR_TX_ALST2 = 0x00008000U,    //!< TxMailbox 2 transmit failure due to arbitration lost.
    ERROR_TX_TERR2 = 0x00010000U,    //!< TxMailbox 2 transmit failure due to transmit error.

    ERROR_TIMEOUT     = 0x00020000U,    //!< Timeout error.
    ERROR_NOT_INIT    = 0x00040000U,    //!< Peripheral not initialized.
    ERROR_NOT_READY   = 0x00080000U,    //!< Peripheral not ready.
    ERROR_NOT_STARTED = 0x00100000U,    //!< Peripheral not started.
    ERROR_PARAM       = 0x00200000U,    //!< Parameter error.

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    ERROR_INVALID_CALLBACK = 0x00400000U,    //!< Invalid Callback error */
#endif

    ERROR_INTERNAL     = 0x00800000U,    //!< Internal error.
    ERROR_DROPPED_PKT  = 0x01000000U,    //!< Packet received before last one has been read.
    RCVD_NEW_PACKET    = 0x02000000U,    //!< A packet has been read.
    RCVD_OLD_PACKET    = 0x04000000U,    //!< A packet has been read.
    NO_PACKET_RECEIVED = 0x08000000U,    //!< No packet has been received.
    PKT_ABORTED        = 0x10000000U,    //!< The transmission was aborted.
    TX_ERROR           = 0x20000000U,    //!< An error occurred during transmission.
};

/** From: https://stackoverflow.com/a/15889501 */
[[nodiscard]] constexpr inline Status operator|(Status a, Status b) noexcept
{
    return static_cast<Status>(cep::Underlying(a) | cep::Underlying(b));
}
[[nodiscard]] constexpr inline Status operator&(Status a, Status b) noexcept
{
    return static_cast<Status>(cep::Underlying(a) & cep::Underlying(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) noexcept
{
    return a = a | b;
}
/**
 * @}
 */
#pragma endregion

union Id
{
    struct
    {
        uint8_t : 1;
        uint8_t  rtr : 1;
        uint8_t  ide : 1;
        uint32_t ExtId : 18;
        uint16_t StdId : 11;
    };
    uint32_t FullId;
};


/**
 * @addtogroup CAN_identifier_type
 * @{
 */
enum class IdentifierType
{
    STD = CAN_ID_STD,    //!< Standard ID >
    EXT = CAN_ID_EXT,    //!< Extended ID >
};
/**
 * @}
 */

/**
 * @addtogroup CAN_remote_transmission_request
 * @{
 */
enum class RTRType
{
    DATA   = CAN_RTR_DATA,      //!< Data Frame >
    REMOTE = CAN_RTR_REMOTE,    //!< Remote Frame >
};
/**
 * @}
 */

/**
 * @addtogroup CAN_receive_FIFO_number
 * @{
 */
enum class RxFifo
{
    FIFO0 = CAN_RX_FIFO0,    //!< CAN Receive FIFO 0 >
    FIFO1 = CAN_RX_FIFO1,    //!< CAN Receive FIFO 1�>
};
/**
 * @}
 */

#pragma region Filter
/**
 * @addtogroup CAN_filter_mode
 * @{
 */
enum class FilterMode
{
    IDMASK = CAN_FILTERMODE_IDMASK,    //!< Identifier mask mode >
    IDLIST = CAN_FILTERMODE_IDLIST,    //!< Identifier list mode >
};
/**
 * @}
 */

/**
 * @addtogroup CAN_filter_scale
 * @{
 */
enum class FilterScale
{
    SCALE_16BIT = CAN_FILTERSCALE_16BIT,    //!< Two 16-bit filters >
    SCALE_32BIT = CAN_FILTERSCALE_32BIT,    //!< One 32-bit filter >
};
/**
 * @}
 */

/**
 * @addtogroup CAN_filter_activation
 * @{
 */
enum class FilterEnable
{
    DISABLE = CAN_FILTER_DISABLE,    //!< Disable filter >
    ENABLE  = CAN_FILTER_ENABLE,     //!< Enable filter >
};
/**
 * @}
 */

/**
 * @addtogroup CAN_filter_FIFO
 * @{
 */
enum class FilterFifo
{
    FIFO0 = CAN_FILTER_FIFO0,    //!< Filter x is assigned to FIFO 0
    FIFO1 = CAN_FILTER_FIFO1,    //!< Filter x is assigned to FIFO 1
};
/**
 * @}
 */

/**
 * @addtogroup  CAN_Filter
 * @brief       CAN filter configuration structure definition
 */
struct Filter
{
    union FilterId_t
    {
        struct
        {
            /**
             * Specifies the filter identification number
             * (Lower 16 bits for a 32-bit filter configuration, or the second filter in a dual
             *  16-bit configuration).
             */
            uint16_t IdLow;

            /**
             * Specifies the filter identification number
             * (Higher 16 bits for a 32-bit filter configuration, or the first filter in a dual
             *  16-bit configuration).
             */
            uint16_t IdHigh;
        };
        Id       id;
        uint32_t FullId;

        FilterId_t(uint32_t filterId) : FullId{filterId} {}
    } FilterId;
    union MaskId_t
    {
        struct
        {
            /**
             * Depending on the @ref FilterMode, specifies either the filter's mask in mask mode or
             * the filter's ID in list mode.
             * (Lower 16 bits for a 32-bit filter configuration, or the second filter in a dual
             *  16-bit configuration).
             */
            uint16_t MaskIdLow;

            /**
             * Depending on the @ref FilterMode, specifies either the filter's mask in mask mode or
             * the filter's ID in list mode.
             * (Lower 16 bits for a 32-bit filter configuration, or the second filter in a dual
             *  16-bit configuration)
             */
            uint16_t MaskIdHigh;
        };
        Id       id;
        uint32_t FullMaskId;

        MaskId_t(uint32_t maskId) : FullMaskId{maskId} {}
    } MaskId;


    uint8_t Bank;    //!< Specifies the filter bank which will be initialized.
                     //! This parameter must be a number between 0 and 13.

    FilterFifo   FifoAssignment;    //!< Specifies the FIFO which will be assigned to the filter.
    FilterMode   Mode;              //!< Specifies the filter mode to be initialized.
    FilterScale  Scale;             //!< Specifies the filter scale.
    FilterEnable Activation;        //!< Enable or disable the filter.

    Filter(std::uint32_t filterId,
           std::uint32_t maskId,
           uint8_t       bank           = 0,
           FilterFifo    fifoAssignment = FilterFifo::FIFO0,
           FilterMode    mode           = FilterMode::IDMASK,
           FilterScale   scale          = FilterScale::SCALE_32BIT,
           FilterEnable  activation     = FilterEnable::ENABLE)
    : FilterId{filterId},
      MaskId{maskId},
      Bank{bank},
      FifoAssignment{fifoAssignment},
      Mode{mode},
      Scale{scale},
      Activation{activation} {

      };
};
/**
 * @}
 */
#pragma endregion

#pragma endregion


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */

#pragma region CAN Packets
template<typename CAN_HeaderType>
class PacketBase
{
public:
    CAN_HeaderType         packetInfo;
    std::array<uint8_t, 8> data{0};
};

class RxPacket : public PacketBase<CAN_RxHeaderTypeDef>
{
public:
    bool isValid = false;

    RxPacket()                = default;
    RxPacket(const RxPacket&) = default;

    void Validate() noexcept { isValid = true; }
};

class TxPacket : public PacketBase<CAN_TxHeaderTypeDef>
{
public:
    template<size_t size>
    TxPacket(uint32_t id, const std::array<uint8_t, size>& packetData, bool isExtended);
    TxPacket(uint32_t id, const std::vector<uint8_t>& packetData, bool isExtended);

private:
    constexpr void CreatePacket(uint32_t id, size_t size, bool isExtended) noexcept
    {
        packetInfo.DLC = size;
        packetInfo.RTR = cep::Underlying((size == 0) ? RTRType::REMOTE : RTRType::DATA);
        packetInfo.IDE = cep::Underlying(isExtended ? IdentifierType::EXT : IdentifierType::STD);
        packetInfo.TransmitGlobalTime = FunctionalState::DISABLE;

        /* Place packet in good id parameter */
        (isExtended ? packetInfo.ExtId : packetInfo.StdId) = id;
        (isExtended ? packetInfo.StdId : packetInfo.ExtId) = 0;
    }
};
#pragma endregion

using DriverModuleType = cep::DriverModule<CAN_HandleTypeDef, Status, RxPacket, TxPacket>;

class CanModule : public DriverModuleType
{
public:
    using Callback_t = typename DriverModuleType::Callback_t;


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    CanModule(CAN_HandleTypeDef* handle, const std::string_view label) : DriverModule{handle, label}
    {
        HAL_CAN_Start(m_handle);
        EnableInterrupts();

        m_status = Status::ERROR_NONE;
    }
    ~CanModule()
    {
        HAL_CAN_Stop(m_handle);
        HAL_CAN_DeInit(m_handle);
    }


    /*********************************************************************************************/
    /* Public member functions declarations ---------------------------------------------------- */
public:
    bool WaitForFreeTxMailbox(FastTick_t timeout) noexcept;
    void ConfigureFilter(const Filter& filter) noexcept;
    void ClearFIFO(uint32_t fifoNumber) noexcept;
    void HandleMessageReception(uint32_t fifoNumber) noexcept;

    void ErrorHandler(const std::string_view file,
                      const std::string_view func,
                      size_t            line) noexcept override;


    /*********************************************************************************************/
    /* Handlers -------------------------------------------------------------------------------- */
private:
    /* ReceptionHandler is defined in the DriverModule class */
    ALWAYS_INLINE void TransmissionHandler() noexcept override;


    /*********************************************************************************************/
    /* Accessors ------------------------------------------------------------------------------- */

#pragma region Accessors
public:
    [[nodiscard]] ALWAYS_INLINE static CanModule* GetInstance(size_t moduleIndex = 0);

private:
    ALWAYS_INLINE void   SetInstance(size_t instanceIndex) override;
    ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override;
#pragma endregion


    /*********************************************************************************************/
    /* Private member functions declarations --------------------------------------------------- */
    void SendPacket(TxPacket& packet) noexcept;

    void EnableInterrupts() noexcept;
};



/*************************************************************************************************/
}; /* namespace CAN */
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
