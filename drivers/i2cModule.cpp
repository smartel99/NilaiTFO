/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  I2C
 * @{
 * @file        i2cModule.cpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/cep20
 * @date        2020/08/24  -  17:00
 *
 * @brief       i2c communication module
 */
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#include "shared/drivers/i2cModule.hpp"
#include "shared/processes/application.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <vector>

namespace I2C {

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
#define TX_TIMEOUT 500_SysTicks

#define IS_IRQ_SRC_DATA_READY(handler) ((handler)->Instance->SR1 & I2C_FLAG_RXNE)
#define READ_DATA_REG(handler) ((handler)->Instance->DR & (uint8_t)0xFF)

/*************************************************************************************************/
/* Private function declarations
 * --------------------------------------------------------------- */
inline static I2cModule*
FindModuleFromHandle(I2C_HandleTypeDef *huart) noexcept;

/*************************************************************************************************/
/* Instance creation
 * ---------------------------------------------------------------------------
 */
#pragma region Instance creation and management

static std::vector<size_t> s_modules{};

ALWAYS_INLINE I2cModule *I2cModule::GetInstance(std::size_t moduleIndex) 
{
  I2cModule* i2cInstance = 
      dynamic_cast<I2cModule*>(Application::GetModule(s_modules[moduleIndex]));
  if (i2cInstance->IsEnabled()) 
  {
    return i2cInstance;
  } 
  else 
  {
    return nullptr;
  }
}
ALWAYS_INLINE void I2cModule::SetInstance(size_t instanceIndex) {
  s_modules.push_back(instanceIndex);
  m_moduleIndex = s_modules.size();
}
ALWAYS_INLINE size_t I2cModule::RemoveInstance(size_t moduleIndex) {
  /* Remove module index from `s_modules` vector using the erase-remove idiom */
  s_modules.erase(std::remove(s_modules.begin(), s_modules.end(), moduleIndex),
                  s_modules.end());

  return s_modules.size();
}

#pragma endregion

/*************************************************************************************************/
/* Handler
 * -------------------------------------------------------------------------------------
 */
void I2cModule::TransmissionHandler() noexcept {
  /* Clear timeout time if the transmission buffer is empty */
  if (m_txBuffer.empty()) {
    return;
  }

  SendPacket(m_txBuffer.back());
  m_txBuffer.pop_back();
}

/*************************************************************************************************/
/* Public member functions definitions
 * --------------------------------------------------------- */
void I2cModule::HandleMessageReception() {
  /* Read the received byte and flush the data register */
  uint8_t newData = ReadSingleByte();

  /* Handle packet length */
  if (m_sequence.m_useLength == true) {
    if (HandleLength(newData) == SectionState::NOT_COMPLETE) {
      return;
    }
  }

  /* Save the newly received data */
  RxPacket &currentPacket = m_currentPacket.data;
  currentPacket.push_back(newData);

  /* Check if the packet has ended */
  if ((currentPacket.size() >=
       m_currentPacket.length) /* If packet length was received */
      || (currentPacket.size() >=
          m_sequence.m_maxLength))  /* If maximum length was reached */
  {
    /* Move the current packet at the end of the reception stack */
    m_rxBuffer.insert(m_rxBuffer.end(), std::move(currentPacket));

    /* Reset variables */
    m_currentPacket.data.clear();
    m_currentPacket.length = 0;
  }
}

/*************************************************************************************************/
/* Private member functions definitions
 * -------------------------------------------------------- */
void I2cModule::SendPacket(TxPacket &packet) noexcept {
  /* Add the message to the queue */

  //if (HAL_I2C_Master_Transmit_IT(m_handle, 
 //                           packet.data(),
  //                          packet.data(),
  //                          packet.size() * sizeof(TxPacket::value_type)))
//{
//    m_status |= static_cast<Status>(m_handle->ErrorCode);
//    CALL_ERROR_HANDLER();
//  }
}

void I2cModule::ErrorHandler(const std::string_view file,
                              const std::string_view func,
                              size_t line) noexcept {
  if (CheckError(Status::NOT_ENABLED)) {
    PRINT_ERROR("I2C Module Not Enabled");
  }
  if (CheckError(Status::TIMEOUT)) {
    PRINT_ERROR("I2C Timeout");
  }
  if (CheckError(Status::BUSY)) {
    PRINT_ERROR("I2C Busy");
  }
  if (CheckError(Status::ERROR)) {
    PRINT_ERROR("I2C Error");
  }
  if (CheckError(Status::DROPPED_BYTE)) {
    PRINT_ERROR("I2C Dropped A Byte");
  }
  if (CheckError(Status::RX_BUFF_OVERFLOW)) {
    PRINT_ERROR("I2C Reception Buffer Overflow");
  }
  if (CheckError(Status::NOT_DONE_RECEIVING)) {
    PRINT_ERROR("I2C Not Done Receiving");
  }

  m_status = Status::OK;
}

GETTER uint8_t I2cModule::ReadSingleByte() {
  uint8_t newData = READ_DATA_REG(m_handle);
  __HAL_UART_FLUSH_DRREGISTER(m_handle);

  return newData;
}

/*************************************************************************************************/
/* Callbacks
 * -----------------------------------------------------------------------------------
 */

#pragma region Callback functions

/* These callbacks are declared as __weak__ by the HAL, and must have C-style
 * linkage in order to override the HAL-provided functions. */
extern "C" {
/**
 * @brief  Automatically handles interruption reception on I2C modules
 * @param  hi2c: Pointer to the handler to receive from
 * @retval None
 *
 * @note   This function should be added in the `HAL_I2C_EV_IRQHandler()` functions
 * in `stm32f4xx_it.c` file.
 */
void i2c_HandleReceptionIRQ(I2C_HandleTypeDef *hi2c1) {
  I2cModule *module = FindModuleFromHandle(hi2c1);
  CEP_ASSERT_NULL(module);

  /* Verify that the interruption was caused by a reception */
  if (IS_IRQ_SRC_DATA_READY(hi2c1) == false) 
  {
    /* It wasn't, let the HAL handle whatever it was, then return */
    HAL_I2C_EV_IRQHandler(hi2c1);
    return;
  }

  /* Handle the incoming message */
  module->HandleMessageReception();
}
}
#pragma endregion

/*************************************************************************************************/
/* Private functions definitions
 * --------------------------------------------------------------- */
inline static I2cModule *
FindModuleFromHandle(I2C_HandleTypeDef *hi2c1) noexcept {
  CEP_ASSERT_NULL(hi2c1);

  /* Check if the module is I2C */
  CHECK_HANDLE_IS_MODULE(hi2c1, I2C_MODULE);

  /* Module not found, return nullptr */
  return nullptr;
}

/*************************************************************************************************/
}; /* namespace I2C */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
