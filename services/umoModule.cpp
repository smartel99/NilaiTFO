/**
 ******************************************************************************
 * @addtogroup umoModule
 * @{
 * @file    umoModule
 * @author  Samuel Martel
 * @brief   Sources for the umoModule module.
 *
 * @date 9/17/2020 8:37:46 AM
 *
 ******************************************************************************
 */
#include "umoModule.h"
#if defined(NILAI_USE_UMO) && defined(NILAI_USE_UART)
#    include "defines/macros.hpp"

UmoModule::UmoModule(UartModule* uart, size_t universeCnt, const std::string& label)
    : m_uart(uart), m_label(label), m_universes(universeCnt)
{
    // Ensure that the pointer is valid.
    CEP_ASSERT(uart != nullptr, "In UmoModule: uart is NULL!");

    /* Configure the UART module:
     * - No SOF
     * - No EOF
     * - 515 bytes (universeID + 512 channels + CRC)
     * - No end of frame received callback
     */
    m_uart->ClearStartOfFrameSequence( );
    m_uart->ClearEndOfFrameSequence( );
    m_uart->SetExpectedRxLen(1 + Universe::CHANNEL_COUNT + 2);
    m_uart->ClearFrameReceiveCpltCallback( );

    LOG_INFO("[UMO]: Initialized");
}

void UmoModule::Run( )
{
    // Check if we should be sending an Universe to the PC.
    int u = 0;
    for (auto& universe : m_universes)
    {
        // If he Universe has been received during the last frame:
        if (universe.isNew)
        {
            // #TODO Compute CRC
            uint16_t crc = 0;
            // It's time to answer to the PC.
            universe.isNew = false;
            LOG_INFO("[UMO] Sending Universe %i", u);
            m_uart->Transmit(std::vector<uint8_t>{(uint8_t)u});
            m_uart->Transmit(universe.universe);
            m_uart->Transmit(std::vector<uint8_t>{(uint8_t)(crc >> 8), (uint8_t)(crc & 0x00FF)});
        }
        u++;
    }

    // Check if we have received an Universe.
    if (m_uart->GetNumberOfWaitingFrames( ) > 0)
    {
        UART::Frame frame = m_uart->Receive( );
        // Make sure the Universe is valid. (Valid ID + CRC)
        if (frame.data[0] < m_universes.size( ) &&
            frame.data.size( ) == (1 + Universe::CHANNEL_COUNT + 2))
        {
            // #TODO Check CRC.
            // Copy Universe into module if it is valid.
            for (size_t i = 0; i < Universe::CHANNEL_COUNT; i++)
            {
                m_universes[frame.data[0]].universe[i] = frame.data[1 + i];
            }
            m_universes[frame.data[0]].isNew = true;
            LOG_INFO("[UMO] Received new Universe with ID %i", frame.data[0]);
        }
    }
}

const std::vector<uint8_t>& UmoModule::GetUniverse(size_t universe) const
{
    CEP_ASSERT(universe < m_universes.size( ),
               "In {}.GetUniverse, invalid universe! (Was {}, should be inferior to {})",
               m_label,
               universe,
               m_universes.size( ));
    return m_universes[universe].universe;
}

bool UmoModule::IsUniverseReady(size_t universe) const
{
    CEP_ASSERT(universe < m_universes.size( ),
               "In {}.IsUniverseReady, universe is out of range. (is {}, should be under {}",
               m_label,
               universe,
               m_universes.size( ));

    return m_universes[universe].isNew;
}

std::vector<uint8_t> UmoModule::GetChannels(size_t universe, size_t channel, size_t size)
{
    [[maybe_unused]] volatile size_t c = channel;
    // Make sure that all params are good.
    CEP_ASSERT(
        universe < m_universes.size( ),
        "In {}.GetChannels, invalid universe requested (universe is {}, must be inferior to {}).",
        m_label,
        universe,
        m_universes.size( ));
    CEP_ASSERT(
        channel < Universe::CHANNEL_COUNT,
        "In {}.GetChannels, invalid channel requested (channel is {}, must be inferior to {}).",
        m_label,
        channel,
        Universe::CHANNEL_COUNT);
    CEP_ASSERT((channel + size) < Universe::CHANNEL_COUNT,
               "In {}.GetChannels, requested number of channels would exceed the range of the "
               "Universe! (channel is {}, size is {}, sum must be inferior to {})",
               m_label,
               channel,
               size,
               Universe::CHANNEL_COUNT);

    // Init a vector for the output data.
    std::vector<uint8_t> data = std::vector<uint8_t>(size);

    // Copy requested data into the vector.
    for (size_t i = 0; i < size; i++)
    {
        data[i] = m_universes[universe].universe[channel + i];
    }

    return data;
}

void UmoModule::GetChannels(size_t universe, size_t channel, uint8_t* outData, size_t size)
{
    // Make sure that all params are good.
    CEP_ASSERT(
        universe < m_universes.size( ),
        "In {}.GetChannels, invalid universe requested (universe is {}, must be inferior to {}).",
        m_label,
        universe,
        m_universes.size( ));
    CEP_ASSERT(
        channel < Universe::CHANNEL_COUNT,
        "In {}.GetChannels, invalid channel requested (channel is {}, must be inferior to {}).",
        m_label,
        channel,
        Universe::CHANNEL_COUNT);
    CEP_ASSERT((channel + size) < Universe::CHANNEL_COUNT,
               "In {}.GetChannels, requested number of channels would exceed the range of the "
               "Universe! (channel is {}, size is {}, sum must be inferior to {})",
               m_label,
               channel,
               size,
               Universe::CHANNEL_COUNT);
    CEP_ASSERT(outData != nullptr, "In {}.GetChannels, outData is NULL", m_label);

    // Copy requested data the buffer.
    for (size_t i = 0; i < size; i++)
    {
        outData[i] = m_universes[universe].universe[channel + i];
    }
}

void UmoModule::SetChannels(size_t universe, size_t channel, const std::vector<uint8_t>& data)
{
    // data.data() is a const uint8_t*, we must remove the const from it.
    SetChannels(universe, channel, const_cast<uint8_t*>(data.data( )), data.size( ));
}

void UmoModule::SetChannels(size_t universe, size_t channel, uint8_t* data, size_t len)
{
    // Make sure that all parameters are valid.
    CEP_ASSERT(universe < m_universes.size( ),
               "In {}.SetChannels, universe is out of range (is {}, must be under {}).",
               m_label,
               universe,
               m_universes.size( ));
    CEP_ASSERT(channel < Universe::CHANNEL_COUNT,
               "In {}.SetChannels, channel is out of range (is {}, must be under {}).",
               m_label,
               channel,
               Universe::CHANNEL_COUNT);
    CEP_ASSERT((channel + len) < Universe::CHANNEL_COUNT,
               "In {}.SetChannels, requested data would be out of range ({} channels requested "
               "starting at channel {}. Channel + len must be under {})",
               m_label,
               channel + len,
               channel,
               Universe::CHANNEL_COUNT);

    // Copy the data into the universe.
    for (size_t i = 0; i < len; i++)
    {
        m_universes[universe].universe[channel + i] = data[i];
    }
}
#endif
