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
#                include <string_view>
#                include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Drivers
 * @{
 */

/**
 * @addtogroup nilai_drivers_spi SPI
 * @{
 */

namespace Nilai::Drivers
{
/**
 * @class SpiModule
 * @brief Class abstracting the SPI peripheral.
 */
class SpiModule : public Module
{
public:
    /**
     * @brief Initializes the SPI module.
     * @param handle The handle of the hardware peripheral.
     * @param label Label assigned to the module.
     */
    SpiModule(SPI_HandleTypeDef* handle, std::string_view label) noexcept;
    /**
     * @brief De-initializes the SPI module, freeing resources.
     */
    ~SpiModule() noexcept override;

    /**
     * @brief Verifies that the SPI module was successfully initialized.
     * @returns True if the initialization was successful.
     * @returns False if the initialization was not successful.
     */
    bool DoPost() noexcept override;

    /**
     * @brief Gets the identifying label of the module.
     * @return The label.
     */
    [[nodiscard]] const std::string& GetLabel() const noexcept { return m_label; }

    /**
     * @brief Transmits a packet of data.
     * @param pkt The data to send.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Transmit(const std::vector<uint8_t>& pkt) noexcept;
    /**
     * @brief Transmits a packet of data.
     * @param data A pointer to the data to be sent.
     * @param len The number of bytes of data to send.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Transmit(const uint8_t* data, size_t len) noexcept;

    /**
     * @brief Transmits a single byte of data.
     * @param data The byte to send.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status TransmitByte(uint8_t data) noexcept { return Transmit(&data, 1); }

    /**
     * @brief Transmits 2 bytes of data in little-endian mode.
     * @param data The data to send.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Transmit16(uint16_t data) noexcept
    {
        return Transmit({static_cast<uint8_t>(data & 0x00FF), static_cast<uint8_t>(data >> 8)});
    }

    /**
     * @brief Receives data from the SPI bus.
     * @param outputData A pointer to the buffer where the data should be written to. It must
     * already be allocated.
     * @param len The number of bytes that needs to be received.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Receive(uint8_t* outputData, size_t len) noexcept;

    /**
     * @brief Receives data from the SPI bus.
     * @param outputData A reference to a vector in which to store the data. It must already have
     * the desired size.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Receive(std::vector<uint8_t>& outputData) noexcept
    {
        return Receive(outputData.data(), outputData.size());
    }

    /**
     * @brief Receives a byte of data from the SPI bus.
     * @param outputData A pointer in which the byte will be written.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status ReceiveByte(uint8_t* outputData) noexcept { return Receive(outputData, 1); }

    /**
     * @brief Receives two bytes of data from the SPI bus.
     * @param outputData A pointer in which the bytes will be written
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Receive16(uint16_t* outputData) noexcept
    {
        return Receive(reinterpret_cast<uint8_t*>(outputData), 2);
    }

    /**
     * @brief Exchanges (transmits and receives) data on the SPI bus.
     * @param txData A pointer to the data to be sent.
     * @param txLen The number of bytes to be sent.
     * @param rxData A pointer in which the received data will be written.
     * @param rxLen The number of bytes to receive.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     *
     * @attention `txLen` must be equal to `rxLen`, and the data pointers must not be null.
     */
    SPI::Status Transaction(const uint8_t* txData,
                            size_t         txLen,
                            uint8_t*       rxData,
                            size_t         rxLen) noexcept;

    /**
     * @brief Exchanges (transmits and receives) data on the SPI bus.
     * @param txData A reference to the data to be sent.
     * @param rxData A reference to the destination of the received data.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     *
     * @attention `txData` and `rxData` must be of the same size.
     */
    SPI::Status Transaction(const std::vector<uint8_t>& txData,
                            std::vector<uint8_t>&       rxData) noexcept;

    /**
     * @brief Exchanges (transmits and receives) a byte of data on the SPI bus.
     * @param txData The byte to send.
     * @param rxData Pointer where the received byte will be written.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status TransactionByte(uint8_t txData, uint8_t* rxData) noexcept
    {
        return Transaction(&txData, 1, rxData, 1);
    }

    /**
     * @brief Exchanges (transmits and receives) two bytes of data on the SPI bus.
     * @param txData The data to send.
     * @param rxData Pointer where the received data will be written.
     * @returns `SPI::Status::NONE` if the transmission was successful.
     * @returns The error code if the transmission failed.
     */
    SPI::Status Transaction16(uint16_t txData, uint16_t* rxData) noexcept
    {
        return Transaction(
          reinterpret_cast<uint8_t*>(&txData), 2, reinterpret_cast<uint8_t*>(rxData), 2);
    }

private:
    std::string        m_label;                         //!< The identifying label of the module.
    SPI_HandleTypeDef* m_handle = nullptr;              //!< Pointer to the hardware peripheral.
    SPI::Status        m_status = SPI::Status::NONE;    //!< Last known status of the module.

    //! Maximum amount of time that will be waited for the SPI peripheral to become ready.
    constexpr static uint16_t s_timeout = 200;

private:
    void ErrorHandler() noexcept;
    bool WaitUntilNotBusy() noexcept;
};
}    // namespace Nilai::Drivers

//!@}
//!@}
//!@}
#            else
#                if WARN_MISSING_STM_DRIVERS
#                    warning NilaiTFO SPI module enabled, but HAL_SPI_USE_MODULE is not defined!
#                endif
#            endif
#        endif
#    endif
#endif

/* ----- END OF FILE ----- */
