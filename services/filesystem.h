/**
 * @addtogroup sdModule.h
 * @{
 *******************************************************************************
 * @file	sdModule.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#ifndef SDMODULE_H_
#define SDMODULE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_FILESYSTEM)

#include "file.h"
#include "defines/pin.h"

#include "ff.h"

#include <string>


namespace cep
{
// TODO Make the file system an object-oriented class that actually represents a file system.
namespace Filesystem
{
using partSize_t = DWORD;
using dword_t    = unsigned long;
using fs_t       = FATFS;
using dir_t      = DIR;
using fileInfo_t = FILINFO;


enum class Result
{
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
};



enum class CodePages
{
    // TODO http://elm-chan.org/fsw/ff/doc/setcp.html
};


struct MakeVolumeParams
{
    // TODO
};


bool   Init(const Pin& detect);
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

std::string ResultToStr(Result res);
};    // namespace Filesystem
}    // namespace cep



/**
 * @}
 */
#endif
/* END OF FILE */
#endif /* FILESYSTEM_H_ */
