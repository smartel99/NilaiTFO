/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  can
 * @{
 * @file        canModule.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/08/10  -  13:27
 *
 * @brief       CAN communication module
 */
#ifndef CAN_MODULE_HPP_
#    define CAN_MODULE_HPP_
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#    if defined(NILAI_USE_CAN)
#        include "stm32f4xx_hal.h"
#        if defined(HAL_CAN_MODULE_ENABLED)
#            include "defines/macros.hpp"
#            include "defines/misc.hpp"
#            include "defines/module.hpp"

#            include <array>
#            include <cstdint>
#            include <functional>
#            include <map>
#            include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Enumerated Types
 * ----------------------------------------------------------------------------
 */

namespace CAN
{
/**
 * @addtogroup  CAN_Status
 * @brief       Enum listing all the possible CAN status
 * @{
 */
enum class Status
{
    //!< No error.
    ERROR_NONE = 0x00000000U,
    //!< Protocol Error Warning.
    ERROR_EWG = 0x00000001U,
    //!< Error Passive.
    ERROR_EPV = 0x00000002U,
    //!< Bus-off error.
    ERROR_BOF = 0x00000004U,
    //!< Stuff error.
    ERROR_STF = 0x00000008U,
    //!< Form error.
    ERROR_FOR = 0x00000010U,
    //!< Acknowledgment error.
    ERROR_ACK = 0x00000020U,
    //!< Bit recessive error.
    ERROR_BR = 0x00000040U,
    //!< Bit dominant error.
    ERROR_BD = 0x00000080U,
    //!< CRC error.
    ERROR_CRC = 0x00000100U,

    //!< Rx FIFO0 overrun error.
    ERROR_RX_FOV0 = 0x00000200U,
    //!< Rx FIFO1 overrun error.
    ERROR_RX_FOV1 = 0x00000400U,

    //!< TxMailbox 0 transmit failure due to arbitration lost.
    ERROR_TX_ALST0 = 0x00000800U,
    //!< TxMailbox 0 transmit failure due to transmit error.
    ERROR_TX_TERR0 = 0x00001000U,
    //!< TxMailbox 1 transmit failure due to arbitration lost.
    ERROR_TX_ALST1 = 0x00002000U,
    //!< TxMailbox 1 transmit failure due to transmit error.
    ERROR_TX_TERR1 = 0x00004000U,
    //!< TxMailbox 2 transmit failure due to arbitration lost.
    ERROR_TX_ALST2 = 0x00008000U,
    //!< TxMailbox 2 transmit failure due to transmit error.
    ERROR_TX_TERR2 = 0x00010000U,

    //!< Timeout error.
    ERROR_TIMEOUT = 0x00020000U,
    //!< Peripheral not initialized.
    ERROR_NOT_INIT = 0x00040000U,
    //!< Peripheral not ready.
    ERROR_NOT_READY = 0x00080000U,
    //!< Peripheral not started.
    ERROR_NOT_STARTED = 0x00100000U,
    //!< Parameter error.
    ERROR_PARAM = 0x00200000U,

#            if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    //!< Invalid Callback error */
    ERROR_INVALID_CALLBACK = 0x00400000U,
#            endif

    //!< Internal error.
    ERROR_INTERNAL = 0x00800000U,
    //!< Packet received before last one has been read.
    ERROR_DROPPED_PKT = 0x01000000U,
    //!< A packet has been read.
    RCVD_NEW_PACKET = 0x02000000U,
    //!< A packet has been read.
    RCVD_OLD_PACKET = 0x04000000U,
    //!< No packet has been received.
    NO_PACKET_RECEIVED = 0x08000000U,
    //!< The transmission was aborted.
    PKT_ABORTED = 0x10000000U,
    //!< An error occurred during transmission.
    TX_ERROR = 0x20000000U,
};

/** From: https://stackoverflow.com/a/15889501 */
constexpr inline Status operator|(Status a, Status b)
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator&(Status a, Status b)
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) { return a = a | b; }
/**
 * @}
 */

enum class FilterMode
{
    //!< Identifier mask mode.
    IdMask = CAN_FILTERMODE_IDMASK,
    //!< Identifier list mode.
    IdList = CAN_FILTERMODE_IDLIST,
};

enum class FilterScale
{
    //!< Two 16-bit filters.
    Scale16bit = CAN_FILTERSCALE_16BIT,
    //!< One 32-bit filter.
    Scale32bit = CAN_FILTERSCALE_32BIT,
};

enum class FilterEnable
{
    Disable = CAN_FILTER_DISABLE,
    Enable  = CAN_FILTER_ENABLE,
};

enum class FilterFifoAssignation
{
    //!< Filter x is assigned to FIFO 0.
    Fifo0 = CAN_FILTER_FIFO0,
    //!< Filter x is assigned to FIFO 1.
    Fifo1 = CAN_FILTER_FIFO1,
};

enum class IdentifierType
{
    Standard = CAN_ID_STD,
    Extended = CAN_ID_EXT,
};

enum class FrameType
{
    Data   = CAN_RTR_DATA,
    Remote = CAN_RTR_REMOTE,
};

enum class RxFifo
{
    //!< CAN Receive FIFO0.
    Fifo0 = CAN_RX_FIFO0,
    //!< CAN Receive FIFO1.
    Fifo1 = CAN_RX_FIFO1,
};

/**
 * @brief CAN filter configuration
 */
struct FilterConfiguration
{
    union {
        struct
        {
            /** Specifies the filter identification number (lower 16 bits for
             *  a 32-bit filter configuration, or the second filter in a dual
             *  16-bit configuration)
             */
            uint16_t idLow;
            /** Secifies the filter identification number (higher 16 bits for a
             ** 32-bit filter configuration, or the first filter in a dual
             ** 16-bit configuration)
             */
            uint16_t idHigh;
        };
        union {
            struct
            {
                uint8_t : 1;
                uint8_t  rtr : 1;
                uint8_t  ide : 1;
                uint32_t extId : 18;
                uint16_t stdId : 11;
            };
            uint32_t fullId;
        } canId;
        uint32_t fullId = 0x00000000;
    } filterId;

    union {
        struct
        {
            /** Depending on the FilterMode, specifies either the filter's mask
             ** in mask mode or the filter's ID in list mode.
             ** (Lower 16 bits for a 32-bit filter configuration, or the
             ** second filter in a dual 16-bit configuration.
             **/
            uint16_t maskIdLow;
            /** Depending on the FilterMode, specifies either the filter's mask
             ** in mask mode or the filter's ID in list mode.
             ** (Higher 16 bits for a 32-bit filter configuration, or the
             ** first filter in a dual 16-bit configuration)
             **/
            uint16_t maskIdHigh;
        };
        union {
            struct
            {
                uint8_t : 1;
                uint8_t  rtr : 1;
                uint8_t  ide : 1;
                uint32_t extId : 18;
                uint16_t stdId : 11;
            };
            uint32_t fullId;
        } canId;
        uint32_t fullId = 0x00000000;
    } maskId;

    //! Specifies the FIFO which will be assigned to the filter.
    FilterFifoAssignation fifo = FilterFifoAssignation::Fifo0;
    //! Specifies the filter bank which will be initialized.
    //! This parameter must be a number between 0 and 13.
    uint8_t bank = 0;
    //! Specifies the filter mode to be initialized.
    FilterMode mode = FilterMode::IdMask;
    //! Specifies the filter scale.
    FilterScale scale = FilterScale::Scale32bit;
    //! Enable or disable the filter.
    FilterEnable activate = FilterEnable::Enable;
};

enum class Irq
{
    TxMailboxEmpty      = CAN_IT_TX_MAILBOX_EMPTY,
    Fifo0MessagePending = CAN_IT_RX_FIFO0_MSG_PENDING,
    Fifo0Full           = CAN_IT_RX_FIFO0_FULL,
    Fifo0Overrun        = CAN_IT_RX_FIFO0_OVERRUN,
    Fifo1MessagePending = CAN_IT_RX_FIFO1_MSG_PENDING,
    Fifo1Full           = CAN_IT_RX_FIFO1_FULL,
    Fifo1Overrun        = CAN_IT_RX_FIFO1_OVERRUN,
    Wakeup              = CAN_IT_WAKEUP,
    SleepAck            = CAN_IT_SLEEP_ACK,
    ErrorWarning        = CAN_IT_ERROR_WARNING,
    ErrorPassive        = CAN_IT_ERROR_PASSIVE,
    BusOffError         = CAN_IT_BUSOFF,
    LastErrorCode       = CAN_IT_LAST_ERROR_CODE,
    ErrorStatus         = CAN_IT_ERROR,
};

struct Frame
{
    CAN_RxHeaderTypeDef    frame;
    std::array<uint8_t, 8> data;
    uint32_t               timestamp = 0;
};
}    // Namespace CAN

class CanModule : public cep::Module
{
public:
    CanModule(CAN_HandleTypeDef* handle, const std::string& label)
        : m_handle(handle), m_label(label)
    {
        CEP_ASSERT(handle != nullptr, "CAN Handle is NULL!");
        m_framesReceived.reserve(5);
        m_callbacks =
            std::map<CAN::Irq, std::function<void( )>>({{CAN::Irq::TxMailboxEmpty, {}},
                                                        {CAN::Irq::Fifo0MessagePending, {}},
                                                        {CAN::Irq::Fifo0Full, {}},
                                                        {CAN::Irq::Fifo0Overrun, {}},
                                                        {CAN::Irq::Fifo1MessagePending, {}},
                                                        {CAN::Irq::Fifo1Full, {}},
                                                        {CAN::Irq::Fifo1Overrun, {}},
                                                        {CAN::Irq::Wakeup, {}},
                                                        {CAN::Irq::SleepAck, {}},
                                                        {CAN::Irq::ErrorWarning, {}},
                                                        {CAN::Irq::ErrorPassive, {}},
                                                        {CAN::Irq::BusOffError, {}},
                                                        {CAN::Irq::LastErrorCode, {}},
                                                        {CAN::Irq::ErrorStatus, {}}});

        HAL_CAN_Start(m_handle);

        LOG_INFO("[%s]: Initialized", m_label.c_str( ));
    }
    virtual ~CanModule( ) override;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    void ConfigureFilter(const CAN::FilterConfiguration& config);

    size_t      GetNumberOfAvailableFrames( ) const { return m_framesReceived.size( ); }
    CAN::Frame  ReceiveFrame( );
    CAN::Status TransmitFrame(uint32_t                    addr,
                              const std::vector<uint8_t>& data          = std::vector<uint8_t>( ),
                              bool                        forceExtended = false);

    void SetCallback(CAN::Irq irq, const std::function<void( )>& callback);
    void ClearCallback(CAN::Irq irq);

    void EnableInterrupt(CAN::Irq irq);
    void EnableInterrupts(const std::vector<CAN::Irq>& irqs)
    {
        for (const auto& irq : irqs)
        {
            EnableInterrupt(irq);
        }
    }
    void DisableInterrupt(CAN::Irq irq);
    void DisableInterrupts(const std::vector<CAN::Irq>& irqs)
    {
        for (const auto& irq : irqs)
        {
            DisableInterrupt(irq);
        }
    }

    void HandleIrq( );

private:
    CAN_FilterTypeDef AssertAndConvertFilterStruct(const CAN::FilterConfiguration& config);
    bool              WaitForFreeMailbox( );
    void              HandleFrameReception(CAN::RxFifo fifo);
    void              HandleTxMailbox0Irq(uint32_t ier);
    void              HandleTxMailbox1Irq(uint32_t ier);
    void              HandleTxMailbox2Irq(uint32_t ier);
    void              HandleRxFifo0Irq(uint32_t ier);
    void              HandleRxFifo1Irq(uint32_t ier);
    void              HandleSleepIrq(uint32_t ier);
    void              HandleWakeupIrq(uint32_t ier);
    void              HandleErrorIrq(uint32_t ier);

private:
    CAN_HandleTypeDef* m_handle = nullptr;
    std::string        m_label  = "";
    CAN::Status        m_status = CAN::Status::ERROR_NONE;

    std::vector<CAN::Frame>                      m_framesReceived;
    std::map<CAN::Irq, std::function<void( )>>   m_callbacks;
    std::map<uint64_t, CAN::FilterConfiguration> m_filters;

    static constexpr uint32_t TIMEOUT = 15;
};
#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO CAN module enabled, but HAL_CAN_MODULE_ENABLED is not defined!
#            endif
#        endif
#    endif
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
