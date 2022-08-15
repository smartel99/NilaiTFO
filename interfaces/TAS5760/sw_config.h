/**
 * @file    sw_config.h
 * @author  Samuel Martel
 * @date    2022-03-03
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
#ifndef NILAI_TAS5760_SWCONFIG_H
#define NILAI_TAS5760_SWCONFIG_H

#if defined(NILAI_USE_SW_TAS5760)
#    if defined(NILAI_TEST)
#    else
#        include "../../defines/pin.h"
#        include "../../defines/smart_pointers.h"
#        include "../../drivers/i2c_module.h"
#        include "enums.h"
#        include "registers/registers.h"

#        if NILAI_TAS5760_PWM_MODE > NILAI_TAS5760_PWM_NONE
#            include "../../drivers/pwmModule.h"
#        endif

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup TAS5760
 * @{
 */

namespace Nilai::Interfaces::TAS5760
{
/**
 * @struct SwConfig
 * @brief Structure describing the configuration of the TAS5760 used in software control mode.
 */
struct SwConfig
{
    //! I2C handle to talk with the TAS5760.
    Ref<Drivers::I2cModule> I2c = nullptr;
    //! I2C address of the chip.
    I2cAddresses Address = I2cAddresses::Addr0;
#        if NILAI_TAS5760_PWM_MODE > NILAI_TAS5760_PWM_NONE
    //! PWM output for channel 1 (left).
    PwmModule* PwmA = nullptr;
    //! PWM output for channel 2 (right).
    PwmModule* PwmB = nullptr;
#        endif

    //! Headphone mute, active low.
    Pin HpMute = {};
    //! Chip shutdown, active low.
    Pin SpkShutdown = {};

    //// Optional Pins ////
    //! Fault Pin, falling edge means the chip detected a fault.
    Pin Fault = {};
    //! Headphone detect pin. An edge means the connections/disconnection of the headphones.
    Pin HpDetect = {};

    PwrCtrlReg              PowerConfig   = {};    //!< Power control configuration register.
    DigCtrlReg              DigitalConfig = {};    //!< Digital control configuration register.
    VolCtrConfigReg         VolumeConfig = {};    //!< Global volume control configuration register.
    AnalCtrlReg             AnalogConfig = {};    //!< Analog control configuration register.
    FaultCfgAndErrStatusReg FaultConfig = {};    //!< Fault configuration and error status register.

    uint8_t DigClipLev13_6 = 0xFF;    //!< Bits 13 to 6 of the digital clipping level.
    uint8_t DigClipLev5_0  = 0xFC;    //!< Bits 5 to 0 of the digital clipping level.
};
}    // namespace Nilai::Interfaces::TAS5760

//!@}
//!@}
//!@}
#    endif
#endif

#endif    // NILAI_TAS5760_SWCONFIG_H
