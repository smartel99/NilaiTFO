/**
 * @file	filesystem.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */
#if defined(NILAI_USE_FILESYSTEM)
#    include "filesystem.h"


namespace Nilai::Filesystem
{

const char* ResultToStr(Result res)
{
#    if defined(NILAI_FS_STATUS_STRING)
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
#    else
    NILAI_UNUSED(res);
    return "";
#    endif
}

}    // namespace Nilai::Filesystem


#endif
