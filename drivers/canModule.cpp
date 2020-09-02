/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  can
 * @{
 * @file        canModule.cpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/10  -  13:56
 *
 * @brief       CAN communication module
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/processes/application.hpp"
#include "shared/drivers/canModule.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <vector>


namespace CAN
{


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define TX_TIMEOUT 500_SysTicks


/*************************************************************************************************/
/* Private functions declarations -------------------------------------------------------------- */
inline static CanModule* FindModuleFromHandle(CAN_HandleTypeDef* hcan) noexcept;
static CAN_FilterTypeDef AssertAndConvertFilterStruct(const Filter& sFilter) noexcept;



/*************************************************************************************************/
/* Construtors --------------------------------------------------------------------------------- */
template<std::size_t size>
TxPacket::TxPacket(uint32_t id, const std::array<uint8_t, size>& packetData, bool isExtended)
{
    static_assert(size <= 8, "The maximum data size is 8 bytes");

    CreatePacket(id, size, isExtended);

    std::copy(packetData.begin(), packetData.end(), data.begin());
}

TxPacket::TxPacket(uint32_t id, const std::vector<uint8_t>& packetData, bool isExtended)
{
    CEP_ASSERT(packetData.size() <= 8, "The maximum data size is 8 bytes");

    CreatePacket(id, packetData.size(), isExtended);

    std::copy(packetData.begin(), packetData.end(), data.begin());
}


/*************************************************************************************************/
/* Handler ------------------------------------------------------------------------------------- */
void CanModule::TransmissionHandler() noexcept
{
    static FastTick_t timeoutTime = FastTick_t::npos;

    HAL_CAN_GetTxMailboxesFreeLevel(m_handle);

    /* Clear timeout time if the transmission buffer is empty */
    if (m_txBuffer.empty())
    {
        timeoutTime = FastTick_t::npos;
        return;
    }

    /* Start the timeout if it's not already started */
    if (timeoutTime == FastTick_t::npos)
    {
        timeoutTime = FASTTICK.Get() + TX_TIMEOUT;
    }

    if (HAL_CAN_GetTxMailboxesFreeLevel(m_handle) != 0)
    {
        SendPacket(m_txBuffer.back());
        m_txBuffer.pop_back();
    }
}


/*************************************************************************************************/
/* Public member functions definitions --------------------------------------------------------- */
#pragma region Public member functions definitions

/**
 * @brief   Wait up to `timeout` FastTicks for a mailbox to be free
 * @param   timeout: The max number of FastTicks to wait
 * @return  True if a mailbox is free, false if not
 */
bool CanModule::WaitForFreeTxMailbox(FastTick_t timeout) noexcept
{
    /* Compute timeout time */
    FastTick_t timeoutTime = FASTTICK.Start() + timeout;

    /* Wait for a mailbox to be free */
    while (HAL_CAN_GetTxMailboxesFreeLevel(m_handle) == 0 &&
           FASTTICK.ShouldTimeout(timeoutTime) == false)
    {
        /* Do nothing here, wait */
    }
    FASTTICK.Stop();

    if (HAL_CAN_GetTxMailboxesFreeLevel(m_handle) == 0)
    {
        /* If no mailboxes are available */
        return false;
    }
    else
    {
        return true;
    }
}

void CanModule::ConfigureFilter(const Filter& filter) noexcept
{
    CAN_FilterTypeDef canFilter = AssertAndConvertFilterStruct(filter);

    if (HAL_CAN_ConfigFilter(m_handle, &canFilter) != HAL_OK)
    {
        m_status |= static_cast<Status>(m_handle->ErrorCode);
        CALL_ERROR_HANDLER();
    }
}

void CanModule::ClearFIFO(uint32_t fifoNumber) noexcept
{
    SysTick_t timeout = SYSTICK.FindTimeoutTime(2);
    while (HAL_CAN_GetRxFifoFillLevel(m_handle, fifoNumber) != 0)
    {
        if (SYSTICK.ShouldTimeout(timeout) == true)
        {
            m_status |= Status::ERROR_TIMEOUT;
            CALL_ERROR_HANDLER();
            break;
        }

        RxPacket          tempPacket;
        HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(m_handle,
                                                        fifoNumber,
                                                        &tempPacket.packetInfo,
                                                        tempPacket.data.data());
        /* If there's an error when getting the message, handle it */
        if (status != HAL_OK)
        {
            /* Update the error code */
            m_status |= static_cast<Status>(m_handle->ErrorCode);
            CALL_ERROR_HANDLER();
        }
    }
}

void CanModule::HandleMessageReception(uint32_t fifoNumber) noexcept
{
    RxPacket receivedPacket{};
    
    /* Get the packet */
    HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(m_handle,
                                                    fifoNumber,
                                                    &receivedPacket.packetInfo,
                                                    receivedPacket.data.data());


    /* If there's an error when getting the message, handle it */
    if (status != HAL_OK)
    {
        /* Update the error code */
        m_status |= static_cast<Status>(m_handle->ErrorCode);
        CALL_ERROR_HANDLER();
        return;
    }

    /* Push the received packet on the reception buffer stack */
    receivedPacket.Validate();
    m_rxBuffer.push_back(receivedPacket);
}

#pragma endregion


/*************************************************************************************************/
/* Private member functions definitions -------------------------------------------------------- */
#pragma region Private member function definitions

void CanModule::SendPacket(TxPacket& packet) noexcept
{
    /* Variable that stores in which mailbox (buffer) the packet is sent from */
    uint32_t buff_num = 0;

    /* Add the message to the queue */
    if (HAL_CAN_AddTxMessage(m_handle, &packet.packetInfo, packet.data.data(), &buff_num) != HAL_OK)
    {
        m_status |= static_cast<Status>(m_handle->ErrorCode);
        CALL_ERROR_HANDLER();
    }
}

#pragma region ErrorHandler

void CanModule::ErrorHandler(const std::string_view file,
                             const std::string_view func,
                             size_t            line) noexcept
{
    if (CheckError(Status::ERROR_EWG))
    {
        PRINT_ERROR("CAN Protocol Error Warning");
    }
    if (CheckError(Status::ERROR_EPV))
    {
        PRINT_ERROR("CAN Error Passive");
    }
    if (CheckError(Status::ERROR_BOF))
    {
        PRINT_ERROR("CAN Bus-Off Error");
    }
    if (CheckError(Status::ERROR_STF))
    {
        PRINT_ERROR("CAN Stuffing Error");
    }
    if (CheckError(Status::ERROR_FOR))
    {
        PRINT_ERROR("CAN Form Error");
    }
    if (CheckError(Status::ERROR_ACK))
    {
        PRINT_ERROR("CAN Ack Error");
    }
    if (CheckError(Status::ERROR_BR))
    {
        PRINT_ERROR("CAN Recessive Bit Error");
    }
    if (CheckError(Status::ERROR_BD))
    {
        PRINT_ERROR("CAN Dominant Bit Error");
    }
    if (CheckError(Status::ERROR_CRC))
    {
        PRINT_ERROR("CAN CRC Error");
    }

    if (CheckError(Status::ERROR_RX_FOV0))
    {
        PRINT_ERROR("CAN FIFO0 Overrun Error");
    }
    if (CheckError(Status::ERROR_RX_FOV1))
    {
        PRINT_ERROR("CAN FIFO1 Overrun Error");
    }

    if (CheckError(Status::ERROR_TX_ALST0))
    {
        PRINT_ERROR("CAN Arbitration Lost Tx Failure (Mailbox 0)");
    }
    if (CheckError(Status::ERROR_TX_ALST1))
    {
        PRINT_ERROR("CAN Arbitration Lost Tx Failure (Mailbox 1)");
    }
    if (CheckError(Status::ERROR_TX_ALST2))
    {
        PRINT_ERROR("CAN Arbitration Lost Tx Failure (Mailbox 2)");
    }

    if (CheckError(Status::ERROR_TX_TERR0))
    {
        PRINT_ERROR("CAN Transmit Error Tx Failure (Mailbox 0)");
    }
    if (CheckError(Status::ERROR_TX_TERR1))
    {
        PRINT_ERROR("CAN Transmit Error Tx Failure (Mailbox 1)");
    }
    if (CheckError(Status::ERROR_TX_TERR2))
    {
        PRINT_ERROR("CAN Transmit Error Tx Failure (Mailbox 2)");
    }

    if (CheckError(Status::ERROR_TIMEOUT))
    {
        PRINT_ERROR("CAN Timeout Error");
    }
    if (CheckError(Status::ERROR_NOT_INIT))
    {
        PRINT_ERROR("CAN Module Was Not Initialized");
    }
    if (CheckError(Status::ERROR_NOT_READY))
    {
        PRINT_ERROR("CAN Peripheral Was Not Ready");
    }
    if (CheckError(Status::ERROR_NOT_STARTED))
    {
        PRINT_ERROR("CAN Peripheral Was Not Started");
    }
    if (CheckError(Status::ERROR_PARAM))
    {
        PRINT_ERROR("CAN Parameter Error");
    }

#if USE_HAL_CAN_REGISTER_CALLBACKS
    if (CheckError(Status::ERROR_INVALID_CALLBACK))
    {
        PRINT_ERROR("CAN Invalid Callback Error");
    }
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */

    if (CheckError(Status::ERROR_INTERNAL))
    {
        PRINT_ERROR("Internal CAN Error");
    }
    if (CheckError(Status::ERROR_DROPPED_PKT))
    {
        PRINT_ERROR("Packet received before last one was read.");
    }
    if (CheckError(Status::RCVD_NEW_PACKET))
    {
        PRINT_ERROR("CAN Packet Received (Not an Error)");
    }
    if (CheckError(Status::RCVD_OLD_PACKET))
    {
        PRINT_ERROR("CAN Packet Received (Not an Error)");
    }
    if (CheckError(Status::NO_PACKET_RECEIVED))
    {
        PRINT_ERROR("CAN No Packet Received (Not an Error)");
    }
    if (CheckError(Status::PKT_ABORTED))
    {
        PRINT_ERROR("CAN Transmission Aborted");
    }
    if (CheckError(Status::TX_ERROR))
    {
        PRINT_ERROR("CAN Transmission Error");
    }

    m_status = Status::ERROR_NONE;
}
#pragma endregion


/**
 * @brief   Enable all interrupts source for the CAN controller
 *
 * @note    `CAN_IT_ERROR` interrupt is disabled and must be enabled manually
 *          with the @ref EnableNoAckInterrupt() function.                                       \n
 *          The reason for this disabling is a bug fix, addressing the issue arising when no
 *          drive boards were present on the master board, which caused a crash through retries. \n
 *          NoAcks are now only handled in interruption after the drive board is found
 *          (although there are still retries and a timeout).
 */
void CanModule::EnableInterrupts() noexcept
{
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_RX_FIFO0_OVERRUN);

    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_RX_FIFO1_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_RX_FIFO1_FULL);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_RX_FIFO1_OVERRUN);

    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_ERROR_WARNING);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_ERROR_PASSIVE);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_BUSOFF);
    __HAL_CAN_ENABLE_IT(m_handle, CAN_IT_LAST_ERROR_CODE);
    //__HAL_CAN_ENABLE_IT(m_handle, CAN_IT_ERROR);
}

#pragma endregion


/*************************************************************************************************/
/* Private functions definitions --------------------------------------------------------------- */
#pragma region Private functions definitions

inline static CanModule* FindModuleFromHandle(CAN_HandleTypeDef* hcan) noexcept
{
    CEP_ASSERT_NULL(hcan);

    /* Check if the module is can1 */
    CHECK_HANDLE_IS_MODULE(hcan, CAN1_MODULE);

    /* Check if the module is can2 */
    CHECK_HANDLE_IS_MODULE(hcan, CAN2_MODULE);

    /* Module not found, return nullptr */
    return nullptr;
}


static CAN_FilterTypeDef AssertAndConvertFilterStruct(const Filter& sFilter) noexcept
{
    using namespace cep;

    CEP_ASSERT(IS_CAN_FILTER_MODE(Underlying(sFilter.Mode)), "Invalid Filter Mode");
    CEP_ASSERT(IS_CAN_FILTER_SCALE(Underlying(sFilter.Scale)), "Invalid Filter Scale");
    CEP_ASSERT(IS_CAN_FILTER_FIFO(Underlying(sFilter.FifoAssignment)), "Invalid Filter Fifo");
    CEP_ASSERT(IS_CAN_FILTER_ACTIVATION(Underlying(sFilter.Activation)),
               "Invalid Filter Activation");
    CEP_ASSERT(IS_CAN_FILTER_BANK_SINGLE(sFilter.Bank), "Invalid Filter Bank");

    CAN_FilterTypeDef filter = {0};

    filter.FilterIdHigh         = sFilter.FilterId.IdHigh;
    filter.FilterIdLow          = sFilter.FilterId.IdLow;
    filter.FilterMaskIdHigh     = sFilter.MaskId.MaskIdHigh;
    filter.FilterMaskIdLow      = sFilter.MaskId.MaskIdLow;
    filter.FilterBank           = sFilter.Bank;
    filter.FilterFIFOAssignment = Underlying(sFilter.FifoAssignment);
    filter.FilterMode           = Underlying(sFilter.Mode);
    filter.FilterScale          = Underlying(sFilter.Scale);
    filter.FilterActivation     = Underlying(sFilter.Activation);
    filter.SlaveStartFilterBank = 0;

    return filter;
}

#pragma endregion


/*************************************************************************************************/
/* HAL Callback functions ---------------------------------------------------------------------- */
#pragma region Callback functions

/* These callbacks are declared as __weak__ by the HAL, and must have C-style linkage in order to
 * override the HAL-provided functions. */
extern "C"
{
    /**
     * @brief   Reception of a single message in FIFO 0
     * @param   hcan: Pointer to a CAN_HandleTypeDef structure that contains the
     *          configuration information for the specified CAN.
     * @retval  None
     */
    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
    {
        CanModule* module = FindModuleFromHandle(hcan);
        CEP_ASSERT_NULL(module);

        module->HandleMessageReception(CAN_RX_FIFO0);
    }

    /**
     * @brief   Reception of a single message in FIFO 1
     * @param   hcan: Pointer to a CAN_HandleTypeDef structure that contains the configuration
     *                information for the specified CAN.
     * @retval  None
     */
    void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
    {
        CanModule* module = FindModuleFromHandle(hcan);
        CEP_ASSERT_NULL(module);

        module->HandleMessageReception(CAN_RX_FIFO1);
    }

    /**
     * @brief   CAN FIFO 0 Full callback, read all of the messages in that FIFO
     * @param   hcan: Pointer to a CAN_HandleTypeDef structure that contains the configuration
     *                information for the specified CAN.
     * @retval  None
     */
    void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef* hcan)
    {
        CanModule* module = FindModuleFromHandle(hcan);
        CEP_ASSERT_NULL(module);

        /* Read messages until FIFO0 is empty */
        /* Each time a message is read, the fill level (FMPi int CAN_RFiR) is decremented by one. */
        while (HAL_CAN_GetRxFifoFillLevel(module->GetHandle(), CAN_RX_FIFO0) != 0)
        {
            module->HandleMessageReception(CAN_RX_FIFO0);
        }
    }

    /**
     * @brief   CAN FIFO 1 Full callback, read all of the messages in that FIFO
     * @param   hcan: Pointer to a CAN_HandleTypeDef structure that contains the
     *          configuration information for the specified CAN.
     * @retval  None
     */
    void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef* hcan)
    {
        CanModule* module = FindModuleFromHandle(hcan);
        CEP_ASSERT_NULL(module);

        /* Read messages until FIFO1 is empty */
        /* Each time a message is read, the fill level (FMPi int CAN_RFiR) is decremented by one. */
        while (HAL_CAN_GetRxFifoFillLevel(module->GetHandle(), CAN_RX_FIFO1) != 0)
        {
            module->HandleMessageReception(CAN_RX_FIFO1);
        }
    }

    /**
     * @brief   Callback function for CAN errors.
     *          This function is a callback, it is thus called automatically.
     * @param   hcan: Pointer to the CAN peripheral
     * @retval  None
     */
    void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan)
    {
        CanModule* module = FindModuleFromHandle(hcan);
        CEP_ASSERT_NULL(module);

        /* Get the error code */
        module->CurrentStatus() |= static_cast<Status>(HAL_CAN_GetError(module->GetHandle()));

        module->CALL_ERROR_HANDLER();
    }

} /* extern "C" */
#pragma endregion


/*************************************************************************************************/
}; /* namespace CAN */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
