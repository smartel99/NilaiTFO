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


#include <array>
#include <functional>
#include <vector>

namespace UART
{


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define TX_TIMEOUT 500_SysTicks

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
