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
#ifndef NILAI_ENUMS_H
#define NILAI_ENUMS_H

#if defined(NILAI_USE_I2C)
namespace Nilai::I2C
{
/**
 ** @enum   Status
 ** @brief  I2C status enum, indicating mostly error states
 **/
enum class Status
{
    //!< No error.
    Ok = 0x00000000,
    /** Bus Error.
     ** This error occurs when the I2C interface detects an external Stop or
     ** Start condition during an address or a data transfer.
     **/
    BusError = 0x00000001,
    /** Arbitration lost.
     ** This error occurs when the I2C interface detects an arbitration lost
     ** condition.
     **/
    ArbitrationLost = 0x00000002,
    /** Acknowledge Failure.
     ** This error occurs when the interface detects a non-acknowledge bit.
     **/
    AckFailure = 0x00000004,
    /** Overrun/underrun error.
     ** An overrun error can occur in slave mode when clock stretching is
     ** disabled and the I2C interface is receiving data.
     ** The interface has received a byte and the data in the data register
     ** has not been read before the next byte is received by the interface.
     **
     ** Underrun error can occur in slave mode when clock stretching is
     ** disabled and the I2C interface is transmitting data.
     ** The interface has not updated the data register with the next byte
     ** before the clock comes for the next byte.
     **/
    OverrunError     = 0x00000008,
    DmaTransferError = 0x00000010,
    TimeoutError     = 0x00000020,
    SizeError        = 0x00000040,
    DmaParamError    = 0x00000080,
    InvalidCallback  = 0x00000100,
    WrongStart       = 0x00000200,
    NotInit          = 0x00000400,
    BadInit          = 0x00000800,
};
}    // namespace Nilai::I2C
#endif

#endif    // NILAI_ENUMS_H
