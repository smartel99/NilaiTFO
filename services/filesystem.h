/**
 * @file	filesystem.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#ifndef NILAI_FILESYSTEM_H
#define NILAI_FILESYSTEM_H

/***********************************************/
/* Includes */
#if defined(NILAI_USE_FILESYSTEM)
#    include "../defines/filesystem/error_codes.h"
#    include "../defines/pin.h"
#    include "ff.h"

#    include "file.h"

#    include <string>


namespace Nilai
{
// TODO Make the file system an object-oriented class that actually represents a file system.
namespace Filesystem
{
using partSize_t = DWORD;
using dword_t    = unsigned long;
using fs_t       = FATFS;
using dir_t      = DIR;
using fileInfo_t = FILINFO;

enum class CodePages
{
    // TODO http://elm-chan.org/fsw/ff/doc/setcp.html
};


struct MakeVolumeParams
{
    // TODO
};


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

const char* ResultToStr(Result res);
}    // namespace Filesystem
}    // namespace Nilai
#endif
/* END OF FILE */
#endif /* FILESYSTEM_H_ */
