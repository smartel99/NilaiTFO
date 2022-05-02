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
#ifndef NILAI_SPI_MODULE_HPP_
#    define NILAI_SPI_MODULE_HPP_
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

#            include "SPI/Enums.h"

#            if defined(NILAI_USE_EVENTS)
#                include "../defines/Events/Events.h"
#            endif

#            include <string>
#            include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
class SpiModule : public cep::Module
{
public:
    SpiModule(SPI_HandleTypeDef* handle, std::string  label);
    ~SpiModule() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

#            if defined(NILAI_USE_EVENTS)
    bool OnEvent(cep::Events::Event* event) override;
#            endif

    cep::SPI::Status Transmit(const std::vector<uint8_t>& pkt);
    cep::SPI::Status Transmit(const uint8_t* data, size_t len);

    inline cep::SPI::Status TransmitByte(uint8_t data) { return Transmit(&data, 1); }
    inline cep::SPI::Status Transmit16(uint16_t data)
    {
        // Send the data in little-endian mode, for backward compatibility.
        // This method is slower than straight up reinterpret_cast, but it is
        // consistent across all implementation.
        return Transmit({(uint8_t)(data & 0x00FF), (uint8_t)(data >> 8)});
    }

    cep::SPI::Status Receive(uint8_t* ouptutData, size_t len);
    cep::SPI::Status Receive(std::vector<uint8_t>& outputData)
    {
        return Receive(outputData.data(), outputData.size());
    }
    inline cep::SPI::Status ReceiveByte(uint8_t* outputData) { return Receive(outputData, 1); }
    inline cep::SPI::Status Receive16(uint16_t* outputData)
    {
        return Receive(reinterpret_cast<uint8_t*>(outputData), 2);
    }

    cep::SPI::Status Transaction(const uint8_t* txData,
                                 size_t         txLen,
                                 uint8_t*       rxData,
                                 size_t         rxLen);
    cep::SPI::Status Transaction(const std::vector<uint8_t>& txData, std::vector<uint8_t>& rxData);

    inline cep::SPI::Status TransactionByte(uint8_t txData, uint8_t* rxData)
    {
        return Transaction(&txData, 1, rxData, 1);
    }
    inline cep::SPI::Status Transaction16(uint16_t txData, uint16_t* rxData)
    {
        return Transaction(
          reinterpret_cast<uint8_t*>(&txData), 2, reinterpret_cast<uint8_t*>(rxData), 2);
    }

private:
    std::string        m_label;
    SPI_HandleTypeDef* m_handle;
    cep::SPI::Status   m_status = cep::SPI::Status::NONE;

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
