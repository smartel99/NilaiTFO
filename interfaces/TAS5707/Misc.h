/**
 * @file    MiscStructs.h
 * @author  Samuel Martel
 * @date    2022-02-08
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_TAS5707_MISCSTRUCTS_H
#define NILAI_TAS5707_MISCSTRUCTS_H

#if defined(NILAI_USE_TAS5707)
#    include "Enums.h"
#    include <array>
#    include <cstdint>

namespace cep::Tas5707
{
/**
 * @brief Structure representing a biquad filter under the 3.23 floating point number
 * representation.
 *
 * You can generate these coefficients using <a href=https://www.ti.com/tool/TAS5707EVM>TI's
 * tool</a>
 */
struct BiquadFilter
{
    uint32_t b0 = 0x00800000;
    uint32_t b1 = 0x00000000;
    uint32_t b2 = 0x00000000;
    uint32_t a1 = 0x00000000;
    uint32_t a2 = 0x00000000;

    [[nodiscard]] std::array<uint8_t, 20> Serialize() const;

    bool operator==(const BiquadFilter& o) const;
};

/**
 * @brief Structure containing the biquad filters for each banks.
 *
 * Each one of the three banks have 7 biquad filters that can be individually and independently
 * configured.
 *
 * @note If automatic bank selection is not enabled, only bank 1 should be used.
 *
 * TODO Make BiquadBanks more space efficient, especially when banks are not used.
 * This struct takes a minimum of 420 bytes (5 uint32 per filter, 7 filters per banks, 3 banks,
 * 5 * 4 * 7 * 3 = 420)
 */
struct BiquadBanks
{
    std::array<BiquadFilter, 7> Bank1 = {};
    std::array<BiquadFilter, 7> Bank2 = {};
    std::array<BiquadFilter, 7> Bank3 = {};
};

struct DynRangeCtrlCoeffs
{
    uint32_t a  = 0x00800000;    //! Alpha factor
    uint32_t oa = 0x00000000;    //! 1 - Alpha factor

    [[nodiscard]] std::array<uint8_t, 8> Serialize() const;
};

struct DynamicRangeControl
{
    //! Toggles the Dynamic Range Control (DRC).
    uint32_t State = DrcModes::Disabled;

    //! Alpha of energy filter for Dynamic Range Control.
    DynRangeCtrlCoeffs Ae = {};
    //! Alpha of attack filter for Dynamic Range Control.
    DynRangeCtrlCoeffs Aa = {};
    //! Alpha of decay filter for Dynamic Range Control.
    DynRangeCtrlCoeffs Ad = {};

    //! DRC-T coefficient, using the 9.23 floating point number format.
    uint32_t T = 0xFDA21490;
    //! DRC-K coefficient, using the 3.23 floating point number format.
    uint32_t K = 0x03842109;
    //! DRC-O coefficient, using the 3.23 floating point number format.
    uint32_t O = 0x00084210;
};

}    // namespace cep::Tas5707
#endif
#endif    // NILAI_TAS5707_MISCSTRUCTS_H
