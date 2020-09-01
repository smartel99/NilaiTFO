/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  SPI
 * @{
 * @file        spiModule.hpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/13  -  09:25
 *
 * @brief       SPI communication module
 */
#pragma once
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#include "shared/defines/pch.hpp"
#include "shared/defines/driverModule.hpp"
#include "shared/defines/misc.hpp"
#include "shared/services/ticks.hpp"

#ifdef HAL_UART_MODULE_ENABLED
#include "Core/Inc/spi.h"
#endif

#include <array>
#include <functional>
#include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
#define SPI1_MODULE (SPI::SpiModule::GetInstance(0))

namespace SPI {

/*************************************************************************************************/
/* Enumerated Types
 * ----------------------------------------------------------------------------
 */

#pragma region Status
/**
 * @addtogroup  SPI_Status
 * @brief       SPI module status, mostly describing error states.
 */
enum class Status {
  SPI_ERROR_NONE = 0x00000000U,  /*!< No error                         */
  SPI_ERROR_MODF = 0x00000001U,  /*!< MODF error                       */
  SPI_ERROR_CRC = 0x00000002U,   /*!< CRC error                        */
  SPI_ERROR_OVR = 0x00000004U,   /*!< OVR error                        */
  SPI_ERROR_FRE = 0x00000008U,   /*!< FRE error                        */
  SPI_ERROR_DMA = 0x00000010U,   /*!< DMA transfer error               */
  SPI_ERROR_FLAG = 0x00000020U,  /*!< Error on RXNE/TXE/BSY Flag       */
  SPI_ERROR_ABORT = 0x00000040U, /*!< Error during SPI Abort procedure */
#if USE_HAL_SPI_REGISTER_CALLBACKS == 1
  SPI_ERROR_INVALID_CALLBACK = 0x00000080U, /*!< Invalid Callback error  */
#endif                                      /* USE_HAL_SPI_REGISTER_CALLBACKS */
  SPI_ERROR_NOT_INIT = 0x00000100U, /*!< Module is not enabled            */
  SPI_ERROR_BAD_INIT = 0x00000200U, /*!< Bad initialization               */
  SPI_ERROR_TIMEOUT = 0x00000400U,  /*!< Module has timed out             */
};

enum class Polarity {
  SPI_CLOCK_POLARITY_LOW = SPI_POLARITY_LOW,  /*!< Clock is low by default
                                               *   and active high >*/
  SPI_CLOCK_POLARITY_HIGH = SPI_POLARITY_HIGH /*!< Clock is high by default
                                               *   and active low >*/
};

enum class Phase {
  SPI_CLOCK_PHASE_1EDGE = SPI_PHASE_1EDGE, /*!< Sampling is done on clock
                                            *   rising edge >*/
  SPI_CLOCK_PHASE_2EDGE = SPI_PHASE_2EDGE  /*!< Sampling is done on clock
                                            *   falling edge >*/
  };
    /** From: https://stackoverflow.com/a/15889501 */
    [[nodiscard]] constexpr inline Status
    operator|(Status a, Status b) noexcept {
  return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                             static_cast<std::underlying_type_t<Status>>(b));
}
[[nodiscard]] constexpr inline Status operator&(Status a, Status b) noexcept {
  return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                             static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status &a, const Status &b) noexcept {
  return a = a | b;
}
/**
 * @}
 */
#pragma endregion

enum class SectionState {
  NOT_COMPLETE,
  COMPLETE,
};

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
class SpiModule;

class Sequence_t {
  friend class SpiModule;
  using DataType = uint8_t;

private:
  bool m_useSof = false;
  std::vector<DataType> m_startOfFrame{};

  bool m_useEof = false;
  std::vector<DataType> m_endOfFrame{};

  bool m_useLength = false;
  size_t m_lengthSize = 0;

  size_t m_maxLength = 127;

public:
  Sequence_t() = default;

  ALWAYS_INLINE void
  SetSOF(std::initializer_list<DataType> startOfFrameSequence) {
    m_startOfFrame = startOfFrameSequence;
    m_useSof = true;
  }
  ALWAYS_INLINE void
  SetEOF(std::initializer_list<DataType> endOfFrameSequence) {
    m_endOfFrame = endOfFrameSequence;
    m_useEof = true;
  }

  ALWAYS_INLINE void SetLengthSize(size_t packetLengthSize) {
    CEP_ASSERT(packetLengthSize <= 8, "Packet length must fit in a uint64_t");
    m_lengthSize = packetLengthSize;
    m_useLength = true;
  }

  ALWAYS_INLINE void SetMaxLength(size_t maxLength) { m_maxLength = maxLength; }
};

using DriverModuleType =
    cep::DriverModule<SPI_HandleTypeDef, Status, std::vector<uint8_t>>;
class SpiModule : public DriverModuleType {
public:
  using RxPacket = typename DriverModuleType::RxPacket_t;
  using TxPacket = typename DriverModuleType::TxPacket_t;
  using Callback_t = typename DriverModuleType::Callback_t;

  /*********************************************************************************************/
  /* Private member variables
   * ---------------------------------------------------------------- */
private:
  Sequence_t m_sequence;

  struct CurrentPacket_t {
    RxPacket data;
    uint64_t length;
  } m_currentPacket;

public:
  SpiModule(SPI_HandleTypeDef *handle, const std::string_view name)
      : DriverModuleType{handle, name} {
    /* MX_SPIx_UART_Init must be called in `application.cpp` */

    m_status = Status::SPI_ERROR_NONE;
  }
  ~SpiModule() {
    HAL_SPI_Abort(m_handle);
    HAL_SPI_DeInit(m_handle);
  }

  /*********************************************************************************************/
  /* Public member functions declarations
   * ---------------------------------------------------- */
  void HandleMessageReception();

  /*********************************************************************************************/
  /* Handlers
   * --------------------------------------------------------------------------------
   */
private:
  ALWAYS_INLINE void TransmissionHandler() noexcept override;

  /*********************************************************************************************/
  /* Accessors
   * -------------------------------------------------------------------------------
   */

#pragma region Accessors
public:
  GETTER static SpiModule *GetInstance(size_t moduleIndex = 0);
  GETTER Sequence_t &Sequence() { return m_sequence; }

private:
  ALWAYS_INLINE void SetInstance(size_t instanceIndex) override;
  ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override;
#pragma endregion

  /*********************************************************************************************/
  /* Private member function declarations
   * ---------------------------------------------------- */

  // Finish the functions! Not usable as it is!
  void Transmit(TxPacket &packet) noexcept;
  void TransmitByte(TxPacket &packet) noexcept;
  void Transmit16(TxPacket &packet) noexcept;

  void spi_Transaction(size_t length,
                                  const uint8_t txData[],
                                  uint8_t rxData[]);
  void spi_TransactionByte(SpiModule *module,
                                                    uint8_t txData,
                                                    uint8_t *rxData);
  void spi_Transaction16(SpiModule *module,
                                                  uint16_t txData,
                                                  uint16_t *rxData);
  
  void ErrorHandler(const std::string_view file, const std::string_view func,
                    size_t line) noexcept override;

  // Finish the functions!
  GETTER uint8_t spi_Receive() noexcept;
  GETTER uint8_t spi_ReceiveByte() noexcept;
  GETTER uint8_t spi_Receive16() noexcept;
  [[nodiscard]] SectionState HandleLength(uint8_t newData);
};

/*************************************************************************************************/
}; // namespace SPI
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
