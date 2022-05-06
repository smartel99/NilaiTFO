/**
 * @file    Enums.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_TAS5760_ENUMS_H
#define NILAI_TAS5760_ENUMS_H


#if defined(NILAI_USE_SW_TAS5760)
#    include <cstdint>

namespace Nilai::Tas5760
{
//! List of the registers of the TAS5760.
enum class Registers : uint8_t
{
    /**
     * @brief Device Identification Register.
     *
     * <br>
     * @b Address: 0x00
     * <br>
     * @b Default: 0x00
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center">Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="8" style="text-align:center">
     *          Device Identification
     *      </td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *  </tr>
     * </table>
     */
    DevId = 0,

    /**
     * @brief Power Control Register.
     *
     * <br>
     * @b Address: 0x01
     * <br>
     * @b Default: 0xFD
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center">Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="6" style="text-align:center">DigClipLev[19:14]</td>
     *      <td style="border-left:1px solid;text-align:center">SPK_SLEEP</td>
     *      <td style="border-left:1px solid;text-align:center">!SPK_SD</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *  </tr>
     * </table>
     */
    PwrCtrl = 0x01,

    /**
     * @brief Digital Control Register.
     *
     * <br>
     * @b Address: 0x02
     * <br>
     * @b Default: 0x14
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center">Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td style="text-align:center">HPF Bypass</td>
     *      <td style="border-left:1px solid;text-align:center">Reserved</td>
     *      <td colspan="2" style="border-left:1px solid;text-align:center">Digital Boost</td>
     *      <td style="border-left:1px solid;text-align:center">!SS/DS</td>
     *      <td colspan="3" style="border-left:1px solid;text-align:center">Serial Audio Input
     * Format</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *  </tr>
     * </table>
     */
    DigCtrl = 0x02,

    /**
     * @brief Volume Control Configuration Register.
     *
     * <br>
     * @b Address: 0x03
     * <br>
     * @b Default 0x80
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center">Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td style="text-align:center">Fade</td>
     *      <td colspan="5" style="border-left:1px solid;text-align:center">Reserved</td>
     *      <td style="border-left:1px solid;text-align:center">Mute R</td>
     *      <td style="border-left:1px solid;text-align:center">Mute L</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *  </tr>
     * </table>
     */
    VolCtrlCfg = 0x03,

    /**
     * @brief Left Channel Volume Control Register.
     *
     * <br>
     * @b Address: 0x04
     * <br>
     * @b Default: 0xCF
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center"                      >Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="8" style="text-align:center">Volume Left</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *  </tr>
     * </table>
     */
    LeftChVolCtrl = 0x04,

    /**
     * @brief Right Channel Volume Control Register.
     *
     * <br>
     * @b Address: 0x05
     * <br>
     * @b Default: 0xCF
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center"                      >Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="8" style="text-align:center">Volume Right</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *  </tr>
     * </table>
     */
    RightChVolCtrl = 0x05,

    /**
     * @brief Analog Control Register.
     *
     * <br>
     * @b Address: 0x06
     * <br>
     * @b Default: 0x51
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center"                      >Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="1" style="text-align:center"                      >PBTL Enable</td>
     *      <td colspan="3" style="border-left:1px solid;text-align:center">PWM Rate Select</td>
     *      <td colspan="2" style="border-left:1px solid;text-align:center">A_GAIN</td>
     *      <td colspan="1" style="border-left:1px solid;text-align:center">PBTL Ch Sel</td>
     *      <td colspan="1" style="border-left:1px solid;text-align:center">Reserved</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *  </tr>
     * </table>
     */
    AnalCtrl = 0x06,

    /**
     * @brief Fault Configuration and Error Status Register.
     *
     * <br>
     * @b Address: 0x08
     * <br>
     * @b Default: 0x00
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center"                      >Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="2" style="text-align:center">Reserved</td>
     *      <td colspan="2" style="border-left:1px solid;text-align:center">OCE Threshold</td>
     *      <td colspan="1" style="border-left:1px solid;text-align:center">Clock Error</td>
     *      <td colspan="1" style="border-left:1px solid;text-align:center">Over Current Error</td>
     *      <td colspan="1" style="border-left:1px solid;text-align:center">DC Error</td>
     *      <td colspan="1" style="border-left:1px solid;text-align:center">Over Temperature
     * Error</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *  </tr>
     * </table>
     */
    FaultCfgAndErrStatus = 0x08,

    /**
     * @brief Digital Clipper 2.
     *
     * <br>
     * @b Address: 0x10
     * <br>
     * @b Default: 0xFF
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center"                      >Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="8" style="text-align:center">DigClipLev[13:6]</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *  </tr>
     * </table>
     */
    DigClip2 = 0x10,

    /**
     * @brief Digital Clipper 1.
     *
     * <br>
     * @b Address: 0x11
     * <br>
     * @b Default: 0xFC
     *
     * <br><br>
     * <table style="border:1px solid">
     *  <caption id="DevIdReg"><b>Register Map</b></caption>
     *  <tr style="border-bottom:1px solid">
     *      <th style="text-align:center"                      >Bit 7</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 6</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 5</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 4</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 3</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 2</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 1</th>
     *      <th style="border-left:1px solid;text-align:center">Bit 0</th>
     *  </tr>
     *  <tr style="border-bottom:1px solid">
     *      <td colspan="6" style="text-align:center">DigClipLev[5:0]</td>
     *      <td colspan="2" style="border-left:1px solid;text-align:center">Reserved</td>
     *  </tr>
     *  <tr>
     *      <td style="text-align:center"                      >1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">1</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *      <td style="border-left:1px solid;text-align:center">0</td>
     *  </tr>
     * </table>
     */
    DigClip1 = 0x11,
};

enum class Channels
{
    Ch1 = 0,
    Ch2
};

/**
 * @brief Possible I2C addresses of the TAS5760, which depends on its ADDR pin.
 */
enum class I2cAddresses
{
    //! When ADDR pin is LOW.
    Addr0 = 0xD8,
    //! When ADDR pin is HIGH.
    Addr1 = 0xDA
};
}    // namespace Nilai::Tas5760
#endif

#endif    // NILAI_TAS5760_ENUMS_H
