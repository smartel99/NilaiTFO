/**
 * @file        spi_module.h
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
#        if defined(NILAI_TEST)
#        else
#            include "../defines/internal_config.h"
#            include NILAI_HAL_HEADER
#            if defined(HAL_SPI_MODULE_ENABLED)
#                include "../defines/macros.h"
#                include "../defines/misc.h"
#                include "../defines/module.h"

#                include "SPI/enums.h"

#                include <string>
#                include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
namespace Nilai::Drivers
{
class SpiModule : public Module
{
public:
    SpiModule(SPI_HandleTypeDef* handle, std::string label);
    ~SpiModule() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    SPI::Status Transmit(const std::vector<uint8_t>& pkt);
    SPI::Status Transmit(const uint8_t* data, size_t len);

    inline SPI::Status TransmitByte(uint8_t data) { return Transmit(&data, 1); }
    inline SPI::Status Transmit16(uint16_t data)
    {
        // Send the data in little-endian mode, for backward compatibility.
        // This method is slower than straight up reinterpret_cast, but it is
        // consistent across all implementation.
        return Transmit({(uint8_t)(data & 0x00FF), (uint8_t)(data >> 8)});
    }

    SPI::Status Receive(uint8_t* ouptutData, size_t len);
    SPI::Status Receive(std::vector<uint8_t>& outputData)
    {
        return Receive(outputData.data(), outputData.size());
    }
    inline SPI::Status ReceiveByte(uint8_t* outputData) { return Receive(outputData, 1); }
    inline SPI::Status Receive16(uint16_t* outputData)
    {
        return Receive(reinterpret_cast<uint8_t*>(outputData), 2);
    }

    SPI::Status Transaction(const uint8_t* txData, size_t txLen, uint8_t* rxData, size_t rxLen);
    SPI::Status Transaction(const std::vector<uint8_t>& txData, std::vector<uint8_t>& rxData);

    inline SPI::Status TransactionByte(uint8_t txData, uint8_t* rxData)
    {
        return Transaction(&txData, 1, rxData, 1);
    }
    inline SPI::Status Transaction16(uint16_t txData, uint16_t* rxData)
    {
        return Transaction(
          reinterpret_cast<uint8_t*>(&txData), 2, reinterpret_cast<uint8_t*>(rxData), 2);
    }

private:
    std::string        m_label;
    SPI_HandleTypeDef* m_handle;
    SPI::Status        m_status = SPI::Status::NONE;

    constexpr static uint16_t TIMEOUT = 200;

private:
    void ErrorHandler();
    bool WaitUntilNotBusy();
};
}    // namespace Nilai::Drivers
#            else
#                if WARN_MISSING_STM_DRIVERS
#                    warning NilaiTFO SPI module enabled, but HAL_SPI_USE_MODULE is not defined!
#                endif
#            endif
#        endif
#    endif
#endif

/* ----- END OF FILE ----- */
