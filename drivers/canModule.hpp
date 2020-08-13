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
#include "shared/defines/pch.hpp"
#include "shared/defines/misc.hpp"
#include "shared/defines/module.hpp"
#include "shared/services/ticks.hpp"

#ifdef HAL_CAN_MODULE_ENABLED
#include "Core/Inc/can.h"
#endif


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define CAN1_MODULE (CAN::CanModule::GetInstance(0))
#define CAN2_MODULE (CAN::CanModule::GetInstance(1))


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
    FIFO1 = CAN_RX_FIFO1,    //!< CAN Receive FIFO 1 >
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

        FilterId_t(uint32_t id) : FullId{id} {}
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

        MaskId_t(uint32_t id) : FullMaskId{id} {}
    } MaskId;


    uint8_t Bank;    //!< Specifies the filter bank which will be initialized.
                     //! This parameter must be a number between 0 and 13.

    FilterFifo   FifoAssignment;    //!< Specifies the FIFO which will be assigned to the filter.
    FilterMode   Mode;              //!< Specifies the filter mode to be initialized.
    FilterScale  Scale;             //!< Specifies the filter scale.
    FilterEnable Activation;        //!< Enable or disable the filter.

    Filter(std::uint32_t filterId,
           std::uint32_t maskId,
           uint8_t       bank,
           FilterFifo    fifoAssignment,
           FilterMode    mode       = FilterMode::IDMASK,
           FilterScale   scale      = FilterScale::SCALE_32BIT,
           FilterEnable  activation = FilterEnable::ENABLE)
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
    bool                   isValid = false;
    CAN_HeaderType         packet;
    std::array<uint8_t, 8> data{0};

    void Validate() { isValid = true; }
};

class RxPacket : public PacketBase<CAN_RxHeaderTypeDef>
{
public:
    RxPacket()                = default;
    RxPacket(const RxPacket&) = default;
};

class TxPacket : public PacketBase<CAN_TxHeaderTypeDef>
{
public:
    template<size_t size>
    TxPacket(uint32_t id, const std::array<uint8_t, size>& packetData, bool isExtended)
    {
        static_assert(size <= 8, "The maximum data size is 8 bytes");

        CreatePacket(id, size, isExtended);

        std::copy(packetData.begin(), packetData.end(), data.begin());

        isValid = true;
    }

    TxPacket(uint32_t id, const std::vector<uint8_t>& packetData, bool isExtended)
    {
        CEP_ASSERT(packetData.size() <= 8, "The maximum data size is 8 bytes");

        CreatePacket(id, packetData.size(), isExtended);

        std::copy(packetData.begin(), packetData.end(), data.begin());

        isValid = true;
    }

private:
    constexpr void CreatePacket(uint32_t id, size_t size, bool isExtended) noexcept
    {
        packet.DLC = size;
        packet.RTR = cep::Underlying((size == 0) ? RTRType::REMOTE : RTRType::DATA);
        packet.IDE = cep::Underlying(isExtended ? IdentifierType::EXT : IdentifierType::STD);
        packet.TransmitGlobalTime = FunctionalState::DISABLE;

        /* Place packet in good id parameter */
        (isExtended ? packet.ExtId : packet.StdId) = id;
        (isExtended ? packet.StdId : packet.ExtId) = 0;
    }
};
#pragma endregion

class CanModule : public cep::Module
{
public:
    using Callback_t = std::function<void(RxPacket)>;

    /*********************************************************************************************/
    /* Private member variables ---------------------------------------------------------------- */
private:
    CAN_HandleTypeDef*      m_handle = nullptr;
    CAN::Status             m_status = Status::ERROR_NOT_INIT;
    std::vector<RxPacket>   m_rxBuffer{};
    std::vector<TxPacket>   m_txBuffer{};
    std::vector<Callback_t> m_callbacks{};


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    CanModule(CAN_HandleTypeDef* handle, std::string_view label)
    : cep::Module{label}, m_handle{handle}
    {
        HAL_CAN_Start(m_handle);
        EnableInterrupts();

        Filter filter{420,
                      0,
                      0,
                      FilterFifo::FIFO0,
                      FilterMode::IDMASK,
                      FilterScale::SCALE_32BIT,
                      FilterEnable::ENABLE};

        ConfigureFilter(filter);

        m_status = Status::ERROR_NONE;
    }
    ~CanModule()
    {
        HAL_CAN_Stop(m_handle);
        HAL_CAN_DeInit(m_handle);
    }


    /*********************************************************************************************/
    /* Public member functions declarations ---------------------------------------------------- */
    bool                   WaitForFreeTxMailbox(FastTick_t timeout) noexcept;
    [[nodiscard]] RxPacket ReceivePacket() noexcept;
    void                   TransmitPacket(const TxPacket& packet);

    void ConfigureFilter(const Filter& filter) noexcept;
    void ClearFIFO(uint32_t fifoNumber) noexcept;

    void AddCallback(const Callback_t& callbackFunc) noexcept;
    void RemoveCallback(const Callback_t& callbackFunc);

    void HandleMessageReception(uint32_t fifoNumber) noexcept;

    void ErrorHandler(std::string_view file, std::string_view func, std::size_t line) noexcept;


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
    [[nodiscard]] ALWAYS_INLINE static CanModule* GetInstance(size_t moduleIndex = 0);
    [[nodiscard]] ALWAYS_INLINE CAN_HandleTypeDef* GetHandle() const noexcept { return m_handle; }
    [[nodiscard]] ALWAYS_INLINE Status& CurrentStatus() noexcept { return m_status; }
    [[nodiscard]] ALWAYS_INLINE std::vector<RxPacket>& GetRxBuffer() noexcept { return m_rxBuffer; }
    [[nodiscard]] ALWAYS_INLINE std::vector<TxPacket>& GetTxBuffer() noexcept { return m_txBuffer; }

private:
    ALWAYS_INLINE void   SetInstance(size_t instanceIndex) override;
    ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override;
#pragma endregion


    /*********************************************************************************************/
    /* Private member functions declarations --------------------------------------------------- */
    void SendPacket(TxPacket& packet) noexcept;

    void                         EnableInterrupts() noexcept;
    [[nodiscard]] constexpr bool CheckError(Status errorCode) const noexcept;
};



/*************************************************************************************************/
}; /* namespace CAN */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
