/**
 * @file    types.h
 * @author  Samuel Martel
 * @date    2022-11-11
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

#ifndef NILAI_SERVICES_FILESYSTEM_TYPES_H
#define NILAI_SERVICES_FILESYSTEM_TYPES_H

#if defined(NILAI_USE_FILESYSTEM)
#    if !defined(NILAI_TEST)
#        include "ff.h"
#    else
#        include <cstdio>
#    endif

#    include "../../defines/filesystem/error_codes.h"
#    include "../../defines/pin.h"

#    include <string>

namespace Nilai::Filesystem
{
#    if !defined(NILAI_TEST)
using file_t  = FIL;
using fsize_t = FSIZE_t;

using partSize_t = DWORD;
using dword_t    = unsigned long;
using fs_t       = FATFS;
using dir_t      = DIR;
using fileInfo_t = FILINFO;

/**
 * @enum FileModes
 * @brief Mode flags that specifies the type of access and open method for a file.
 * Multiple flags can be combined.
 */
enum class FileModes : uint8_t
{
    //! Specifies read access to the file. Data can be read from the file.
    Read = FA_READ,
    //! Specifies write access to the file. Data can be written to the file.
    //! Combine with Read for read-write access.
    Write = FA_WRITE,
    //! Opens a file. The function fails if the file is not existing. (Default)
    OpenExisting = FA_OPEN_EXISTING,
    //! Creates a new file. The function fails with Result::Exist if the file already exists.
    CreateNew = FA_CREATE_NEW,
    //! Creates a new file. If the file already exists, it will be truncated and overwritten.
    CreateAlways = FA_CREATE_ALWAYS,
    //! Opens the file if it exists. If it doesn't, create it.
    OpenAlways = FA_OPEN_ALWAYS,
    //! Same as FA_OPEN_ALWAYS except the read/write pointer is set at the end of the file.
    OpenAppend = FA_OPEN_APPEND,

    DEFAULT       = Read | OpenExisting,
    DEFAULT_WRITE = Read | Write | OpenAlways,
    WRITE_APPEND  = Read | Write | OpenAppend,
};
#    else
using file_t  = FILE;
using fsize_t = std::size_t;

using partSize_t = std::size_t;
using dword_t    = std::size_t;
using fs_t       = void;
using dir_t      = void;
using fileInfo_t = void;

using FATFS = void;

/**
 * @enum FileModes
 * @brief Mode flags that specifies the type of access and open method for a file.
 * Multiple flags can be combined.
 */
enum class FileModes : uint8_t
{
    //! Specifies read access to the file. Data can be read from the file.
    Read = 0,
    //! Specifies write access to the file. Data can be written to the file.
    //! Combine with Read for read-write access.
    Write,
    //! Opens a file. The function fails if the file is not existing. (Default)
    OpenExisting,
    //! Creates a new file. The function fails with Result::Exist if the file already exists.
    CreateNew,
    //! Creates a new file. If the file already exists, it will be truncated and overwritten.
    CreateAlways,
    //! Opens the file if it exists. If it doesn't, create it.
    OpenAlways,
    //! Same as FA_OPEN_ALWAYS except the read/write pointer is set at the end of the file.
    OpenAppend,

    DEFAULT       = Read | OpenExisting,
    DEFAULT_WRITE = Read | Write | OpenAlways,
    WRITE_APPEND  = Read | Write | OpenAppend,
};
#    endif

enum class CodePages
{
    // TODO http://elm-chan.org/fsw/ff/doc/setcp.html
};


struct MakeVolumeParams
{
    // TODO
};

constexpr inline FileModes operator|(FileModes a, FileModes b)
{
    return static_cast<FileModes>(static_cast<std::underlying_type_t<FileModes>>(a) |
                                  static_cast<std::underlying_type_t<FileModes>>(b));
}
constexpr inline FileModes operator&(FileModes a, FileModes b)
{
    return static_cast<FileModes>(static_cast<std::underlying_type_t<FileModes>>(a) &
                                  static_cast<std::underlying_type_t<FileModes>>(b));
}
constexpr inline FileModes operator|=(FileModes& a, const FileModes& b)
{
    return a = a | b;
}

Result  Fopen(file_t** file, const char* path, FileModes mode);
Result  Fclose(file_t** file);
fsize_t Fread(file_t* file, void* buff, size_t len);
fsize_t Fwrite(file_t* file, const void* buff, size_t len);
Result  Fseek(file_t* file, fsize_t offset);
Result  Frewind(file_t* file);
Result  Fflush(file_t* file);
Result  Fgets(file_t* file, std::string& outStr, size_t maxLen);
Result  Fputc(file_t* file, int ch);
Result  Fputs(file_t* file, const char* str);
fsize_t Ftell(file_t* file);
bool    Feof(file_t* file);
fsize_t Fsize(file_t* file);
bool    Ferror(file_t* file);

namespace Impl
{
bool   Init(const Nilai::Pin& detect = {});
void   Deinit();
Result Mount(const std::string& drive = "", bool forceMount = false);
Result Unmount();
bool   IsMounted();

Result MakeVolume(const std::string& drive, const MakeVolumeParams& params);
Result MakePartition(const std::string& drive, const partSize_t ps[]);

Result GetFreeClusters(const std::string& drive, dword_t* outClst, fs_t** fatfs);
Result GetDriveInfo(const std::string& drive, std::string& label, dword_t* sn = nullptr);
Result SetDriveInfo(const std::string& label);

Result SetCodePage(CodePages code);

Result OpenDir(const std::string& path, dir_t* outDir);
Result CloseDir(dir_t* dir);
Result ReadDir(dir_t* dir, fileInfo_t* outInfo);
Result FindFirst(dir_t*             outDir,
                 fileInfo_t*        outInfo,
                 const std::string& dirPath,
                 const std::string& pattern = "");
Result FindNext(dir_t* dir, fileInfo_t* outInfo);

Result GetStat(const std::string& path, fileInfo_t* outInfo);

Result Mkdir(const std::string& path);
Result Unlink(const std::string& path);

Result Utime(const std::string& path, const fileInfo_t* fno);
}    // namespace Impl

}    // namespace Nilai::Filesystem
#endif
#endif    // NILAI_SERVICES_FILESYSTEM_TYPES_H
