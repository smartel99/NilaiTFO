/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  SPI
 * @{
 * @file        spiModule.cpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/13  -  09:40
 *
 * @brief       SPI communication module
 */
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#include "shared/drivers/spiModule.hpp"
#include "shared/processes/application.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <vector>

namespace SPI {

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
#define SPI_TX_TIMEOUT 500_SysTicks

#define IS_IRQ_SRC_DATA_READY(handler) ((handler)->Instance->SR & SPI_IT_RXNE)
#define READ_DATA_REG(handler) ((handler)->Instance->DR & (uint8_t)0xFF)

/*************************************************************************************************/
/* Private function declarations
 * --------------------------------------------------------------- */
inline static SpiModule *
FindModuleFromHandle(SPI_HandleTypeDef *hspi) noexcept;

/*************************************************************************************************/
/* Instance creation
 * ---------------------------------------------------------------------------
 */
#pragma region Instance creation and management

static std::vector<size_t> s_modules{};

ALWAYS_INLINE SpiModule *SpiModule::GetInstance(std::size_t moduleIndex) {
  SpiModule *spiInstance =
      dynamic_cast<SpiModule *>(
      Application::GetModule(s_modules[moduleIndex]));
  if (spiInstance->IsEnabled()) {
    return spiInstance;
  } else {
    return nullptr;
  }
}
ALWAYS_INLINE void SpiModule::SetInstance(size_t instanceIndex) {
  s_modules.push_back(instanceIndex);
  m_moduleIndex = s_modules.size();
}
ALWAYS_INLINE size_t SpiModule::RemoveInstance(size_t moduleIndex) {
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
void SpiModule::TransmissionHandler() noexcept {
  /* Clear timeout time if the transmission buffer is empty */
  if (m_txBuffer.empty()) {
    return;
  }

  Transmit(m_txBuffer.back());
  m_txBuffer.pop_back();
}

/*************************************************************************************************/
/* Public member functions definitions
 * --------------------------------------------------------- */
void SpiModule::HandleMessageReception() {
  /* Read the received byte and flush the data register */
  uint8_t newData = spi_Receive();

  /* Handle Start Of Frame */
  if (m_sequence.m_useSof == true) {
    if (HandleSOF(newData) == SectionState::NOT_COMPLETE) {
      m_currentPacket.length = 0;
      return;
    }
  }

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
          m_sequence.m_maxLength)   /* If maximum length was reached */
      || (IsEOFComplete() == true)) /* If EOF was received */
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
void SpiModule::Transmit(TxPacket &packet) noexcept {
  /* Add the message to the queue */

  if (HAL_SPI_Transmit(m_handle, packet.data(),
                        packet.size() * sizeof(TxPacket::value_type),
                        TX_TIMEOUT) != HAL_OK) {
    m_status |= static_cast<Status>(m_handle->ErrorCode);
    CALL_ERROR_HANDLER();
  }
}

void spi_Transaction(size_t length,
                     const uint8_t txData[], uint8_t rxData[])
    {

/* Compute timeout time */
FastTick_t timeoutTime = FASTTICK.Start() + SPI_TX_TIMEOUT;
/* Check if peripheral is busy */
while ( == Status::SPI_ERROR_FLAG &&
       FASTTICK.ShouldTimeout(timeoutTime) == false) {
  /* Do nothing here, wait */
}
FASTTICK.Stop();
  /* Check if peripheral is busy */
  if (waitUntilNotBusy(module, SPI_TIMEOUT) == false) {
    /* Timed out */
    return SPI_ERROR_TIMEOUT;
  }
//
//  module->newDataFlag = false; /* Data hasn't been received yet */
//
//  /* Start transmission */
//  if (HAL_SPI_TransmitReceive(module->hspi, (uint8_t *)txData, rxData, length,
//                              SPI_TIMEOUT) != HAL_OK) {
//    module->status |= module->hspi->ErrorCode;
//    Error_Handler();
//    return module->status;
//  }
//  return SPI_ERROR_NONE;
}
void spi_TransactionByte(SpiModule *module, uint8_t txData,
                         uint8_t *rxData)
    {
  //return spi_Transaction(module, 1, &txData, rxData);
  }
  void spi_Transaction16(SpiModule *module, uint16_t txData,
                         uint16_t *rxData) {
    //return spi_Transaction(module, 2, (uint8_t *)&txData, (uint8_t *)rxData);
    }

  void SpiModule::ErrorHandler(const std::string_view file,
                               const std::string_view func,
                               size_t line) noexcept {
    if (CheckError(Status::SPI_ERROR_NOT_INIT)) {
      PRINT_ERROR("SPI Module Not Enabled");
    }
    if (CheckError(Status::SPI_ERROR_TIMEOUT)) {
      PRINT_ERROR("SPI Timeout");
    }
    if (CheckError(Status::SPI_ERROR_FLAG)) {
      PRINT_ERROR("SPI RXNE/TXE/BSY flag");
    }
    if (CheckError(Status::SPI_ERROR_MODF)) {
      PRINT_ERROR("SPI MODF error!");
    }
    if (CheckError(Status::SPI_ERROR_CRC)) {
      PRINT_ERROR("SPI Bad CRC");
    }
    if (CheckError(Status::SPI_ERROR_OVR)) {
      PRINT_ERROR("SPI OVR error!");
    }
    if (CheckError(Status::SPI_ERROR_FRE)) {
      PRINT_ERROR("SPI FRE error!");
    }
    if (CheckError(Status::SPI_ERROR_DMA)) {
      PRINT_ERROR("SPI DMA transfer error!");
    }
    if (CheckError(Status::SPI_ERROR_ABORT)) {
      PRINT_ERROR("SPI error during abort procedure!");
    }
    m_status = Status::SPI_ERROR_NONE;
}

GETTER uint8_t spi_Receive() {

}
GETTER uint8_t spi_ReceiveByte() {

}
GETTER uint8_t spi_Receive16() {

}

}

    /**
     * @brief   Get the incoming data and calculates the necessary packet length
     * from it. The length data can be spread through multiple bytes (up to 8).
     *
     * @param   uint8_t newData  : Byte just read from the serial port
     *
     * @retval  COMPLETE:     If all the length bytes were received
     *          NOT_COMPLETE: Otherwise
     */
    [[nodiscard]] SectionState SpiModule::HandleLength(uint8_t newData) {
  if (m_currentPacket.data.size() <
      m_sequence.m_startOfFrame.size() + m_sequence.m_lengthSize) {
    size_t n = m_currentPacket.data.size() - m_sequence.m_startOfFrame.size();
    m_currentPacket.length |= newData << (8 * n);

    m_currentPacket.data.push_back(newData);

    return SectionState::NOT_COMPLETE;
  }

  return SectionState::COMPLETE;
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
 * @brief  Automatically handles interruption reception on UART modules
 * @param  huart: Pointer to the handler to receive from
 * @retval None
 *
 * @note   This function should be added in the `UARTx_IRQHandler()` functions
 * in `stm32f4xx_it.c` file.
 */
void spi_HandleReceptionIRQ(SPI_HandleTypeDef *hspi) {
  SpiModule *module = FindModuleFromHandle(hspi);
  CEP_ASSERT_NULL(module);

  /* Verify that the interruption was caused by a reception */
  if (IS_IRQ_SRC_DATA_READY(hspi) == false) {
    /* It wasn't, let the HAL handle whatever it was, then return */
    HAL_SPI_IRQHandler(hspi);
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

/*************************************************************************************************/
}; /* namespace UART */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
