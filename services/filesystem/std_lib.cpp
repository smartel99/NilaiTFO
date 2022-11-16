/**
 * @file    std_lib.cpp
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

#if defined(NILAI_USE_FILESYSTEM)
#    if defined(NILAI_TEST)
#        include "types.h"

#        include "../../defines/filesystem/error_codes.h"

#        include <cstdio>

#        include "../../defines/macros.h"
#        include "../../defines/pin.h"

namespace Nilai::Filesystem
{
static inline const char* FileModesFlag(FileModes modes)
{
    switch (modes)
    {
        case FileModes::Read: return "r";
        case FileModes::Write: return "w";
        case FileModes::OpenExisting: return "r+";
        case FileModes::CreateNew: return "wx";
        case FileModes::CreateAlways: return "w";
        case FileModes::OpenAlways: return "w+";
        case FileModes::OpenAppend: return "a+";
        case FileModes::WRITE_APPEND: return "a+";
        default: return "";
    }
}

Result Fopen(file_t* file, const char* path, FileModes mode)
{
    *file = std::fopen(path, FileModesFlag(mode));
    if (*file == nullptr)
    {
        return Result::NoFile;
    }
    else
    {
        return Result::Ok;
    }
}

Result Fclose(file_t* file)
{
    return static_cast<Result>(std::fclose(*file));
}


fsize_t Fread(file_t* file, void* buff, size_t len)
{
    return std::fread(buff, sizeof(char), len, *file);
}

fsize_t Fwrite(file_t* file, const void* buff, size_t len)
{
    return std::fwrite(buff, sizeof(char), len, *file);
}

Result Fseek(file_t* file, fsize_t offset)
{
    return static_cast<Result>(std::fseek(*file, offset, SEEK_SET));
}

Result Frewind(file_t* file)
{
    std::rewind(*file);
    return Result::Ok;
}

Result Fflush(file_t* file)
{
    return std::fflush(*file) == 0 ? Result::Ok : Result::IntErr;
}

Result Fgets(file_t* file, std::string& outStr, size_t maxLen)
{
    outStr = std::string(maxLen, '\0');

    char* res = fgets(outStr.data(), static_cast<int>(outStr.size()), *file);
    if (res == nullptr)
    {
        return static_cast<Result>(ferror(*file));
    }

    outStr = res;

    return Result::Ok;
}

Result Fputc(file_t* file, int ch)
{
    return std::fputc(ch, *file) == 0 ? Result::Ok : Result::IntErr;
}

Result Fputs(file_t* file, const char* str)
{
    return std::fputs(str, *file) == 0 ? Result::Ok : Result::IntErr;
}

fsize_t Ftell(file_t* file)
{
    return std::ftell(*file);
}

bool Feof(file_t* file)
{
    return (std::feof(*file) != 0);
}

fsize_t Fsize(file_t* file)
{
    long curr = std::ftell(*file);
    std::fseek(*file, 0, SEEK_END);
    fsize_t size = std::ftell(*file);
    std::fseek(*file, curr, SEEK_SET);
    return size;
}

bool Ferror(file_t* file)
{
    return ferror(*file) != 0;
}


bool Init([[maybe_unused]] const Nilai::Pin& pin)
{
    return true;
}

void Deinit()
{
}

Result Mount([[maybe_unused]] const std::string& drive, [[maybe_unused]] bool forceMount)
{
    return Result::Ok;
}

Result Unmount()
{
    return Result::Ok;
}

bool IsMounted()
{
    return true;
}

Result MakeVolume([[maybe_unused]] const std::string&      drive,
                  [[maybe_unused]] const MakeVolumeParams& params)
{
    return Result::NotEnabled;
}

Result MakePartition([[maybe_unused]] const std::string& drive,
                     [[maybe_unused]] const partSize_t   ps[])
{
    return Result::NotEnabled;
}

Result GetFreeClusters([[maybe_unused]] const std::string& drive,
                       [[maybe_unused]] dword_t*           outClst,
                       [[maybe_unused]] fs_t**             fatfs)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result GetDriveInfo([[maybe_unused]] const std::string& drive,
                    [[maybe_unused]] std::string&       label,
                    [[maybe_unused]] dword_t*           sn)
{
    NILAI_ASSERT(false, "This function is not enabled!");
    return Result::Ok;
}

Result SetDriveInfo([[maybe_unused]] const std::string& label)
{
    return Result::NotEnabled;
}

Result SetCodePage([[maybe_unused]] CodePages code)
{
    return Result::NotEnabled;
}

Result OpenDir([[maybe_unused]] const std::string& path, [[maybe_unused]] dir_t* outDir)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result CloseDir([[maybe_unused]] dir_t* dir)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result ReadDir([[maybe_unused]] dir_t* dir, [[maybe_unused]] fileInfo_t* outInfo)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result FindFirst([[maybe_unused]] dir_t*             outDir,
                 [[maybe_unused]] fileInfo_t*        outInfo,
                 [[maybe_unused]] const std::string& dirPath,
                 [[maybe_unused]] const std::string& pattern)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result FindNext([[maybe_unused]] dir_t* dir, [[maybe_unused]] fileInfo_t* outInfo)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result GetStat([[maybe_unused]] const std::string& path, [[maybe_unused]] fileInfo_t* outInfo)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result Mkdir([[maybe_unused]] const std::string& path)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result Unlink([[maybe_unused]] const std::string& path)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

Result Utime([[maybe_unused]] const std::string& path, [[maybe_unused]] const fileInfo_t* fno)
{
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
}

}    // namespace Nilai::Filesystem
#    endif
#endif
