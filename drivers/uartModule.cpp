/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  uart
 * @{
 * @file        uartModule.cpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/13  -  09:40
 *
 * @brief       UART communication module
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/drivers/uartModule.hpp"
#include "shared/processes/application.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <vector>


namespace UART
{


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define TX_TIMEOUT 500_SysTicks

#define IS_IRQ_SRC_DATA_READY(handler) ((handler)->Instance->SR & UART_IT_RXNE)
#define READ_DATA_REG(handler)         ((handler)->Instance->DR & (uint8_t)0xFF)


/*************************************************************************************************/
/* Private function declarations --------------------------------------------------------------- */
inline static UartModule* FindModuleFromHandle(UART_HandleTypeDef* huart) noexcept;


/*************************************************************************************************/
/* Instance creation --------------------------------------------------------------------------- */
#pragma region Instance creation and management

static std::vector<size_t> s_modules{};

ALWAYS_INLINE UartModule* UartModule::GetInstance(std::size_t moduleIndex)
{
    UartModule* uartInstance =
      dynamic_cast<UartModule*>(Application::GetModule(s_modules[moduleIndex]));
    if (uartInstance->IsEnabled())
    {
        return uartInstance;
    }
    else
    {
        return nullptr;
    }
}
ALWAYS_INLINE void UartModule::SetInstance(size_t instanceIndex)
{
    s_modules.push_back(instanceIndex);
    m_moduleIndex = s_modules.size();
}
ALWAYS_INLINE size_t UartModule::RemoveInstance(size_t moduleIndex)
{
    /* Remove module index from `s_modules` vector using the erase-remove idiom */
    s_modules.erase(std::remove(s_modules.begin(), s_modules.end(), moduleIndex), s_modules.end());

    return s_modules.size();
}

#pragma endregion


/*************************************************************************************************/
/* Handler ------------------------------------------------------------------------------------- */
void UartModule::TransmissionHandler() noexcept
{
    /* Clear timeout time if the transmission buffer is empty */
    if (m_txBuffer.empty())
    {
        return;
    }

    SendPacket(m_txBuffer.back());
    m_txBuffer.pop_back();
}


/*************************************************************************************************/
/* Public member functions definitions --------------------------------------------------------- */
void UartModule::HandleMessageReception()
{
    /* Read the received byte and flush the data register */
    uint8_t newData = ReadSingleByte();

    /* Handle Start Of Frame */
    if (m_sequence.m_useSof == true)
    {
        if (HandleSOF(newData) == SectionState::NOT_COMPLETE)
        {
            m_currentPacket.length = 0;
            return;
        }
    }

    /* Handle packet length */
    if (m_sequence.m_useLength == true)
    {
        if (HandleLength(newData) == SectionState::NOT_COMPLETE)
        {
            return;
        }
    }
    
    

    /* Save the newly received data */
    RxPacket& currentPacket = m_currentPacket.data;
    currentPacket.push_back(newData);

    /* Check if the packet has ended */
    if ((currentPacket.size() >= m_currentPacket.length)    /* If packet length was received */
        || (currentPacket.size() >= m_sequence.m_maxLength) /* If maximum length was reached */
        || (IsEOFComplete() == true))                       /* If EOF was received */
    {
        /* Move the current packet at the end of the reception stack */
        m_rxBuffer.insert(m_rxBuffer.end(), std::move(currentPacket));

        /* Reset variables */
        m_currentPacket.data.clear();
        m_currentPacket.length = 0;
    }
}


/*************************************************************************************************/
/* Private member functions definitions -------------------------------------------------------- */
void UartModule::SendPacket(TxPacket& packet) noexcept
{
    /* Add the message to the queue */


    if (HAL_UART_Transmit(m_handle,
                          packet.data(),
                          packet.size() * sizeof(TxPacket::value_type),
                          TX_TIMEOUT) != HAL_OK)
    {
        m_status |= static_cast<Status>(m_handle->ErrorCode);
        CALL_ERROR_HANDLER();
    }
}

void UartModule::ErrorHandler(const std::string_view file,
                              const std::string_view func,
                              size_t                 line) noexcept
{
    if (CheckError(Status::NOT_ENABLED))
    {
        PRINT_ERROR("UART Module Not Enabled");
    }
    if (CheckError(Status::TIMEOUT))
    {
        PRINT_ERROR("UART Timeout");
    }
    if (CheckError(Status::BUSY))
    {
        PRINT_ERROR("UART Busy");
    }
    if (CheckError(Status::ERROR))
    {
        PRINT_ERROR("UART Error");
    }
    if (CheckError(Status::DROPPED_BYTE))
    {
        PRINT_ERROR("UART Dropped A Byte");
    }
    if (CheckError(Status::RX_BUFF_OVERFLOW))
    {
        PRINT_ERROR("UART Reception Buffer Overflow");
    }
    if (CheckError(Status::NOT_DONE_RECEIVING))
    {
        PRINT_ERROR("UART Not Done Receiving");
    }

    m_status = Status::OK;
}

GETTER uint8_t UartModule::ReadSingleByte()
{
    uint8_t newData = READ_DATA_REG(m_handle);
    __HAL_UART_FLUSH_DRREGISTER(m_handle);

    return newData;
}


/**
 * @brief   Check if the current packet's start of frame has been completely received.
 *
 * @retval  true: If the SOF was completely received
 *          false: If the SOF wasn't completely received
 */
bool UartModule::IsSOFComplete()
{
    auto& sof = m_sequence.m_startOfFrame;
    return std::equal(sof.begin(), sof.end(), m_currentPacket.data.begin());
}

/**
 * @brief   Check if the current packet's end of frame has been completely received.
 *
 * @retval  true: If the EOF was completely received
 *          false: If the EOF wasn't completely received
 */
bool UartModule::IsEOFComplete()
{
    auto& eof = m_sequence.m_endOfFrame;
    return std::equal(eof.begin(), eof.end(), m_currentPacket.data.end() - eof.size());
}


/**
 * @brief   Compare the incoming data with the SOF setting, and checks if it matches.
 *          If the data doesn't match, clear the packet and restart from zero.
 *
 * @param   uint8_t newData : Byte just read from the serial port
 *
 * @retval  COMPLETE if the SOF was completely received in the current packet
 *          NOT_COMPLETE otherwise
 */
[[nodiscard]] SectionState UartModule::HandleSOF(uint8_t newData)
{
    /* Check if the SOF was already completely read */
    if (!IsSOFComplete())
    {
        /* Check if the newly received data fits the SOF */
        if (newData == m_sequence.m_startOfFrame.at(m_currentPacket.data.size()))
        {
            /* Save the SOF byte */
            m_currentPacket.data.push_back(newData);
            return SectionState::NOT_COMPLETE;
        }
        else
        {
            /* There was an error, clear the packet */
            m_currentPacket.data.clear();
            m_currentPacket.length = 0;
            return SectionState::NOT_COMPLETE;
        }
    }

    return SectionState::COMPLETE;
}

/**
 * @brief   Get the incoming data and calculates the necessary packet length from it.
 *          The length data can be spread through multiple bytes (up to 8).
 *
 * @param   uint8_t newData  : Byte just read from the serial port
 *
 * @retval  COMPLETE:     If all the length bytes were received
 *          NOT_COMPLETE: Otherwise
 */
[[nodiscard]] SectionState UartModule::HandleLength(uint8_t newData)
{
    if (m_currentPacket.data.size() < m_sequence.m_startOfFrame.size() + m_sequence.m_lengthSize)
    {
        size_t n = m_currentPacket.data.size() - m_sequence.m_startOfFrame.size();
        m_currentPacket.length |= newData << (8 * n);

        m_currentPacket.data.push_back(newData);

        return SectionState::NOT_COMPLETE;
    }

    return SectionState::COMPLETE;
}


/*************************************************************************************************/
/* Callbacks ----------------------------------------------------------------------------------- */

#pragma region Callback functions

/* These callbacks are declared as __weak__ by the HAL, and must have C-style linkage in order to
 * override the HAL-provided functions. */
extern "C"
{
    /**
     * @brief  Automatically handles interruption reception on UART modules
     * @param  huart: Pointer to the handler to receive from
     * @retval None
     *
     * @note   This function should be added in the `UARTx_IRQHandler()` functions in
     *         `stm32f4xx_it.c` file.
     */
    void uart_HandleReceptionIRQ(UART_HandleTypeDef* huart)
    {
        UartModule* module = FindModuleFromHandle(huart);
        CEP_ASSERT_NULL(module);

        /* Verify that the interruption was caused by a reception */
        if (IS_IRQ_SRC_DATA_READY(huart) == false)
        {
            /* It wasn't, let the HAL handle whatever it was, then return */
            HAL_UART_IRQHandler(huart);
            return;
        }

        /* Handle the incoming message */
        module->HandleMessageReception();
    }
}
#pragma endregion


/*************************************************************************************************/
/* Private functions definitions --------------------------------------------------------------- */
inline static UartModule* FindModuleFromHandle(UART_HandleTypeDef* huart) noexcept
{
    CEP_ASSERT_NULL(huart);

    /* Check if the module is uart1 */
    CHECK_HANDLE_IS_MODULE(huart, UART1_MODULE);

    /* Check if the module is uart2 */
    CHECK_HANDLE_IS_MODULE(huart, UART2_MODULE);

    /* Check if the module is uart3 */
    CHECK_HANDLE_IS_MODULE(huart, UART3_MODULE);

    /* Check if the module is uart4 */
    CHECK_HANDLE_IS_MODULE(huart, UART4_MODULE);

    /* Module not found, return nullptr */
    return nullptr;
}


/*************************************************************************************************/
}; /* namespace UART */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
