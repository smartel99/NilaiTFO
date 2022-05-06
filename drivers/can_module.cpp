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
#include "drivers/can_module.h"

#if defined(NILAI_USE_CAN) && defined(HAL_CAN_MODULE_ENABLED)
#    include "services/logger.h"

#    include <algorithm>

static std::map<CAN_HandleTypeDef*, Nilai::Drivers::CanModule*> s_modules = {};
namespace Nilai::Drivers
{
CanModule::CanModule(CAN_HandleTypeDef* handle, std::string label)
: m_handle(handle), m_label(std::move(label))
{
    CEP_ASSERT(handle != nullptr, "CAN Handle is NULL!");
    m_framesReceived.reserve(5);
    m_callbacks =
      std::map<Can::Irq, std::function<void()>>({{Can::Irq::TxMailboxEmpty, []() {}},
                                                 {Can::Irq::Fifo0MessagePending, []() {}},
                                                 {Can::Irq::Fifo0Full, []() {}},
                                                 {Can::Irq::Fifo0Overrun, []() {}},
                                                 {Can::Irq::Fifo1MessagePending, []() {}},
                                                 {Can::Irq::Fifo1Full, []() {}},
                                                 {Can::Irq::Fifo1Overrun, []() {}},
                                                 {Can::Irq::Wakeup, []() {}},
                                                 {Can::Irq::SleepAck, []() {}},
                                                 {Can::Irq::ErrorWarning, []() {}},
                                                 {Can::Irq::ErrorPassive, []() {}},
                                                 {Can::Irq::BusOffError, []() {}},
                                                 {Can::Irq::LastErrorCode, []() {}},
                                                 {Can::Irq::ErrorStatus, []() {}}});

    HAL_CAN_Start(m_handle);

#    if defined(NILAI_CAN_REGISTER_CALLBACKS) || 1
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_TX_MAILBOX0_COMPLETE_CB_ID, &CanTxMailbox0CpltCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_TX_MAILBOX1_COMPLETE_CB_ID, &CanTxMailbox1CpltCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_TX_MAILBOX2_COMPLETE_CB_ID, &CanTxMailbox2CpltCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_TX_MAILBOX0_ABORT_CB_ID, &CanTxMailbox0AbortCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_TX_MAILBOX1_ABORT_CB_ID, &CanTxMailbox1AbortCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_TX_MAILBOX2_ABORT_CB_ID, &CanTxMailbox2AbortCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, &CanRxFifo0MsgPendingCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_RX_FIFO0_FULL_CB_ID, &CanRxFifo0FullCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID, &CanRxFifo1MsgPendingCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_RX_FIFO1_FULL_CB_ID, &CanRxFifo1FullCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_SLEEP_CB_ID, &CanSleepCallback);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID, &CanWakeUpFromRxCb);
    HAL_CAN_RegisterCallback(m_handle, HAL_CAN_ERROR_CB_ID, &CanErrorCb);
#    endif

    s_modules[handle] = this;

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

void CanModule::ConfigureFilter(const Can::FilterConfiguration& config)
{
    uint64_t hash = ((uint64_t)config.filterId.fullId << 32) | config.maskId.fullId;

    CAN_FilterTypeDef filter = AssertAndConvertFilterStruct(config);

    if (HAL_CAN_ConfigFilter(m_handle, &filter) != HAL_OK)
    {
        CEP_ASSERT(false, "In %s::ConfigureFilter: Unable to configure filter!", m_label.c_str());
    }

    m_filters[hash] = config;
}

Can::Frame CanModule::ReceiveFrame()
{
    Can::Frame frame = m_framesReceived.back();
    m_framesReceived.pop_back();
    return frame;
}

Can::Status CanModule::TransmitFrame(uint32_t                    addr,
                                     const std::vector<uint8_t>& data,
                                     bool                        forceExtended)
{
    return TransmitFrame(addr, data.data(), data.size(), forceExtended);
}


Can::Status CanModule::TransmitFrame(uint32_t       addr,
                                     const uint8_t* data,
                                     size_t         len,
                                     bool           forceExtended)
{
    CAN_TxHeaderTypeDef head = BuildTxHeader(addr, len, forceExtended);

    if (!WaitForFreeMailbox())
    {
        LOG_ERROR("In %s::TransmitFrame: Timed out before a Tx mailbox is free", m_label.c_str());
        return Can::Status::TX_ERROR;
    }

    uint32_t buffNum = 0;

    // Add frame to the mailbox.
    // Using const_cast here because data.data() is a const uint8_t* and not an uint8_t*.
    if (HAL_CAN_AddTxMessage(m_handle, &head, const_cast<uint8_t*>(data), &buffNum) != HAL_OK)
    {
        LOG_ERROR("In %s::TransmitFrame: Unable to add frame to mailbox", m_label.c_str());
        return Can::Status::TX_ERROR;
    }

    return Can::Status::ERROR_NONE;
}

void CanModule::SetCallback(Can::Irq irq, const Callback& callback)
{
    if (callback)
    {
        m_callbacks[irq] = callback;
    }
}

void CanModule::ClearCallback(Can::Irq irq)
{
    // Empty but valid function.
    m_callbacks[irq] = [](const CanModule&) {};
}

void CanModule::EnableInterrupt(Can::Irq irq)
{
    __HAL_CAN_ENABLE_IT(m_handle, (uint32_t)irq);
}

void CanModule::DisableInterrupt(Can::Irq irq)
{
    __HAL_CAN_DISABLE_IT(m_handle, (uint32_t)irq);
}

void CanModule::HandleFrameReception(Can::RxFifo fifo)
{
    Can::Frame frame {};
    HAL_CAN_GetRxMessage(m_handle, (uint32_t)fifo, &frame.frame, frame.data.data());
    frame.timestamp = GetTime();

    m_framesReceived.push_back(frame);

    switch (fifo)
    {
        case Can::RxFifo::Fifo0: m_callbacks[Can::Irq::Fifo0MessagePending](*this); break;
        case Can::RxFifo::Fifo1: m_callbacks[Can::Irq::Fifo1MessagePending](*this); break;
        default: CEP_ASSERT(false, "In %s::HandleFrameReception, invalid FIFO!", m_label.c_str());
    }
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
    head.RTR = len == 0 ? (uint32_t)Can::FrameType::Remote : (uint32_t)Can::FrameType::Data;
    // Cap amount of data at 8 bytes.
    head.DLC = std::min(len, (size_t)8);
    return head;
}

/*****************************************************************************/
/* Private method definitions                                                */
/*****************************************************************************/
CAN_FilterTypeDef CanModule::AssertAndConvertFilterStruct(const Can::FilterConfiguration& config)
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
    uint32_t timeout = GetTime() + CanModule::s_timeout;

    while (GetTime() <= timeout)
    {
        if (HAL_CAN_GetTxMailboxesFreeLevel(m_handle) != 0)
        {
            return true;
        }
    }

    return false;
}

void CanModule::CanTxMailbox0CpltCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::TxMailboxEmpty](*this);
}

void CanModule::CanTxMailbox1CpltCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::TxMailboxEmpty](*this);
}

void CanModule::CanTxMailbox2CpltCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::TxMailboxEmpty](*this);
}

void CanModule::CanTxMailbox0AbortCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::TxMailboxEmpty](*this);
}

void CanModule::CanTxMailbox1AbortCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::TxMailboxEmpty](*this);
}

void CanModule::CanTxMailbox2AbortCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::TxMailboxEmpty](*this);
}

void CanModule::CanRxFifo0MsgPendingCb(CAN_HandleTypeDef* can)
{
    HandleFrameReception(Can::RxFifo::Fifo0);
}

void CanModule::CanRxFifo0FullCallback(CAN_HandleTypeDef* can)
{
    HandleFrameReception(Can::RxFifo::Fifo0);
}

void CanModule::CanRxFifo1MsgPendingCb(CAN_HandleTypeDef* can)
{
    HandleFrameReception(Can::RxFifo::Fifo1);
}

void CanModule::CanRxFifo1FullCallback(CAN_HandleTypeDef* can)
{
    HandleFrameReception(Can::RxFifo::Fifo1);
}

void CanModule::CanSleepCallback(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::SleepAck](*this);
}

void CanModule::CanWakeUpFromRxCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::Wakeup](*this);
}

void CanModule::CanErrorCb(CAN_HandleTypeDef* can)
{
    s_modules[can]->m_callbacks[Can::Irq::ErrorStatus](*this);
}

#    if !defined(NILAI_CAN_REGISTER_CALLBACKS)
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanTxMailbox0CpltCb(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanTxMailbox1CpltCb(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanTxMailbox2CpltCb(hcan);
}

void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanTxMailbox0AbortCb(hcan);
}

void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanTxMailbox1AbortCb(hcan);
}

void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanTxMailbox2AbortCb(hcan);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanRxFifo0MsgPendingCb(hcan);
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanRxFifo0FullCallback(hcan);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanRxFifo1MsgPendingCb(hcan);
}

void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanRxFifo1FullCallback(hcan);
}

void HAL_CAN_SleepCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanSleepCallback(hcan);
}

void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanWakeUpFromRxCb(hcan);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan)
{
    CanModule::CanErrorCb(hcan);
}
#    endif


}    // namespace Nilai::Drivers

#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
