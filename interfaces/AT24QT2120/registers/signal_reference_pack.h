/**
 * @file    signal_reference_pack.h
 * @author  Samuel Martel
 * @date    2022-08-23
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */

#ifndef GUARD_AT24QT2120_REGISTERS_SIGNAL_REFERENCE_PACK_H
#define GUARD_AT24QT2120_REGISTERS_SIGNAL_REFERENCE_PACK_H

#if defined(NILAI_USE_AT24QT2120)

#    include "../../../defines/misc.h"
#    include "key.h"

#    include <array>
#    include <vector>

namespace Nilai::Interfaces::AT24QT2120
{
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup AT24QT2120
 * @{
 */

/**
 * @struct SignalReferencePack
 * @brief Collection of all the signal levels and reference levels of the keys.
 */
class SignalReferencePack
{
public:
    //! A pair of Signal/Reference.
    struct Pack
    {
        uint8_t  Threshold = 0;
        uint8_t  Options   = 0;
        uint8_t  Pulse     = 0;
        uint8_t  Scale     = 0;
        uint16_t Signal    = 0;
        uint16_t Reference = 0;
    };
    using Packs = std::array<Pack, static_cast<size_t>(Keys::Count)>;

    /**
     * Total number of bytes making the signal-reference pack.
     * Each pack contains 7 bytes:
     * - 1 for the threshold
     * - 1 for the options
     * - 1 combining the pulse and the scale
     * - 2 for the signal
     * - 2 for the reference
     */
    static constexpr size_t s_dataLen = static_cast<size_t>(Keys::Count) * 7;

    constexpr SignalReferencePack() noexcept = default;
    constexpr SignalReferencePack(const std::vector<uint8_t>& data) noexcept
    {
        NILAI_ASSERT(data.size() == s_dataLen, "Bad data");
        constexpr const size_t s_threshStartIdx = 0;
        constexpr const size_t s_optionsStartIdx =
          s_threshStartIdx + (static_cast<size_t>(Keys::Count) * sizeof(decltype(Pack::Threshold)));
        constexpr const size_t s_pulseScaleStartIdx =
          s_optionsStartIdx + (static_cast<size_t>(Keys::Count) * sizeof(decltype(Pack::Options)));
        constexpr const size_t s_signalStartIdx =
          s_pulseScaleStartIdx + (static_cast<size_t>(Keys::Count) * sizeof(decltype(Pack::Pulse)));
        constexpr const size_t s_referenceStartIdx =
          s_signalStartIdx + (static_cast<size_t>(Keys::Count) * sizeof(decltype(Pack::Signal)));

        for (size_t i = 0; i < static_cast<size_t>(Keys::Count); i++)
        {
            size_t offset  = i * 2;
            m_signalRef[i] = {data[s_threshStartIdx + i],
                              data[s_optionsStartIdx + i],
                              static_cast<uint8_t>(data[s_pulseScaleStartIdx + i] >> 4),
                              static_cast<uint8_t>(data[s_pulseScaleStartIdx + i] & 0x0F),
                              Nilai::Pack<uint16_t>(data[s_signalStartIdx + offset],
                                                    data[s_signalStartIdx + offset + 1]),
                              Nilai::Pack<uint16_t>(data[s_referenceStartIdx + offset],
                                                    data[s_referenceStartIdx + offset + 1])};
        }
    }

    [[nodiscard]] Pack& operator[](Keys k) noexcept { return m_signalRef[static_cast<size_t>(k)]; }
    [[nodiscard]] const Pack& operator[](Keys k) const noexcept
    {
        return m_signalRef[static_cast<size_t>(k)];
    }
    [[nodiscard]] Pack&       operator[](size_t k) noexcept { return m_signalRef[k]; }
    [[nodiscard]] const Pack& operator[](size_t k) const noexcept { return m_signalRef[k]; }

    [[nodiscard]] Packs::iterator       begin() noexcept { return m_signalRef.begin(); }
    [[nodiscard]] Packs::const_iterator cbegin() const noexcept { return m_signalRef.cbegin(); }
    [[nodiscard]] Packs::iterator       end() noexcept { return m_signalRef.end(); }
    [[nodiscard]] Packs::const_iterator cend() const noexcept { return m_signalRef.cend(); }

private:
    Packs m_signalRef = {};
};

//!@}
//!@}
//!@}
}    // namespace Nilai::Interfaces::AT24QT2120
#endif
#endif    // GUARD_AT24QT2120_REGISTERS_SIGNAL_REFERENCE_PACK_H
