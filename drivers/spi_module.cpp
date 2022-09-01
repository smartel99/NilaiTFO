/**
 * @file        spi_module.cpp
 * @author      Lou-Gabriel Gaucher
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 * @date        2020/08/13  -  09:40
 *
 * @brief       SPI communication module
 */
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#include "spi_module.h"
#if defined(NILAI_USE_SPI) && defined(HAL_SPI_MODULE_ENABLED)
#    include "../processes/application.h"

#    include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
#    define SPI_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)
#    define SPI_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str() __VA_OPT__(, ) __VA_ARGS__)

namespace Nilai::Drivers
{
/*************************************************************************************************/
/* Public function definitions
 * --------------------------------------------------------------- */
SpiModule::SpiModule(SPI_HandleTypeDef* handle, std::string_view label) noexcept
: m_label(label), m_handle(handle)
{
    NILAI_ASSERT(handle != nullptr, "Handle is NULL!");
    SPI_INFO("Initialized");
}

SpiModule::~SpiModule() noexcept
{
    /* Abort ongoing messages on SPI peripheral */
    if (HAL_SPI_Abort_IT(m_handle) != HAL_OK)
    {
        m_status |= static_cast<SPI::Status>(m_handle->ErrorCode);
        ErrorHandler();
    }

    /* De-init peripheral */
    if (HAL_SPI_DeInit(m_handle) != HAL_OK)
    {
        m_status |= static_cast<SPI::Status>(m_handle->ErrorCode);
        ErrorHandler();
    }
}

bool SpiModule::DoPost() noexcept
{
    SPI_INFO("POST OK");
    return true;
}

SPI::Status SpiModule::Transmit(const std::vector<uint8_t>& pkt) noexcept
{
    return Transmit(pkt.data(), pkt.size());
}

SPI::Status SpiModule::Transmit(const uint8_t* data, size_t len) noexcept
{
    NILAI_ASSERT(data != nullptr, "Ptr is null");
    NILAI_ASSERT(len != 0, "len is 0");

    // Wait for peripheral to be free.
    if (!WaitUntilNotBusy())
    {
        return SPI::Status::TIMEOUT;
    }

    if (HAL_SPI_Transmit(
          m_handle, const_cast<uint8_t*>(data), static_cast<uint16_t>(len), SpiModule::s_timeout) !=
        HAL_OK)
    {
        m_status |= static_cast<SPI::Status>(m_handle->ErrorCode);
        ErrorHandler();
        return m_status;
    }

    return SPI::Status::NONE;
}

SPI::Status SpiModule::Receive(uint8_t* ouptutData, size_t len) noexcept
{
    NILAI_ASSERT(ouptutData != nullptr, "Ptr is null");
    NILAI_ASSERT(len > 0, "len is 0");

    // Wait for SPI peripheral to be ready.
    if (!WaitUntilNotBusy())
    {
        return SPI::Status::TIMEOUT;
    }

    if (HAL_SPI_Receive(m_handle, ouptutData, static_cast<uint16_t>(len), SpiModule::s_timeout) !=
        HAL_OK)
    {
        m_status |= static_cast<SPI::Status>(m_handle->ErrorCode);
        ErrorHandler();
        return m_status;
    }

    return SPI::Status::NONE;
}

SPI::Status SpiModule::Transaction(const uint8_t*          txData,
                                   size_t                  txLen,
                                   uint8_t*                rxData,
                                   [[maybe_unused]] size_t rxLen) noexcept
{
    NILAI_ASSERT(txData != nullptr, "TxData is NULL");
    NILAI_ASSERT(rxData != nullptr, "RxData is NULL");
    NILAI_ASSERT(txLen > 0, "TxLen is 0");
    NILAI_ASSERT(txLen == rxLen, "TxLen != RxLen");

    // Wait for SPI peripheral to be ready.
    if (!WaitUntilNotBusy())
    {
        return SPI::Status::TIMEOUT;
    }

    if (HAL_SPI_TransmitReceive(m_handle,
                                const_cast<uint8_t*>(txData),
                                rxData,
                                static_cast<uint16_t>(txLen),
                                SpiModule::s_timeout) != HAL_OK)
    {
        m_status |= static_cast<SPI::Status>(m_handle->ErrorCode);
        ErrorHandler();
        return m_status;
    }

    return SPI::Status::NONE;
}

SPI::Status SpiModule::Transaction(const std::vector<uint8_t>& txData,
                                   std::vector<uint8_t>&       rxData) noexcept
{
    // Make sure we got enough space in rxData.
    rxData.resize(txData.size());

    return Transaction(txData.data(), txData.size(), rxData.data(), rxData.size());
}

/*************************************************************************************************/
/* Private functions definitions
 * --------------------------------------------------------------- */
void SpiModule::ErrorHandler() noexcept
{
    // TODO: Not this.
    if ((m_status & SPI::Status::MODF) != SPI::Status::NONE)
    {
        SPI_ERROR("MODF");
    }
    if ((m_status & SPI::Status::CRC_ERROR) != SPI::Status::NONE)
    {
        SPI_ERROR("CRC");
    }
    if ((m_status & SPI::Status::OVR) != SPI::Status::NONE)
    {
        SPI_ERROR("OVR");
    }
    if ((m_status & SPI::Status::DMA) != SPI::Status::NONE)
    {
        SPI_ERROR("FRE");
    }
    if ((m_status & SPI::Status::FLAG) != SPI::Status::NONE)
    {
        SPI_ERROR("FLAG");
    }
    if ((m_status & SPI::Status::ABORT) != SPI::Status::NONE)
    {
        SPI_ERROR("ABORT state");
    }

    if ((m_status & SPI::Status::NOT_INIT) != SPI::Status::NONE)
    {
        SPI_ERROR("Not initialized");
    }
    if ((m_status & SPI::Status::TIMEOUT) != SPI::Status::NONE)
    {
        SPI_ERROR("Timeout");
    }

    m_status = SPI::Status::NONE;
}

bool SpiModule::WaitUntilNotBusy() noexcept
{
    // Timeout time = 200 fast ticks.
    uint32_t timeoutTime = GetTime() + s_timeout;

    // While the SPI port is busy and the timeout wasn't hit:
    while (GetTime() <= timeoutTime)
    {
        // Check if the SPI port is ready.
        if (m_handle->State == HAL_SPI_STATE_READY)
        {
            return true;
        }
    }

    return false;
}
}    // namespace Nilai::Drivers
#endif
/* ----- END OF FILE ----- */
