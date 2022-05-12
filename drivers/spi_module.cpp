/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  SPI
 * @{
 * @file        spiModule.cpp
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
#    include "../defines/macros.h"
#    include "../processes/application.h"
#    include "../services/logger.h"

#    include <algorithm>
#    include <array>
#    include <functional>
#    include <utility>
#    include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
#    define SPI_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define SPI_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

namespace Nilai::Drivers
{
/*************************************************************************************************/
/* Public function definitions
 * --------------------------------------------------------------- */
SpiModule::SpiModule(SPI_HandleTypeDef* handle, std::string label)
: m_label(std::move(label)), m_handle(handle)
{
    NILAI_ASSERT(handle != nullptr, "SPI Handle is NULL!");
    SPI_INFO("Initialized");
}

SpiModule::~SpiModule()
{
    /* Abort ongoing messages on SPI peripheral */
    if (HAL_SPI_Abort_IT(m_handle) != HAL_OK)
    {
        m_status |= (SPI::Status)m_handle->ErrorCode;
        ErrorHandler();
    }

    /* De-init peripheral */
    if (HAL_SPI_DeInit(m_handle) != HAL_OK)
    {
        m_status |= (SPI::Status)m_handle->ErrorCode;
        ErrorHandler();
    }
}

/**
 * If the initialization passed, the POST passes.
 * @return
 */
bool SpiModule::DoPost()
{
    SPI_INFO("POST OK");
    return true;
}

void SpiModule::Run()
{
}

SPI::Status SpiModule::Transmit(const std::vector<uint8_t>& pkt)
{
    return Transmit(pkt.data(), pkt.size());
}

SPI::Status SpiModule::Transmit(const uint8_t* data, size_t len)
{
    NILAI_ASSERT(data != nullptr, "SPI Data pointer in null in SpiModule::Transmit");
    NILAI_ASSERT(len != 0, "SPI Transmit data length is 0 in SpiModule::Transmit");

    // Wait for peripheral to be free.
    if (!WaitUntilNotBusy())
    {
        return SPI::Status::TIMEOUT;
    }

    if (HAL_SPI_Transmit(m_handle, const_cast<uint8_t*>(data), (uint16_t)len, SpiModule::TIMEOUT) !=
        HAL_OK)
    {
        m_status |= (SPI::Status)m_handle->ErrorCode;
        ErrorHandler();
        return m_status;
    }

    return SPI::Status::NONE;
}

SPI::Status SpiModule::Receive(uint8_t* ouptutData, size_t len)
{
    NILAI_ASSERT(ouptutData != nullptr, "Pointer is NULL in SpiModule::Receive");
    NILAI_ASSERT(len > 0, "Length is 0 in SpiModule::Receive");

    // Wait for SPI peripheral to be ready.
    if (!WaitUntilNotBusy())
    {
        return SPI::Status::TIMEOUT;
    }

    if (HAL_SPI_Receive(m_handle, ouptutData, (uint16_t)len, SpiModule::TIMEOUT) != HAL_OK)
    {
        m_status |= (SPI::Status)m_handle->ErrorCode;
        ErrorHandler();
        return m_status;
    }

    return SPI::Status::NONE;
}

SPI::Status SpiModule::Transaction(const uint8_t* txData,
                                   size_t         txLen,
                                   uint8_t*       rxData,
                                   size_t         rxLen)
{
    NILAI_ASSERT(txData != nullptr, "TxData is NULL in SpiModule::Transaction");
    NILAI_ASSERT(rxData != nullptr, "RxData is NULL in SpiModule::Transaction");
    NILAI_ASSERT(txLen > 0, "TxLen is 0 in SpiModule::Transaction");
    NILAI_ASSERT(txLen == rxLen, "TxLen is not equal to RxLen in SpiModule::Transaction");

    // Wait for SPI peripheral to be ready.
    if (!WaitUntilNotBusy())
    {
        return SPI::Status::TIMEOUT;
    }

    if (HAL_SPI_TransmitReceive(
          m_handle, const_cast<uint8_t*>(txData), rxData, (uint16_t)txLen, SpiModule::TIMEOUT) !=
        HAL_OK)
    {
        m_status |= (SPI::Status)m_handle->ErrorCode;
        ErrorHandler();
        return m_status;
    }

    return SPI::Status::NONE;
}

SPI::Status SpiModule::Transaction(const std::vector<uint8_t>& txData, std::vector<uint8_t>& rxData)
{
    // Make sure we got enough space in rxData.
    rxData.resize(txData.size());

    return Transaction(txData.data(), txData.size(), rxData.data(), rxData.size());
}

/*************************************************************************************************/
/* Private functions definitions
 * --------------------------------------------------------------- */
void SpiModule::ErrorHandler()
{
    // TODO: Not this.
    if ((m_status & SPI::Status::MODF) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI MODF error");
    }
    if ((m_status & SPI::Status::CRC_ERROR) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI CRC error");
    }
    if ((m_status & SPI::Status::OVR) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI OVR error");
    }
    if ((m_status & SPI::Status::DMA) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI FRE error");
    }
    if ((m_status & SPI::Status::FLAG) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI FLAG error");
    }
    if ((m_status & SPI::Status::ABORT) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI ABORT state");
    }

    if ((m_status & SPI::Status::NOT_INIT) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI module is not initialized");
    }
    if ((m_status & SPI::Status::TIMEOUT) != SPI::Status::NONE)
    {
        SPI_ERROR("SPI timeout error");
    }

    m_status = SPI::Status::NONE;
}

bool SpiModule::WaitUntilNotBusy()
{
    // Timeout time = 200 fast ticks.
    uint32_t timeoutTime = GetTime() + 200;

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
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
