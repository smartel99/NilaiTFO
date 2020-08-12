/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  can
 * @{
 * @file        can.cpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/10  -  13:56
 *
 * @brief       CAN communication module
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/drivers/canModule.hpp"
#include "shared/processes/application.hpp"


namespace CAN
{


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define TX_TIMEOUT  500_SysTicks


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
 * @warning This macro should *only* be called from the `CanModule::ErrorHandler` function.
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


/*************************************************************************************************/
/* Private functions declarations -------------------------------------------------------------- */
inline static CanModule* FindModuleFromHandle(CAN_HandleTypeDef* hcan) noexcept;
static CAN_FilterTypeDef AssertAndConvertFilterStruct(const Filter& sFilter) noexcept;


/*************************************************************************************************/
/* Instance creation --------------------------------------------------------------------------- */
#pragma region Instance creation and management

static std::vector<size_t> s_modules{};

ALWAYS_INLINE CanModule* CanModule::GetInstance(std::size_t moduleIndex)
{
    CanModule* canInstance =
      dynamic_cast<CanModule*>(Application::GetModule(s_modules[moduleIndex]));
    if (canInstance->IsEnabled())
    {
        return canInstance;
    }
    else
    {
        return nullptr;
    }
}
ALWAYS_INLINE void CanModule::SetInstance(size_t instanceIndex)
{
    s_modules.push_back(instanceIndex);
    m_moduleIndex = s_modules.size();
}
ALWAYS_INLINE size_t CanModule::RemoveInstance(size_t moduleIndex)
{
    /* Remove module index from `s_modules` vector using the erase-remove idiom */
    s_modules.erase(std::remove(s_modules.begin(), s_modules.end(), moduleIndex), s_modules.end());

    return s_modules.size();
}

#pragma endregion


/*************************************************************************************************/
/* Handler ------------------------------------------------------------------------------------- */
void CanModule::TaskHandler()
{
    static FastTick_t timeoutTime = FASTTICK.Start() + TX_TIMEOUT;

    
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

/**
 * @brief   Get the latest CAN packet
 * @return  The latest CAN packet received.
 *          If no CAN packets were in the reception buffer and this function was called, an empty,
 *          invalid, CAN packet struct will be returned instead.
 */
RxPacket CanModule::ReceivePacket() noexcept
{
    if (m_rxBuffer.empty() == true)
    {
        // Return empty, invalid, CAN Packet if there are no new received messages
        return RxPacket{};
    }

    /* Copies the last value of the reception buffer and delete the original, return the copy */
    RxPacket temp = m_rxBuffer.back();
    m_rxBuffer.pop_back();
    return temp;
}

/**
 * @brief   Add a new packet to the transmit queue, to be transmitted when a new mailbox is
 *          available.
 * @param   packet: Packet to transmit
 */
void CanModule::TransmitPacket(const TxPacket& packet)
{
    m_txBuffer.push_back(packet);
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
        HAL_StatusTypeDef status =
          HAL_CAN_GetRxMessage(m_handle, fifoNumber, &tempPacket.packet, tempPacket.data.data());
        /* If there's an error when getting the message, handle it */
        if (status != HAL_OK)
        {
            /* Update the error code */
            m_status |= static_cast<Status>(m_handle->ErrorCode);
            CALL_ERROR_HANDLER();
        }
    }
}
/**
 * @brief   Add a new callback function to the callback function stack.
 *          The callback function will be called whenever there is a new CAN message being handled.
 */
void CanModule::AddCallback(const Callback_t& callbackFunc) noexcept
{
    m_callbacks.push_back(callbackFunc);
}

/**
 * @brief   Remove a specific callback function from the callback function stack.
 */
void CanModule::RemoveCallback(const Callback_t& callbackFunc)
{
    /* Comparing std::functions is hard (^-^;) */

    /* clang-format off */
    auto comparison = [&](const Callback_t& compare)
                      {
                         return callbackFunc.target<Callback_t>() == compare.target<Callback_t>();
                      };
    /* clang-format on */


    m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(), comparison),
                      m_callbacks.end());
}

void CanModule::HandleMessageReception(uint32_t fifoNumber) noexcept
{
    RxPacket receivedPacket{};
    /* Get the packet */
    HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(m_handle,
                                                    fifoNumber,
                                                    &receivedPacket.packet,
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

    /* Call the callback functions */
    for (Callback_t& callback : m_callbacks)
    {
        callback(receivedPacket);
    }
}

#pragma endregion


/*************************************************************************************************/
/* Private member functions definitions -------------------------------------------------------- */
#pragma region Private member function definitions

/**
 * @brief   Check if the status flag of the current module has the specified
 *          error code masked within.
 *          Returns true if the error code is part of the status
 */
[[nodiscard]] constexpr bool CanModule::CheckError(Status errorCode) const noexcept
{
    return (m_status & errorCode) == errorCode;
}

#pragma region ErrorHandler

void CanModule::ErrorHandler(const std::string_view file,
                             const std::string_view func,
                             std::size_t            line) noexcept
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
    CanModule* module = CAN1_MODULE;
    if (module != nullptr)
    {
        if (hcan->Instance == module->GetHandle()->Instance)
        {
            return module;
        }
    }

    /* Check if the module is can2 */
    module = CAN2_MODULE;
    if (module != nullptr)
    {
        if (hcan->Instance == module->GetHandle()->Instance)
        {
            return module;
        }
    }

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

#pragma endregion


/*************************************************************************************************/
};    // namespace CAN
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
