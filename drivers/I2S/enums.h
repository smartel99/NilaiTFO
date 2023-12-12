/**
 * @file    enums.h
 * @author  Samuel Martel
 * @date    2022-05-02
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
#ifndef NILAI_I2S_ENUMS_H
#define NILAI_I2S_ENUMS_H

#if defined(NILAI_USE_I2S)
#    if defined(NILAI_TEST)
#    else
#        include "Core/Inc/i2s.h"

#        include <cstdint>

namespace Nilai::I2S
{
enum class AudioFreqs
{
    f192k   = I2S_AUDIOFREQ_192K,
    f96k    = I2S_AUDIOFREQ_96K,
    f48k    = I2S_AUDIOFREQ_48K,
    f44k1   = I2S_AUDIOFREQ_44K,
    f32k    = I2S_AUDIOFREQ_32K,
    f22k05  = I2S_AUDIOFREQ_22K,
    f16k    = I2S_AUDIOFREQ_16K,
    f11k025 = I2S_AUDIOFREQ_11K,
    f8k     = I2S_AUDIOFREQ_8K,
};

constexpr AudioFreqs ToAudioFreq(std::uint32_t freq)
{
    if (freq <= static_cast<uint32_t>(AudioFreqs::f8k))
    {
        return AudioFreqs::f8k;
    }
    if (freq >= static_cast<uint32_t>(AudioFreqs::f192k))
    {
        return AudioFreqs::f192k;
    }
    return static_cast<AudioFreqs>(freq);
}
}    // namespace Nilai::I2S

#    endif

#endif
#endif    // NILAI_I2S_ENUMS_H
