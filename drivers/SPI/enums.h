/**
 * @file    Enums.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_SPI_ENUMS_H
#define NILAI_SPI_ENUMS_H

#if defined(NILAI_USE_SPI)
#    if defined(NILAI_TEST)
#    else
#        include "../../defines/internal_config.h"
#        include NILAI_HAL_HEADER

#        include <type_traits>

namespace Nilai::SPI
{
/**
 * @addtogroup  SPI_Status
 * @brief       SPI module status, mostly describing error states.
 */
enum class Status
{
    /*!< No error                         */
    NONE = 0x00000000U,
    /*!< MODF error                       */
    MODF = 0x00000001U,
    /*!< CRC error                        */
    CRC_ERROR = 0x00000002U,
    /*!< OVR error                        */
    OVR = 0x00000004U,
    /*!< FRE error                        */
    FRE = 0x00000008U,
    /*!< DMA transfer error               */
    DMA = 0x00000010U,
    /*!< Error on RXNE/TXE/BSY Flag       */
    FLAG = 0x00000020U,
    /*!< Error during SPI Abort procedure */
    ABORT = 0x00000040U,
#        if USE_HAL_SPI_REGISTER_CALLBACKS == 1
    /*!< Invalid Callback error  */
    SPI_ERROR_INVALID_CALLBACK = 0x00000080U,
#        endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    /*!< Module is not enabled            */
    NOT_INIT = 0x00000100U,
    /*!< Bad initialization               */
    BAD_INIT = 0x00000200U,
    /*!< Module has timed out             */
    TIMEOUT = 0x00000400U,
};

enum class Polarity
{
    /*!< Clock is low by default
     *   and active high >*/
    LOW = SPI_POLARITY_LOW,
    /*!< Clock is high by default
     *   and active low >*/
    HIGH = SPI_POLARITY_HIGH
};

enum class Phase
{
    /*!< Sampling is done on clock
     *  rising edge */
    EDGE1 = SPI_PHASE_1EDGE,
    /*!< Sampling is done on clock
     *   falling edge >*/
    EDGE2 = SPI_PHASE_2EDGE
};

/** From: https://stackoverflow.com/a/15889501 */
constexpr inline Status operator|(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) |
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator&(Status a, Status b) noexcept
{
    return static_cast<Status>(static_cast<std::underlying_type_t<Status>>(a) &
                               static_cast<std::underlying_type_t<Status>>(b));
}
constexpr inline Status operator|=(Status& a, const Status& b) noexcept
{
    return a = a | b;
}
/**
 * @}
 */

enum class SectionState
{
    NOT_COMPLETE,
    COMPLETE,
};
}    // namespace Nilai::SPI
#    endif
#endif

#endif    // NILAI_SPI_ENUMS_H
