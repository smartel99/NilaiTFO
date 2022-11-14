/**
 * @file    error_codes.h
 * @author  Samuel Martel
 * @date    2022-02-24
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
#ifndef NILAI_FILESYSTEM_ERRORCODES_H
#define NILAI_FILESYSTEM_ERRORCODES_H

#if defined(NILAI_USE_FILESYSTEM)

#    if !defined(NILAI_TEST)
#        include "ff.h"
#    endif


namespace Nilai::Filesystem
{
enum class Result
{
#    if !defined(NILAI_TEST)
    Ok               = FR_OK,
    DiskError        = FR_DISK_ERR,
    IntErr           = FR_INT_ERR,
    NotReady         = FR_NOT_READY,
    NoFile           = FR_NO_FILE,
    NoPath           = FR_NO_PATH,
    InvalidName      = FR_INVALID_NAME,
    Denied           = FR_DENIED,
    Exist            = FR_EXIST,
    InvalidObject    = FR_INVALID_OBJECT,
    WriteProtected   = FR_WRITE_PROTECTED,
    InvalidDrive     = FR_INVALID_DRIVE,
    NotEnabled       = FR_NOT_ENABLED,
    NoFilesystem     = FR_NO_FILESYSTEM,
    MkfsAborted      = FR_MKFS_ABORTED,
    Timeout          = FR_TIMEOUT,
    Locked           = FR_LOCKED,
    NotEnoughCore    = FR_NOT_ENOUGH_CORE,
    TooManyOpenFiles = FR_TOO_MANY_OPEN_FILES,
    InvalidParameter = FR_INVALID_PARAMETER,
#    else
    Ok               = 0,
    DiskError        = -1,
    IntErr           = -1,
    NotReady         = -1,
    NoFile           = -1,
    NoPath           = -1,
    InvalidName      = -1,
    Denied           = -1,
    Exist            = -1,
    InvalidObject    = -1,
    WriteProtected   = -1,
    InvalidDrive     = -1,
    NotEnabled       = -1,
    NoFilesystem     = -1,
    MkfsAborted      = -1,
    Timeout          = -1,
    Locked           = -1,
    NotEnoughCore    = -1,
    TooManyOpenFiles = -1,
    InvalidParameter = -1,
#    endif
};
}    // namespace Nilai::Filesystem
#endif

#endif    // NILAI_FILESYSTEM_ERRORCODES_H
