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
#include "umo_module.h"
#if defined(NILAI_USE_UMO) && (defined(NILAI_USE_UART) || defined(NILAI_USE_CAN))
#    include "defines/macros.hpp"

#    if defined(NILAI_UMO_USE_CAN)
#        error Not implemented
#    endif

UmoModule::UmoModule(Handle_t* handle, size_t universeCnt, const std::string& label)
: m_handle(handle), m_label(label), m_universes(universeCnt)
{
    // Ensure that the pointer is valid.
    CEP_ASSERT(handle != nullptr, "In UmoModule: handle is NULL!");

    m_universes.resize(universeCnt);

#    if defined(NILAI_UMO_USE_UART)
    /* Configure the UART module:
     * - No SOF or EOF
     * - 515 bytes (universeID + 512 channels + CRC)
     * - No end of frame received callback
     */
    m_handle->SetExpectedRxLen(1 + Universe::CHANNEL_COUNT + 2);
    m_handle->ClearStartOfFrameSequence();
    m_handle->ClearEndOfFrameSequence();
    m_handle->ClearFrameReceiveCpltCallback();
#    elif defined(NILAI_UMO_USE_CAN)

#    endif

    LOG_INFO("[UMO]: Initialized");
}

bool UmoModule::DoPost()
{
    // Having 0 universes is not OK.
    if (m_universes.size() == 0)
    {
        LOG_ERROR("[UMO]: No universes!");
        return false;
    }

    // Set all channels to 0. They should already be that way, but we never know.
    for (auto& universe : m_universes)
    {
        for (auto& ch : universe.universe)
        {
            ch = 0;
        }
    }

    return true;
}

void UmoModule::Run()
{
    // Check if we should be sending an Universe to the PC.
    for (size_t i = 0; i < m_universes.size(); i++)
    {
        if (m_universes[i].age == -1)
        {
            // We haven't received this universe yet.
            continue;
        }
        // If the Universe is old enough to die:
        if (m_universes[i].age++ > OLDEST_AGE)
        {
            // #TODO Compute CRC
            uint16_t crc = 0;
            // It's time to answer to the PC.
            m_universes[i].age = -1;
            LOG_INFO("[UMO] Sending Universe %i", i);
#    if defined(NILAI_UMO_USE_UART)
            m_handle->Transmit(std::vector<uint8_t> {(uint8_t)i});
            m_handle->Transmit(m_universes[i].universe);
            m_handle->Transmit(std::vector<uint8_t> {(uint8_t)(crc >> 8), (uint8_t)(crc & 0x00FF)});
#    elif defined(NILAI_UMO_USE_CAN)

#    endif
        }
    }

#    if defined(NILAI_UMO_USE_UART)
    // Check if we have received an Universe.
    if (m_handle->GetNumberOfWaitingFrames() > 0)
    {
        CEP_UART::Frame frame = m_handle->Receive();
        // Make sure the Universe is valid. (Valid ID + CRC)
        if (frame.data[0] < m_universes.size( ) /*&&
            frame.data.size( ) == (1 + Universe::CHANNEL_COUNT + 2)*/)
        {
            // #TODO Check CRC.
            // Copy Universe into module if it is valid.
            for (size_t i = 0; i < Universe::CHANNEL_COUNT; i++)
            {
                m_universes[frame.data[0]].universe[i] = frame.data[1 + i];
            }
            m_universes[frame.data[0]].age = 0;    // Mark the universe as newly born.
            LOG_INFO("[UMO] Received new Universe with ID %i", frame.data[0]);
        }
    }
#    elif defined(NILAI_UMO_USE_CAN)

#    endif
}

const std::vector<uint8_t>& UmoModule::GetUniverse(size_t universe) const
{
    CEP_ASSERT(universe < m_universes.size(),
               "In {}.GetUniverse, invalid universe! (Was {}, should be inferior to {})",
               m_label,
               universe,
               m_universes.size());
    return m_universes[universe].universe;
}

bool UmoModule::IsUniverseReady(size_t universe) const
{
    CEP_ASSERT(universe < m_universes.size(),
               "In {}.IsUniverseReady, universe is out of range. (is {}, should be under {}",
               m_label,
               universe,
               m_universes.size());

    return (m_universes[universe].age == 0);
}

std::vector<uint8_t> UmoModule::GetChannels(size_t universe, size_t channel, size_t size)
{
    [[maybe_unused]] volatile size_t c = channel;
    // Make sure that all params are good.
    CEP_ASSERT(universe < m_universes.size(),
               "In {}.GetChannels, invalid universe requested (universe is {}, must be inferior to "
               "{}).",
               m_label,
               universe,
               m_universes.size());
    CEP_ASSERT(channel < Universe::CHANNEL_COUNT,
               "In {}.GetChannels, invalid channel requested (channel is {}, must be inferior to "
               "{}).",
               m_label,
               channel,
               Universe::CHANNEL_COUNT);
    CEP_ASSERT((channel + size) <= Universe::CHANNEL_COUNT,
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
    CEP_ASSERT(universe < m_universes.size(),
               "In {}.GetChannels, invalid universe requested (universe is {}, must be inferior to "
               "{}).",
               m_label,
               universe,
               m_universes.size());
    CEP_ASSERT(channel < Universe::CHANNEL_COUNT,
               "In {}.GetChannels, invalid channel requested (channel is {}, must be inferior to "
               "{}).",
               m_label,
               channel,
               Universe::CHANNEL_COUNT);
    CEP_ASSERT((channel + size) <= Universe::CHANNEL_COUNT,
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
    SetChannels(universe, channel, const_cast<uint8_t*>(data.data()), data.size());
}

void UmoModule::SetChannels(size_t universe, size_t channel, uint8_t* data, size_t len)
{
    // Make sure that all parameters are valid.
    CEP_ASSERT(universe < m_universes.size(),
               "In {}.SetChannels, universe is out of range (is {}, must be under {}).",
               m_label,
               universe,
               m_universes.size());
    CEP_ASSERT(channel < Universe::CHANNEL_COUNT,
               "In {}.SetChannels, channel is out of range (is {}, must be under {}).",
               m_label,
               channel,
               Universe::CHANNEL_COUNT);
    CEP_ASSERT((channel + len) <= Universe::CHANNEL_COUNT,
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
