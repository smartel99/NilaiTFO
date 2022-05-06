/**
 * @file    Structs.h
 * @author  Samuel Martel
 * @date    2022-04-19
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
#ifndef NILAI_CAN_STRUCTS_H
#define NILAI_CAN_STRUCTS_H

#if defined(NILAI_USE_CAN)

#    include <array>
#    include <cstddef>
#    include <cstdint>

#    ifdef NILAI_TEST
// In test mode, define these if we're not including the HAL's headers.
#        ifndef CAN_FILTERMODE_IDMASK
#            define CAN_FILTERMODE_IDMASK 0
#        endif
#        ifndef CAN_FILTERMODE_IDLIST
#            define CAN_FILTERMODE_IDLIST 1
#        endif
#        ifndef CAN_FILTERSCALE_16BIT
#            define CAN_FILTERSCALE_16BIT 0
#        endif
#        ifndef CAN_FILTERSCALE_32BIT
#            define CAN_FILTERSCALE_32BIT 1
#        endif
#        ifndef CAN_FILTER_DISABLE
#            define CAN_FILTER_DISABLE 0
#        endif
#        ifndef CAN_FILTER_ENABLE
#            define CAN_FILTER_ENABLE 1
#        endif
#        ifndef CAN_FILTER_FIFO0
#            define CAN_FILTER_FIFO0 0
#        endif
#        ifndef CAN_FILTER_FIFO1
#            define CAN_FILTER_FIFO1 1
#        endif
#        ifndef CAN_ID_STD
#            define CAN_ID_STD 0
#        endif
#        ifndef CAN_ID_EXT
#            define CAN_ID_EXT 1
#        endif
#        ifndef CAN_RTR_DATA
#            define CAN_RTR_DATA 0
#        endif
#        ifndef CAN_RTR_REMOTE
#            define CAN_RTR_REMOTE 1
#        endif
#        ifndef CAN_RX_FIFO0
#            define CAN_RX_FIFO0 0
#        endif
#        ifndef CAN_RX_FIFO1
#            define CAN_RX_FIFO1 1
#        endif

struct CAN_RxHeaderTypeDef
{
    uint32_t StdId            = 0;
    uint32_t ExtId            = 0;
    uint32_t IDE              = 0;
    uint32_t RTR              = 0;
    uint32_t DLC              = 0;
    uint32_t Timestamp        = 0;
    uint32_t FilterMatchIndex = 0;
};
#    endif
namespace Nilai::Can
{
enum class FilterMode
{
    //!< Identifier mask mode.
    IdMask = CAN_FILTERMODE_IDMASK,
    //!< Identifier list mode.
    IdList = CAN_FILTERMODE_IDLIST,
};

enum class FilterScale
{
    //!< Two 16-bit filters.
    Scale16bit = CAN_FILTERSCALE_16BIT,
    //!< One 32-bit filter.
    Scale32bit = CAN_FILTERSCALE_32BIT,
};

enum class FilterEnable
{
    Disable = CAN_FILTER_DISABLE,
    Enable  = CAN_FILTER_ENABLE,
};

enum class FilterFifoAssignation
{
    //!< Filter x is assigned to FIFO 0.
    Fifo0 = CAN_FILTER_FIFO0,
    //!< Filter x is assigned to FIFO 1.
    Fifo1 = CAN_FILTER_FIFO1,
};

enum class IdentifierType
{
    Standard = CAN_ID_STD,
    Extended = CAN_ID_EXT,
};

enum class FrameType
{
    Data   = CAN_RTR_DATA,
    Remote = CAN_RTR_REMOTE,
};

enum class RxFifo
{
    //!< CAN Receive FIFO0.
    Fifo0 = CAN_RX_FIFO0,
    //!< CAN Receive FIFO1.
    Fifo1 = CAN_RX_FIFO1,
};

/**
 * @brief CAN filter configuration
 */
struct FilterConfiguration
{
    union
    {
        struct
        {
            /** Specifies the filter identification number (lower 16 bits for
             *  a 32-bit filter configuration, or the second filter in a dual
             *  16-bit configuration)
             */
            uint16_t idLow;
            /** Secifies the filter identification number (higher 16 bits for a
             ** 32-bit filter configuration, or the first filter in a dual
             ** 16-bit configuration)
             */
            uint16_t idHigh;
        };
        union
        {
            struct
            {
                uint8_t : 1;
                uint8_t  rtr : 1;
                uint8_t  ide : 1;
                uint32_t extId : 18;
                uint16_t stdId : 11;
            };
            uint32_t fullId;
        } canId;
        uint32_t fullId = 0x00000000;
    } filterId;

    union
    {
        struct
        {
            /** Depending on the FilterMode, specifies either the filter's mask
             ** in mask mode or the filter's ID in list mode.
             ** (Lower 16 bits for a 32-bit filter configuration, or the
             ** second filter in a dual 16-bit configuration.
             **/
            uint16_t maskIdLow;
            /** Depending on the FilterMode, specifies either the filter's mask
             ** in mask mode or the filter's ID in list mode.
             ** (Higher 16 bits for a 32-bit filter configuration, or the
             ** first filter in a dual 16-bit configuration)
             **/
            uint16_t maskIdHigh;
        };
        union
        {
            struct
            {
                uint8_t : 1;
                uint8_t  rtr : 1;
                uint8_t  ide : 1;
                uint32_t extId : 18;
                uint16_t stdId : 11;
            };
            uint32_t fullId;
        } canId;
        uint32_t fullId = 0x00000000;
    } maskId;

    //! Specifies the FIFO which will be assigned to the filter.
    FilterFifoAssignation fifo = FilterFifoAssignation::Fifo0;
    //! Specifies the filter bank which will be initialized.
    //! This parameter must be a number between 0 and 13.
    uint8_t bank = 0;
    //! Specifies the filter mode to be initialized.
    FilterMode mode = FilterMode::IdMask;
    //! Specifies the filter scale.
    FilterScale scale = FilterScale::Scale32bit;
    //! Enable or disable the filter.
    FilterEnable activate = FilterEnable::Enable;

    bool operator==(const FilterConfiguration& o) const
    {
        return filterId.fullId == o.filterId.fullId && maskId.fullId == o.maskId.fullId &&
               fifo == o.fifo && bank == o.bank && mode == o.mode && scale == o.scale &&
               activate == o.activate;
    }
};

struct Frame
{
    CAN_RxHeaderTypeDef    frame {};
    std::array<uint8_t, 8> data {};
    uint32_t               timestamp = 0;

    bool operator==(const Frame& other)
    {
        // If both have the same extended ID:
        if (frame.ExtId == other.frame.ExtId)
        {
            // For each byte of data:
            for (size_t i = 0; i < data.size(); i++)
            {
                // If the two bytes are not the same:
                if (data[i] != other.data[i])
                {
                    // Frames are different.
                    return false;
                }
            }
            // Everything is identical, true.
            return true;
        }
        // ID don't match
        return false;
    }

    bool operator!=(const Frame& other) { return !(*this == other); }
};
}    // namespace Nilai::Can
#endif
#endif    // NILAI_CAN_STRUCTS_H
