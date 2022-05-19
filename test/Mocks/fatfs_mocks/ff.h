/**
 * @file    ff.h
 * @author  Samuel Martel
 * @date    2022-05-19
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

#ifndef NILAI_MOCKS_FF_H
#define NILAI_MOCKS_FF_H

#include <cstdint>

enum FRESULT
{
    FR_OK = 0,          /* (0) Succeeded */
    FR_DISK_ERR,        /* (1) A hard error occurred in the low level disk I/O layer */
    FR_INT_ERR,         /* (2) Assertion failed */
    FR_NOT_READY,       /* (3) The physical drive cannot work */
    FR_NO_FILE,         /* (4) Could not find the file */
    FR_NO_PATH,         /* (5) Could not find the path */
    FR_INVALID_NAME,    /* (6) The path name format is invalid */
    FR_DENIED,          /* (7) Access denied due to prohibited access or directory full */
    FR_EXIST,           /* (8) Access denied due to prohibited access */
    FR_INVALID_OBJECT,  /* (9) The file/directory object is invalid */
    FR_WRITE_PROTECTED, /* (10) The physical drive is write protected */
    FR_INVALID_DRIVE,   /* (11) The logical drive number is invalid */
    FR_NOT_ENABLED,     /* (12) The volume has no work area */
    FR_NO_FILESYSTEM,   /* (13) There is no valid FAT volume */
    FR_MKFS_ABORTED,    /* (14) The f_mkfs() aborted due to any problem */
    FR_TIMEOUT,         /* (15) Could not get a grant to access the volume within defined period */
    FR_LOCKED,          /* (16) The operation is rejected according to the file sharing policy */
    FR_NOT_ENOUGH_CORE, /* (17) LFN working buffer could not be allocated */
    FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_LOCK */
    FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
};

struct _FDID
{
    void*    fs = nullptr; /* Pointer to the owner file system object */
    uint16_t id;           /* Owner file system mount ID */
    uint8_t  attr;         /* Object attribute */
    uint8_t  stat;    /* Object chain status (b1-0: =0:not contiguous, =2:contiguous (no data on
* FAT),
                      =3:flagmented in this session, b2:sub-directory stretched) */
    uint32_t sclust;  /* Object start cluster (0:no cluster or root directory) */
    uint32_t objsize; /* Object size (valid when sclust != 0) */
#if _FS_EXFAT
    uint32_t n_cont; /* Size of first fragment, clusters - 1 (valid when stat == 3) */
    uint32_t n_frag; /* Size of last fragment needs to be written (valid when not zero) */
    uint32_t c_scl;  /* Containing directory start cluster (valid when sclust != 0) */
    uint32_t
      c_size; /* b31-b8:Size of containing directory, b7-b0: Chain status (valid when c_scl != 0) */
    uint32_t c_ofs; /* Offset in the containing directory (valid when sclust != 0 and non-directory
                    object) */
#endif
#if _FS_LOCK != 0
    uint32_t lockid; /* File lock ID origin from 1 (index of file semaphore table Files[]) */
#endif
};

#define _MIN_SS 512 /* 512, 1024, 2048 or 4096 */
#define _MAX_SS 512 /* 512, 1024, 2048 or 4096 */

struct FIL
{
    _FDID    obj;  /* Object identifier (must be the 1st member to detect invalid object pointer) */
    uint8_t  flag; /* File status flags */
    uint8_t  err;  /* Abort flag (error code) */
    uint32_t fptr; /* File read/write pointer (Zeroed on file open) */
    uint32_t clust; /* Current cluster of fpter (invalid when fptr is 0) */
    uint32_t sect;  /* Sector number appearing in buf[] (0:invalid) */
#if !_FS_READONLY
    uint32_t dir_sect; /* Sector number containing the directory entry */
    uint8_t* dir_ptr;  /* Pointer to the directory entry in the win[] */
#endif
#if _USE_FASTSEEK
    uint32_t* cltbl; /* Pointer to the cluster link map table (nulled on open, set by
                      * application) */
#endif
#if !_FS_TINY
    uint8_t buf[_MAX_SS]; /* File private data read/write window */
#endif
};

#endif    // NILAI_MOCKS_FF_H
