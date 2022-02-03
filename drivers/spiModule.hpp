/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  SPI
 * @{
 * @file        spiModule.hpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/08/13  -  09:25
 *
 * @brief       SPI communication module
 */
#ifndef SPI_MODULE_HPP_
#    define SPI_MODULE_HPP_
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#    if defined(NILAI_USE_SPI)
#        include "../defines/internalConfig.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_SPI_MODULE_ENABLED)
#            include "../defines/macros.hpp"
#            include "../defines/misc.hpp"
#            include "../defines/module.hpp"

#            include <string>
#            include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

namespace CEP_SPI
{
/*************************************************************************************************/
/* Enumerated Types
 * ----------------------------------------------------------------------------
 */

/**
 * @addtogroup  SPI_Status
 * @brief       SPI module status, mostly describing error states.
 */
enum class Status
{
    /*!< No error                         */
    NONE = 0x00000000U,
    /*!< MODF error                       */
    MODF = 0x00000001U,
    /*!< CRC error                        */
    CRC_ERROR = 0x00000002U,
    /*!< OVR error                        */
    OVR = 0x00000004U,
    /*!< FRE error                        */
    FRE = 0x00000008U,
    /*!< DMA transfer error               */
    DMA = 0x00000010U,
    /*!< Error on RXNE/TXE/BSY Flag       */
    FLAG = 0x00000020U,
    /*!< Error during SPI Abort procedure */
    ABORT = 0x00000040U,
#            if USE_HAL_SPI_REGISTER_CALLBACKS == 1
    /*!< Invalid Callback error  */
    SPI_ERROR_INVALID_CALLBACK = 0x00000080U,
#            endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    /*!< Module is not enabled            */
    NOT_INIT = 0x00000100U,
    /*!< Bad initialization               */
    BAD_INIT = 0x00000200U,
    /*!< Module has timed out             */
    TIMEOUT = 0x00000400U,
};

enum class Polarity
{
    /*!< Clock is low by default
     *   and active high >*/
    LOW = SPI_POLARITY_LOW,
    /*!< Clock is high by default
     *   and active low >*/
    HIGH = SPI_POLARITY_HIGH
};

enum class Phase
{
    /*!< Sampling is done on clock
     *  rising edge */
    EDGE1 = SPI_PHASE_1EDGE,
    /*!< Sampling is done on clock
     *   falling edge >*/
    EDGE2 = SPI_PHASE_2EDGE
};

/** From: https://stackoverflow.com/a/15889501 */
constexpr inline Status operator|(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator&(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) noexcept
{
    return a = a | b;
}
/**
 * @}
 */

enum class SectionState
{
    NOT_COMPLETE,
    COMPLETE,
};
/*************************************************************************************************/
}    // namespace CEP_SPI

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
class SpiModule : public cep::Module
{
public:
    SpiModule(SPI_HandleTypeDef* handle, const std::string& label);
    virtual ~SpiModule() override;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

    CEP_SPI::Status Transmit(const std::vector<uint8_t> pkt);
    CEP_SPI::Status Transmit(const uint8_t* data, size_t len);

    inline CEP_SPI::Status TransmitByte(uint8_t data) { return Transmit(&data, 1); }
    inline CEP_SPI::Status Transmit16(uint16_t data)
    {
        // Send the data in little-endian mode, for backward compatibility.
        // This method is slower than straight up reinterpret_cast, but it is
        // consistent across all implementation.
        return Transmit({(uint8_t)(data & 0x00FF), (uint8_t)(data >> 8)});
    }

    CEP_SPI::Status Receive(uint8_t* ouptutData, size_t len);
    CEP_SPI::Status Receive(std::vector<uint8_t>& outputData)
    {
        return Receive(outputData.data(), outputData.size());
    }
    inline CEP_SPI::Status ReceiveByte(uint8_t* outputData) { return Receive(outputData, 1); }
    inline CEP_SPI::Status Receive16(uint16_t* outputData)
    {
        return Receive(reinterpret_cast<uint8_t*>(outputData), 2);
    }

    CEP_SPI::Status Transaction(const uint8_t* txData, size_t txLen, uint8_t* rxData, size_t rxLen);
    // #TODO Explain difference between const std::vector& and std::vector&
    CEP_SPI::Status Transaction(const std::vector<uint8_t>& txData, std::vector<uint8_t>& rxData);

    inline CEP_SPI::Status TransactionByte(uint8_t txData, uint8_t* rxData)
    {
        return Transaction(&txData, 1, rxData, 1);
    }
    inline CEP_SPI::Status Transaction16(uint16_t txData, uint16_t* rxData)
    {
        return Transaction(
          reinterpret_cast<uint8_t*>(&txData), 2, reinterpret_cast<uint8_t*>(rxData), 2);
    }

private:
    std::string        m_label;
    SPI_HandleTypeDef* m_handle;
    CEP_SPI::Status    m_status = CEP_SPI::Status::NONE;

    constexpr static uint16_t TIMEOUT = 200;

private:
    void ErrorHandler();
    bool WaitUntilNotBusy();
};
#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO SPI module enabled, but HAL_SPI_USE_MODULE is not defined!
#            endif
#        endif
#    endif
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
