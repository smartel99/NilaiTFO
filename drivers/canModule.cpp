/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  can
 * @{
 * @file        canModule.cpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/08/10  -  13:56
 *
 * @brief       CAN communication module
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "drivers/canModule.hpp"

#if defined(NILAI_USE_CAN) && defined(HAL_CAN_MODULE_ENABLED)
#    include "services/logger.hpp"

#    include <algorithm>

CanModule::CanModule(CAN_HandleTypeDef* handle, std::string label)
: m_handle(handle), m_label(std::move(label))
{
    CEP_ASSERT(handle != nullptr, "CAN Handle is NULL!");
    m_framesReceived.reserve(5);
    m_callbacks =
      std::map<CEP_CAN::Irq, std::function<void()>>({{CEP_CAN::Irq::TxMailboxEmpty, []() {}},
                                                     {CEP_CAN::Irq::Fifo0MessagePending, []() {}},
                                                     {CEP_CAN::Irq::Fifo0Full, []() {}},
                                                     {CEP_CAN::Irq::Fifo0Overrun, []() {}},
                                                     {CEP_CAN::Irq::Fifo1MessagePending, []() {}},
                                                     {CEP_CAN::Irq::Fifo1Full, []() {}},
                                                     {CEP_CAN::Irq::Fifo1Overrun, []() {}},
                                                     {CEP_CAN::Irq::Wakeup, []() {}},
                                                     {CEP_CAN::Irq::SleepAck, []() {}},
                                                     {CEP_CAN::Irq::ErrorWarning, []() {}},
                                                     {CEP_CAN::Irq::ErrorPassive, []() {}},
                                                     {CEP_CAN::Irq::BusOffError, []() {}},
                                                     {CEP_CAN::Irq::LastErrorCode, []() {}},
                                                     {CEP_CAN::Irq::ErrorStatus, []() {}}});

    HAL_CAN_Start(m_handle);

    LOG_INFO("[%s]: Initialized", m_label.c_str());
}

CanModule::~CanModule()
{
    HAL_CAN_Stop(m_handle);
}

/**
 * If it passes initialization, it passes the POST.
 * @return
 *
 * TODO: Make the POST attempt a communication in loopback mode.
 */
bool CanModule::DoPost()
{
    LOG_INFO("[%s]: POST OK", m_label.c_str());
    return true;
}

void CanModule::Run()
{
}

void CanModule::Reset()
{
    // Copy the initialization parameters, so we can use them again.
    CAN_InitTypeDef params = m_handle->Init;
    HAL_CAN_DeInit(m_handle);
    m_handle->Init = params;
    if (HAL_CAN_Init(m_handle) != HAL_OK)
    {
        CEP_ASSERT(false, "Unable to restart %s!", m_label.c_str());
    }
    HAL_CAN_Start(m_handle);
}

void CanModule::ConfigureFilter(const CEP_CAN::FilterConfiguration& config)
{
    uint64_t hash = ((uint64_t)config.filterId.fullId << 32) | config.maskId.fullId;

    CAN_FilterTypeDef filter = AssertAndConvertFilterStruct(config);

    if (HAL_CAN_ConfigFilter(m_handle, &filter) != HAL_OK)
    {
        CEP_ASSERT(false, "In %s::ConfigureFilter: Unable to configure filter!", m_label.c_str());
    }

    m_filters[hash] = config;
}

CEP_CAN::Frame CanModule::ReceiveFrame()
{
    CEP_CAN::Frame frame = m_framesReceived.back();
    m_framesReceived.pop_back();
    return frame;
}

CEP_CAN::Status CanModule::TransmitFrame(uint32_t                    addr,
                                         const std::vector<uint8_t>& data,
                                         bool                        forceExtended)
{
    return TransmitFrame(addr, data.data(), data.size(), forceExtended);
}


CEP_CAN::Status CanModule::TransmitFrame(uint32_t       addr,
                                         const uint8_t* data,
                                         size_t         len,
                                         bool           forceExtended)
{
    CAN_TxHeaderTypeDef head = BuildTxHeader(addr, len, forceExtended);

    if (!WaitForFreeMailbox())
    {
        LOG_ERROR("In %s::TransmitFrame: Timed out before a Tx mailbox is free", m_label.c_str());
        return CEP_CAN::Status::TX_ERROR;
    }

    uint32_t buffNum = 0;

    // Add frame to the mailbox.
    // Using const_cast here because data.data() is a const uint8_t* and not an uint8_t*.
    if (HAL_CAN_AddTxMessage(m_handle, &head, const_cast<uint8_t*>(data), &buffNum) != HAL_OK)
    {
        LOG_ERROR("In %s::TransmitFrame: Unable to add frame to mailbox", m_label.c_str());
        return CEP_CAN::Status::TX_ERROR;
    }

    return CEP_CAN::Status::ERROR_NONE;
}

void CanModule::SetCallback(CEP_CAN::Irq irq, const std::function<void()>& callback)
{
    if (callback)
    {
        m_callbacks[irq] = callback;
    }
}

void CanModule::ClearCallback(CEP_CAN::Irq irq)
{
    // Empty but valid function.
    m_callbacks[irq] = []() {};
}

void CanModule::EnableInterrupt(CEP_CAN::Irq irq)
{
    __HAL_CAN_ENABLE_IT(m_handle, (uint32_t)irq);
}

void CanModule::DisableInterrupt(CEP_CAN::Irq irq)
{
    __HAL_CAN_DISABLE_IT(m_handle, (uint32_t)irq);
}

void CanModule::HandleIrq()
{
    // CAN Interrupt Register.
    uint32_t ier = m_handle->Instance->IER;

    // If an interrupt source was treated, assume it's the only one.
    if (HandleTxMailbox0Irq(ier)) return;
    if (HandleTxMailbox1Irq(ier)) return;
    if (HandleTxMailbox2Irq(ier)) return;
    if (HandleRxFifo0Irq(ier)) return;
    if (HandleRxFifo1Irq(ier)) return;
    if (HandleSleepIrq(ier)) return;
    if (HandleWakeupIrq(ier)) return;
    if (HandleErrorIrq(ier)) return;
}

void CanModule::HandleFrameReception(CEP_CAN::RxFifo fifo)
{
    CEP_CAN::Frame frame {};
    HAL_CAN_GetRxMessage(m_handle, (uint32_t)fifo, &frame.frame, frame.data.data());
    frame.timestamp = HAL_GetTick();

    m_framesReceived.push_back(frame);

    switch (fifo)
    {
        case CEP_CAN::RxFifo::Fifo0: m_callbacks[CEP_CAN::Irq::Fifo0MessagePending](); break;
        case CEP_CAN::RxFifo::Fifo1: m_callbacks[CEP_CAN::Irq::Fifo1MessagePending](); break;
        default: CEP_ASSERT(false, "In %s::HandleFrameReception, invalid FIFO!", m_label.c_str());
    }
}

bool CanModule::HandleTxMailbox0Irq(uint32_t ier)
{
    // If Tx interrupts are enabled:
    if ((ier & CAN_IT_TX_MAILBOX_EMPTY) != 0)
    {
        uint32_t tsrFlags = m_handle->Instance->TSR;
        // If Interrupt is caused by mailbox 0:
        if ((tsrFlags & CAN_TSR_RQCP0) != 0)
        {
            // Clear the transmission complete flag (plus TXOK0, ALST0 and TERR0).
            // (CAN_FLAG_RQCP0 encapsulates TXOK0, ALST0 and TERR0)
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_RQCP0);

            // Check Mailbox 0 Transmission complete.
            if ((tsrFlags & CAN_TSR_TXOK0) != 0)
            {
                m_callbacks[CEP_CAN::Irq::TxMailboxEmpty]();
            }
            else
            {
                // Check Arbitration Lost flag.
                if ((tsrFlags & CAN_TSR_ALST0) != 0)
                {
                    m_status |= CEP_CAN::Status::ERROR_TX_ALST0;
                }

                // Check Transmission Error flag.
                else if ((tsrFlags & CAN_TSR_TERR0) != 0)
                {
                    m_status |= CEP_CAN::Status::ERROR_TX_TERR0;
                }
                else
                {
                    // Transmission Mailbox 0 abort callback.
#    if USE_HAL_CAN_REGISTER_CALLBACKS == 1
                    // Call registered callback.
                    m_handle->TxMailbox0AbortCallback(m_handle);
#    else
                    // Call weak (surcharged) callback.
                    HAL_CAN_TxMailbox0AbortCallback(m_handle);
#    endif
                }
            }
            return true;
        }
    }
    return false;
}

bool CanModule::HandleTxMailbox1Irq(uint32_t ier)
{
    // If Tx interrupts are enabled:
    if ((ier & CAN_IT_TX_MAILBOX_EMPTY) != 0)
    {
        uint32_t tsrFlags = m_handle->Instance->TSR;
        // If Interrupt is caused by mailbox 1:
        if ((tsrFlags & CAN_TSR_RQCP1) != 0)
        {
            // Clear the transmission complete flag (plus TXOK1, ALST1 and TERR1).
            // (CAN_FLAG_RQCP1 encapsulates TXOK1, ALST1 and TERR1)
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_RQCP1);

            // Check Mailbox 0 Transmission complete.
            if ((tsrFlags & CAN_TSR_TXOK1) != 0)
            {
                m_callbacks[CEP_CAN::Irq::TxMailboxEmpty]();
            }
            else
            {
                // Check Arbitration Lost flag.
                if ((tsrFlags & CAN_TSR_ALST1) != 0)
                {
                    m_status |= CEP_CAN::Status::ERROR_TX_ALST1;
                }

                // Check Transmission Error flag.
                else if ((tsrFlags & CAN_TSR_TERR1) != 0)
                {
                    m_status |= CEP_CAN::Status::ERROR_TX_TERR1;
                }
                else
                {
                    // Transmission Mailbox 1 abort callback.
#    if USE_HAL_CAN_REGISTER_CALLBACKS == 1
                    // Call registered callback.
                    m_handle->TxMailbox1AbortCallback(m_handle);
#    else
                    // Call weak (surcharged) callback.
                    HAL_CAN_TxMailbox1AbortCallback(m_handle);
#    endif
                }
            }
            return true;
        }
    }

    return false;
}

bool CanModule::HandleTxMailbox2Irq(uint32_t ier)
{
    // If Tx interrupts are enabled:
    if ((ier & CAN_IT_TX_MAILBOX_EMPTY) != 0)
    {
        uint32_t tsrFlags = m_handle->Instance->TSR;
        // If Interrupt is caused by mailbox 2:
        if ((tsrFlags & CAN_TSR_RQCP2) != 0)
        {
            // Clear the transmission complete flag (plus TXOK2, ALST2 and TERR2).
            // (CAN_FLAG_RQCP2 encapsulates TXOK2, ALST2 and TERR2)
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_RQCP2);

            // Check Mailbox 0 Transmission complete.
            if ((tsrFlags & CAN_TSR_TXOK2) != 0)
            {
                m_callbacks[CEP_CAN::Irq::TxMailboxEmpty]();
            }
            else
            {
                // Check Arbitration Lost flag.
                if ((tsrFlags & CAN_TSR_ALST2) != 0)
                {
                    m_status |= CEP_CAN::Status::ERROR_TX_ALST2;
                }
                // Check Transmission Error flag.
                else if ((tsrFlags & CAN_TSR_TERR2) != 0)
                {
                    m_status |= CEP_CAN::Status::ERROR_TX_TERR2;
                }
                else
                {
                    // Transmission Mailbox 2 abort callback.
#    if USE_HAL_CAN_REGISTER_CALLBACKS == 1
                    // Call registered callback.
                    m_handle->TxMailbox2AbortCallback(m_handle);
#    else
                    // Call weak (surcharged) callback.
                    HAL_CAN_TxMailbox2AbortCallback(m_handle);
#    endif
                }
            }
            return true;
        }
    }
    return false;
}

bool CanModule::HandleRxFifo0Irq(uint32_t ier)
{
    bool               handled = false;
    volatile uint32_t& rf0r    = m_handle->Instance->RF0R;

    // If FIFO 0 overrun interrupt is enabled:
    if ((ier & CAN_IT_RX_FIFO0_OVERRUN) != 0)
    {
        // If FIFO 0 overrun flag is set:
        if ((rf0r & CAN_RF0R_FOVR0) != 0)
        {
            // Set CAN error code.
            m_status |= CEP_CAN::Status::ERROR_RX_FOV0;

            // Clear flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_FOV0);
            handled = true;
        }
    }

    // If FIFO 0 Full interrupt is enabled:
    if ((ier & CAN_IT_RX_FIFO0_FULL) != 0)
    {
        // If FIFO 0 Full flag is set:
        if ((rf0r & CAN_RF0R_FULL0) != 0)
        {
            // Clear FIFO 0 full flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_FF0);

            // While there's still unread frames in the FIFO:
            while ((rf0r & CAN_RF0R_FMP0) != 0)
            {
                // Read them.
                HandleFrameReception(CEP_CAN::RxFifo::Fifo0);
            }
            handled = true;
        }
    }

    // If FIFO 0 Message Pending interrupt is enabled:
    if ((ier & CAN_IT_RX_FIFO0_MSG_PENDING) != 0)
    {
        // Check if message is still pending.
        if ((m_handle->Instance->RF0R & CAN_RF0R_FMP0) != 0)
        {
            HandleFrameReception(CEP_CAN::RxFifo::Fifo0);
            handled = true;
        }
    }

    return handled;
}

bool CanModule::HandleRxFifo1Irq(uint32_t ier)
{
    bool               handled = false;
    volatile uint32_t& rf1r    = m_handle->Instance->RF1R;

    // If FIFO 1 overrun interrupt is enabled:
    if ((ier & CAN_IT_RX_FIFO1_OVERRUN) != 0)
    {
        // If FIFO 1 overrun flag is set:
        if ((rf1r & CAN_RF1R_FOVR1) != 0)
        {
            // Set CAN error code.
            m_status |= CEP_CAN::Status::ERROR_RX_FOV1;

            // Clear flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_FOV1);
            handled = true;
        }
    }

    // If FIFO 1 Full interrupt is enabled:
    if ((ier & CAN_IT_RX_FIFO1_FULL) != 0)
    {
        // If FIFO 1 Full flag is set:
        if ((rf1r & CAN_RF1R_FULL1) != 0)
        {
            // Clear FIFO 0 full flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_FF1);

            // While there's still unread frames in the FIFO:
            while ((rf1r & CAN_RF1R_FMP1) != 0)
            {
                // Read them.
                HandleFrameReception(CEP_CAN::RxFifo::Fifo1);
            }
            handled = true;
        }
    }

    // If FIFO 1 Message Pending interrupt is enabled:
    if ((ier & CAN_IT_RX_FIFO1_MSG_PENDING) != 0)
    {
        // Check if message is still pending.
        if ((m_handle->Instance->RF1R & CAN_RF1R_FMP1) != 0)
        {
            HandleFrameReception(CEP_CAN::RxFifo::Fifo1);
            handled = true;
        }
    }

    return handled;
}

bool CanModule::HandleSleepIrq(uint32_t ier)
{
    // If Sleep interrupt is enabled:
    if ((ier & CAN_IT_SLEEP_ACK) != 0)
    {
        // If Sleep interrupt flag is set:
        if ((m_handle->Instance->MSR & CAN_MSR_SLAKI) != 0)
        {
            // Clear Sleep interrupt flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_SLAKI);

            m_callbacks[CEP_CAN::Irq::SleepAck]();
            return true;
        }
    }
    return false;
}

bool CanModule::HandleWakeupIrq(uint32_t ier)
{
    // If Wakeup interrupt is enabled:
    if ((ier & CAN_IT_WAKEUP) != 0)
    {
        // If wakeup flag is set:
        if ((m_handle->Instance->MSR & CAN_MSR_WKUI) != 0)
        {
            // Clear the flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_WKU);

            m_callbacks[CEP_CAN::Irq::Wakeup]();
            return true;
        }
    }
    return false;
}

bool CanModule::HandleErrorIrq(uint32_t ier)
{
    // If error interrupt is enabled:
    if ((ier & CAN_IT_ERROR) != 0)
    {
        // If error flag is set:
        if ((m_handle->Instance->MSR & CAN_MSR_ERRI) != 0)
        {
            uint32_t esr = m_handle->Instance->ESR;
            // Check Error Warning flag.
            if (((ier & CAN_IT_ERROR_WARNING) != 0) && ((esr & CAN_ESR_EWGF) != 0))
            {
                // Set status code.
                m_status |= CEP_CAN::Status::ERROR_EWG;

                // No need to clear the flag since it is read only.

                m_callbacks[CEP_CAN::Irq::ErrorWarning];
            }
            // Check error passive flag.
            if (((ier & CAN_IT_ERROR_PASSIVE) != 0) && ((esr & CAN_ESR_EPVF) != 0))
            {
                // Set status code.
                m_status |= CEP_CAN::Status::ERROR_EPV;

                // No need to clear the flag since it is read only.
                m_callbacks[CEP_CAN::Irq::ErrorPassive];
            }
            // Check bus-off flag.
            if (((ier & CAN_IT_BUSOFF) != 0) && ((esr & CAN_ESR_BOFF) != 0))
            {
                // Set status code.
                m_status |= CEP_CAN::Status::ERROR_BOF;

                // No need to clear the flag since it is read only.
                m_callbacks[CEP_CAN::Irq::BusOffError]();
            }
            // Check last error code flag.
            if (((ier & CAN_IT_LAST_ERROR_CODE) != 0) && ((esr & CAN_ESR_LEC) != 0))
            {
                switch (esr & CAN_ESR_LEC)
                {
                    case CAN_ESR_LEC_0:
                        // Stuff Error.
                        m_status |= CEP_CAN::Status::ERROR_STF;
                        break;
                    case CAN_ESR_LEC_1:
                        // Form Error.
                        m_status |= CEP_CAN::Status::ERROR_FOR;
                        break;
                    case (CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
                        // Acknowledgement error.
                        m_status |= CEP_CAN::Status::ERROR_ACK;
                        break;
                    case CAN_ESR_LEC_2:
                        // Bit Recessive Error.
                        m_status |= CEP_CAN::Status::ERROR_BR;
                        break;
                    case (CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
                        // Bit Dominant Error.
                        m_status |= CEP_CAN::Status::ERROR_BD;
                        break;
                    case (CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
                        // CRC Error:
                        m_status |= CEP_CAN::Status::ERROR_CRC;
                        break;
                    default: break;
                }

                // Clear Last Error code Flag.
                CLEAR_BIT(m_handle->Instance->ESR, CAN_ESR_LEC);

                m_callbacks[CEP_CAN::Irq::LastErrorCode]();
            }
            // Clear ERRI Flag.
            __HAL_CAN_CLEAR_FLAG(m_handle, CAN_FLAG_ERRI);

            return true;
        }
    }

    return false;
}

CAN_TxHeaderTypeDef CanModule::BuildTxHeader(uint32_t addr, size_t len, bool forceExtended)
{
    CAN_TxHeaderTypeDef head = {0, 0, 0, 0, 0, (FunctionalState)0};
    head.StdId               = addr & 0x000007FF;
    head.ExtId               = addr & 0x1FFFFFFF;
    // If address is higher than 0x7FF, use extended ID.
    if ((addr > 0x7FF) || forceExtended)
    {
        head.IDE = CAN_ID_EXT;
    }
    else
    {
        head.IDE = CAN_ID_STD;
    }

    // If we have data, this is a data frame. Else it's a remote frame.
    head.RTR = len == 0 ? (uint32_t)CEP_CAN::FrameType::Remote : (uint32_t)CEP_CAN::FrameType::Data;
    // Cap amount of data at 8 bytes.
    head.DLC = std::min(len, (size_t)8);
    return head;
}

/*****************************************************************************/
/* Private method definitions                                                */
/*****************************************************************************/
CAN_FilterTypeDef CanModule::AssertAndConvertFilterStruct(
  const CEP_CAN::FilterConfiguration& config)
{
    CAN_FilterTypeDef filter = {0, 0, 0, 0, 0, (FunctionalState)0, 0, 0, 0, 0};

    filter.FilterIdHigh         = config.filterId.idHigh;
    filter.FilterIdLow          = config.filterId.idLow;
    filter.FilterMaskIdHigh     = config.maskId.maskIdHigh;
    filter.FilterMaskIdLow      = config.maskId.maskIdLow;
    filter.FilterFIFOAssignment = (uint32_t)config.fifo;
    filter.FilterBank           = config.bank;
    filter.FilterMode           = (uint32_t)config.mode;
    filter.FilterScale          = (uint32_t)config.scale;
    filter.FilterActivation     = (uint32_t)config.activate;
    filter.SlaveStartFilterBank = 0;

    return filter;
}

bool CanModule::WaitForFreeMailbox()
{
    uint32_t timeout = HAL_GetTick() + CanModule::s_timeout;

    while (HAL_GetTick() <= timeout)
    {
        if (HAL_CAN_GetTxMailboxesFreeLevel(m_handle) != 0)
        {
            return true;
        }
    }

    return false;
}


#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
