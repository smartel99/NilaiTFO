/**
 * @file    fatfs.cpp
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
#    if !defined(NILAI_TEST)
#        include "../filesystem.h"
#        include "types.h"

#        include "ff.h"

#        include "../../defines/filesystem/error_codes.h"
#        include "../../defines/macros.h"

#        include "ff_gen_drv.h"
#        include "user_diskio.h"

#        define ASSERT_FS() NILAI_ASSERT(s_data.fs != nullptr, "File system is not ready!");
#        define CHECK_IF_READY()                                                                   \
            do                                                                                     \
            {                                                                                      \
                if (s_data.isInit == false) return Result::NotReady;                               \
            } while (0)

struct FsData
{
    Nilai::Pin sdPin;
    char       sdPath[4] = {0};    //!< SD logical drive path.
    FATFS*     fs        = nullptr;
    bool       isInit    = false;
    bool       isMounted = false;
};

static FsData s_data;

namespace Nilai::Filesystem
{
Result Fopen(file_t** file, const char* path, FileModes mode)
{
    if(*file == nullptr)
    {
        *file = new file_t;
    }
    return static_cast<Result>(f_open(*file, path, static_cast<BYTE>(mode)));
}

Result Fclose(file_t** file)
{
    Result r= static_cast<Result>(f_close(*file));

    if(*file != nullptr)
    {
        delete *file;
        *file = nullptr;
    }
    return r;
}

fsize_t Fread(file_t* file, void* buff, size_t len)
{
    UINT br = 0;
    if (f_read(file, buff, len, &br) == FR_OK)
    {
        return br;
    }
    return -1;
}

fsize_t Fwrite(file_t* file, const void* buff, size_t len)
{
    UINT bw = 0;
    if (f_write(file, buff, len, &bw) == FR_OK)
    {
        return bw;
    }
    return -1;
}

Result Fseek(file_t* file, fsize_t offset)
{

#        if _FS_MINIMIZE <= 2
    return static_cast<Result>(f_lseek(file, offset));
#        else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#        endif
}

Result Frewind(file_t* file)
{
#        if _FS_MINIMIZE <= 2
    return static_cast<Result>(f_lseek(file, 0));
#        else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#        endif
}

Result Fflush(file_t* file)
{
#        if _FS_READONLY == 0
    return static_cast<Result>(f_sync(file));
#        else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#        endif
}

Result Fgets(file_t* file, std::string& outStr, size_t maxLen)
{
#        if _USE_STRFUNC >= 1
    outStr = std::string(maxLen, '\0');

    char* res = f_gets(outStr.data(), static_cast<int>(outStr.size()), file);
    if (res == nullptr)
    {
        return static_cast<Result>(f_error(file));
    }

    outStr = res;

    return Result::Ok;
#        else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#        endif
}

Result Fputc(file_t* file, int ch)
{
#        if _FS_READONLY == 0 && _USE_STRFUNC >= 1

    if (f_putc(ch, file) <= 0)
    {
        return static_cast<Result>(f_error(file));
    }
    else
    {
        return Result::Ok;
    }
#        else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#        endif
}

Result Fputs(file_t* file, const char* str)
{
#        if _FS_READONLY == 0 && _USE_STRFUNC >= 1
    if (f_puts(str, file) <= 0)
    {
        return static_cast<Result>(f_error(file));
    }
    else
    {
        return Result::Ok;
    }

#        else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#        endif
}

fsize_t Ftell(file_t* file)
{
    return f_tell(file);
}

bool Feof(file_t* file)
{
    return (f_eof(file) != 0);
}

fsize_t Fsize(file_t* file)
{
    return f_size(file);
}

bool Ferror(file_t* file)
{
    return static_cast<Result>(f_error(file)) != Result::Ok;
}

bool Init(const Nilai::Pin& pin)
{
    s_data.sdPin = pin;
    s_data.fs    = new FATFS;
    NILAI_ASSERT(s_data.fs != nullptr, "Unable to allocate memory for file system!");

    s_data.isInit = FATFS_LinkDriver(&USER_Driver, s_data.sdPath) == 0;

    return s_data.isInit;
}

void Deinit()
{
    if (s_data.fs != nullptr)
    {
        delete s_data.fs;
        s_data.fs = nullptr;
    }

    s_data.isInit = FATFS_UnLinkDriver(s_data.sdPath) != 0;
}

Result Mount(const std::string& drive, bool forceMount)
{
    CHECK_IF_READY();
    if (s_data.fs == nullptr)
    {
        s_data.fs = new FATFS;
        NILAI_ASSERT(s_data.fs != nullptr, "Unable to allocate memory for file system!");
    }

    // Wait a tiny bit to make sure that the SD card is properly powered.
    Nilai::Delay(5);
    Result r =
      static_cast<Result>(f_mount(s_data.fs, drive.c_str(), static_cast<BYTE>(forceMount)));
    s_data.isMounted = r == Result::Ok;
    if (r != Result::Ok)
    {
        LOG_ERROR("Unable to mount drive '%s': (%d) %s",
                  drive.c_str(),
                  static_cast<int>(r),
                  ResultToStr(r));
    }

    return r;
}

Result Unmount()
{
    CHECK_IF_READY();
    if (s_data.fs == nullptr)
    {
        return Result::Ok;
    }
    else
    {
        delete s_data.fs;
        s_data.fs = nullptr;

        // Mounting null is how you unmount a disk.
        Result r = static_cast<Result>(f_mount(s_data.fs, "", 0));
        if (r == Result::Ok)
        {
            s_data.isMounted = false;
        }
        return r;
    }
}

bool IsMounted()
{
    return s_data.isMounted;
}

Result MakeVolume([[maybe_unused]] const std::string&      drive,
                  [[maybe_unused]] const MakeVolumeParams& params)
{
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result MakePartition([[maybe_unused]] const std::string& drive,
                     [[maybe_unused]] const partSize_t   ps[])
{
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result GetFreeClusters(const std::string& drive, dword_t* outClst, fs_t** fatfs)
{
#        if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return static_cast<Result>(f_getfree(drive.c_str(), outClst, fatfs));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result GetDriveInfo([[maybe_unused]] const std::string& drive,
                    [[maybe_unused]] std::string&       label,
                    [[maybe_unused]] dword_t*           sn)
{
#        if _USE_LABEL == 1
    CHECK_IF_READY();
    // Temporary buffer to contain the information.
    // 34 is the biggest size that a label can be.
    char   tmp[34] = {0};
    Result r       = static_cast<Result>(f_getlabel(drive.c_str(), tmp, sn));
    label          = std::string {tmp};
    return r;
#        else
    NILAI_ASSERT(false, "This function is not enabled!");
    return Result::Ok;
#        endif
}

Result SetDriveInfo([[maybe_unused]] const std::string& label)
{
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result SetCodePage([[maybe_unused]] CodePages code)
{
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result OpenDir([[maybe_unused]] const std::string& path, [[maybe_unused]] dir_t* outDir)
{
#        if _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return static_cast<Result>(f_opendir(outDir, path.c_str()));

#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result CloseDir([[maybe_unused]] dir_t* dir)
{
#        if _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return static_cast<Result>(f_closedir(dir));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result ReadDir([[maybe_unused]] dir_t* dir, [[maybe_unused]] fileInfo_t* outInfo)
{
#        if _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return static_cast<Result>(f_readdir(dir, outInfo));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result FindFirst([[maybe_unused]] dir_t*             outDir,
                 [[maybe_unused]] fileInfo_t*        outInfo,
                 [[maybe_unused]] const std::string& dirPath,
                 [[maybe_unused]] const std::string& pattern)
{
#        if _USE_FIND >= 1 && _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return static_cast<Result>(f_findfirst(outDir, outInfo, dirPath.c_str(), pattern.c_str()));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result FindNext([[maybe_unused]] dir_t* dir, [[maybe_unused]] fileInfo_t* outInfo)
{
#        if _USE_FIND >= 1 && _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return static_cast<Result>(f_findnext(dir, outInfo));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result GetStat([[maybe_unused]] const std::string& path, [[maybe_unused]] fileInfo_t* outInfo)
{
#        if _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return static_cast<Result>(f_stat(path.c_str(), outInfo));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result Mkdir([[maybe_unused]] const std::string& path)
{
#        if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return static_cast<Result>(f_mkdir(path.c_str()));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result Unlink([[maybe_unused]] const std::string& path)
{
#        if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return static_cast<Result>(f_unlink(path.c_str()));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}

Result Utime([[maybe_unused]] const std::string& path, [[maybe_unused]] const fileInfo_t* fno)
{
#        if _FS_READONLY == 0 && _USE_CHMOD == 1
    CHECK_IF_READY();

    return static_cast<Result>(f_utime(path.c_str(), fno));
#        else
    NILAI_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#        endif
}
}    // namespace Nilai::Filesystem
#    endif
#endif
