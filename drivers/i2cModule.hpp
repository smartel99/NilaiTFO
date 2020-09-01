/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  i2c
 * @{
 * @file        i2cModule.hpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/cep20
 * @date        2020/08/13  -  09:25
 *
 * @brief       I2C communication module
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

#ifdef HAL_I2C_MODULE_ENABLED
#include "Core/Inc/i2c.h"
#endif

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
#define I2C_MODULE (I2C::I2cModule::GetInstance(0))

namespace I2C {

/*************************************************************************************************/
/* Enumerated Types
 * ----------------------------------------------------------------------------
 */

#pragma region Status
/**
 * @addtogroup  I2C_Status
 * @brief       I2C module status, mostly describing error states.
 */
enum class Status {
  OK = 0x0000,                 //!< I2C_OK.
  NOT_ENABLED = 0x0001,        //!< I2C_NOT_ENABLED.
  TIMEOUT = 0x0002,            //!< I2C_TIMEOUT.
  BUSY = 0x0004,               //!< I2C_BUSY.
  ERROR = 0x0008,              //!< I2C_ERROR.
  DROPPED_BYTE = 0x0010,       //!< I2C_DROPPED_BYTE.
  RX_BUFF_OVERFLOW = 0x0020,   //!< I2C_RX_BUFF_OVERFLOW.
  NOT_DONE_RECEIVING = 0x0040, //!< I2C_NOT_DONE_RECEIVING.
};

/** From: https://stackoverflow.com/a/15889501 */
[[nodiscard]] constexpr inline Status operator|(Status a, Status b) noexcept {
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
class I2cModule;

class Sequence_t {
  friend class I2cModule;
  using DataType = uint8_t;

private:

  bool m_useLength = false;
  size_t m_lengthSize = 0;

  size_t m_maxLength = 127;

public:
  Sequence_t() = default;

  ALWAYS_INLINE void SetLengthSize(size_t packetLengthSize) {
    CEP_ASSERT(packetLengthSize <= 8, "Packet length must fit in a uint64_t");
    m_lengthSize = packetLengthSize;
    m_useLength = true;
  }

  ALWAYS_INLINE void SetMaxLength(size_t maxLength) { m_maxLength = maxLength; }
};

using DriverModuleType =
    cep::DriverModule<I2C_HandleTypeDef, Status, std::vector<uint8_t>>;
class I2cModule : public DriverModuleType {
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
  I2cModule(I2C_HandleTypeDef *handle, const std::string_view name)
      : DriverModuleType{handle, name} {
    /* MX_I2CX_Init must be called in `application.cpp` */

    m_status = Status::OK;
  }
  ~I2cModule() {
    //HAL_I2C_Abort(m_handle);
    HAL_I2C_DeInit(m_handle);
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
  GETTER static I2cModule *GetInstance(size_t moduleIndex = 0);
  GETTER Sequence_t &Sequence() { return m_sequence; }

private:
  ALWAYS_INLINE void SetInstance(size_t instanceIndex) override;
  ALWAYS_INLINE size_t RemoveInstance(size_t moduleIndex) override;
#pragma endregion

  /*********************************************************************************************/
  /* Private member function declarations
   * ---------------------------------------------------- */
  void SendPacket(TxPacket &packet) noexcept;
  void ErrorHandler(const std::string_view file, const std::string_view func,
                    size_t line) noexcept override;

  GETTER uint8_t ReadSingleByte();
  [[nodiscard]] SectionState HandleLength(uint8_t newData);
};

/*************************************************************************************************/
}; /* namespace I2C */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
