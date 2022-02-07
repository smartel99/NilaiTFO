/**
 * @addtogroup sdModule.cpp
 * @{
 *******************************************************************************
 * @file	sdModule.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */
#if defined(NILAI_USE_FILESYSTEM)
#    include "filesystem.h"

#    include "../defines/macros.hpp"
#    include "ff_gen_drv.h"
#    include "user_diskio.h"

#    define ASSERT_FS() CEP_ASSERT(s_data.fs != nullptr, "File system is not ready!");
#    define CHECK_IF_READY()                                                                       \
        do                                                                                         \
        {                                                                                          \
            if (s_data.isInit == false) return Result::NotReady;                                   \
        } while (0)

struct FsData
{
    cep::Pin sdPin;
    char     sdPath[4] = {0};    //!< SD logical drive path.
    FATFS*   fs        = nullptr;
    bool     isInit    = false;
    bool     isMounted = false;
};

static FsData s_data;

namespace cep
{
namespace Filesystem
{
bool Init(const cep::Pin& pin)
{
    s_data.sdPin = pin;
    s_data.fs    = new FATFS;
    CEP_ASSERT(s_data.fs != nullptr, "Unable to allocate memory for file system!");

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
        CEP_ASSERT(s_data.fs != nullptr, "Unable to allocate memory for file system!");
    }

    // Wait a tiny bit to make sure that the SD card is properly powered.
    HAL_Delay(5);
    Result r = (Result)f_mount(s_data.fs, drive.c_str(), (BYTE)forceMount);
    s_data.isMounted = r == Result::Ok;
    if (r != Result::Ok)
    {
        LOG_ERROR("Unable to mount drive '%s': (%d) %s", drive.c_str(), (int)r, ResultToStr(r).c_str());
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
        Result r = (Result)f_mount(s_data.fs, "", 0);
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

Result MakeVolume(const std::string& drive, const MakeVolumeParams& params)
{
    UNUSED(drive);
    UNUSED(params);
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result MakePartition(const std::string& drive, const partSize_t ps[])
{
    UNUSED(drive);
    UNUSED(ps);
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result GetFreeClusters(const std::string& drive, dword_t* outClst, fs_t** fatfs)
{
#    if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return (Result)f_getfree(drive.c_str(), outClst, fatfs);
#    else
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result GetDriveInfo(const std::string& drive, std::string& label, dword_t* sn)
{
#    if _USE_LABEL == 1
    CHECK_IF_READY();
    // Temporary buffer to contain the information.
    // 34 is the biggest size that a label can be.
    char   tmp[34] = {0};
    Result r       = (Result)f_getlabel(drive.c_str(), tmp, sn);
    label          = std::string {tmp};
    return r;
#    else
    CEP_ASSERT(false, "This function is not enabled!");
    return Result::Ok;
#    endif
}

Result SetDriveInfo(const std::string& label)
{
    UNUSED(label);
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result SetCodePage(CodePages code)
{
    UNUSED(code);
    CHECK_IF_READY();

    return Result::NotEnabled;
}

Result OpenDir(const std::string& path, dir_t* outDir)
{
#    if _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return (Result)f_opendir(outDir, path.c_str());

#    else
    UNUSED(path);
    UNUSED(outDir);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result CloseDir(dir_t* dir)
{
#    if _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return (Result)f_closedir(dir);
#    else
    UNUSED(dir);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result ReadDir(dir_t* dir, fileInfo_t* outInfo)
{
#    if _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return (Result)f_readdir(dir, outInfo);
#    else
    UNUSED(dir);
    UNUSED(outInfo);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result FindFirst(dir_t*             outDir,
                 fileInfo_t*        outInfo,
                 const std::string& dirPath,
                 const std::string& pattern)
{
#    if _USE_FIND >= 1 && _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return (Result)f_findfirst(outDir, outInfo, dirPath.c_str(), pattern.c_str());
#    else
    UNUSED(outDir);
    UNUSED(outInfo);
    UNUSED(dirPath);
    UNUSED(pattern);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result FindNext(dir_t* dir, fileInfo_t* outInfo)
{
#    if _USE_FIND >= 1 && _FS_MINIMIZE <= 1
    CHECK_IF_READY();

    return (Result)f_findnext(dir, outInfo);
#    else
    UNUSED(dir);
    UNUSED(outInfo);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result GetStat(const std::string& path, fileInfo_t* outInfo)
{
#    if _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return (Result)f_stat(path.c_str(), outInfo);
#    else
    UNUSED(dir);
    UNUSED(outInfo);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result Mkdir(const std::string& path)
{
#    if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return (Result)f_mkdir(path.c_str());
#    else
    UNUSED(path);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result Unlink(const std::string& path)
{
#    if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    CHECK_IF_READY();

    return (Result)f_unlink(path.c_str());
#    else
    UNUSED(path);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

Result Utime(const std::string& path, const fileInfo_t* fno)
{
#    if _FS_READONLY == 0 && _USE_CHMOD == 1
    CHECK_IF_READY();

    return (Result)f_utime(path.c_str(), fno);
#    else
    UNUSED(path);
    UNUSED(fno);
    CEP_ASSERT(false, "Function not enabled!");
    return Result::NotEnabled;
#    endif
}

std::string ResultToStr(Result res)
{
    switch (res)
    {
        case Result::Ok: return "OK";
        case Result::DiskError: return "Disk Error";
        case Result::IntErr: return "FATFS Internal Error";
        case Result::NotReady: return "Storage device not ready";
        case Result::NoFile: return "File not found";
        case Result::NoPath: return "Path not found";
        case Result::InvalidName: return "Path name not valid";
        case Result::Denied: return "Access denied";
        case Result::Exist: return "Object already exists";
        case Result::InvalidObject: return "Invalid object";
        case Result::WriteProtected: return "The media is write-protected";
        case Result::InvalidDrive: return "Invalid drive number";
        case Result::NotEnabled: return "Work area for the logical drive has not been mounted";
        case Result::NoFilesystem: return "No valid FAT volume found";
        case Result::MkfsAborted: return "Format aborted";
        case Result::Timeout: return "Timed out";
        case Result::Locked: return "Operation rejected by file sharing control";
        case Result::NotEnoughCore: return "Not enough memory";
        case Result::TooManyOpenFiles: return "Too many open files";
        case Result::InvalidParameter: return "Invalid Parameter";
        default: return "Unknown error";
    }
}

}    // namespace Filesystem
}    // namespace cep


#endif
