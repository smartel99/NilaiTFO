/**
 * @file	file.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr. 21, 2021
 *******************************************************************************
 */

#ifndef NILAI_FILE_H_
#define NILAI_FILE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_FILESYSTEM)
#    if defined(NILAI_TEST)

#    else

#        include "../defines/filesystem/error_codes.h"

#        include "ff.h"

#        include <functional>
#        include <string>

namespace Nilai::Filesystem
{
using file_t  = FIL;
using fsize_t = FSIZE_t;

/**
 * Mode flags that specifies the type of access and open method for a file.
 * Multiple flags can be combined.
 */
enum class FileModes : uint8_t
{
    //!< Specifies read access to the file. Data can be read from the file.
    Read = FA_READ,
    //!< Specifies write access to the file. Data can be written to the file.
    //! Combine with Read for read-write access.
    Write = FA_WRITE,
    //!< Opens a file. The function fails if the file is not existing. (Default)
    OpenExisting = FA_OPEN_EXISTING,
    //!< Creates a new file. The function fails with Result::Exist if the file already exists.
    CreateNew = FA_CREATE_NEW,
    //!< Creates a new file. If the file already exists, it will be truncated and overwritten.
    CreateAlways = FA_CREATE_ALWAYS,
    //!< Opens the file if it exists. If it doesn't, create it.
    OpenAlways = FA_OPEN_ALWAYS,
    //!< Same as FA_OPEN_ALWAYS except the read/write pointer is set at the end of the file.
    OpenAppend = FA_OPEN_APPEND,

    DEFAULT       = Read | OpenExisting,
    DEFAULT_WRITE = Read | Write | OpenAlways,
    WRITE_APPEND  = Read | Write | OpenAppend,
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

enum class AllocModes
{
    PrepareToAllocate = 0,
    AllocateNow       = 1,
};

class File
{
public:
    File() = default;
    File(const std::string& path, FileModes mode = FileModes::DEFAULT);
    ~File();

    Result Open(const std::string& path = "", FileModes mode = FileModes::DEFAULT);
    Result Close();
    Result Read(void* outData, size_t lenDesired, size_t* lenRead = nullptr);
    Result Write(const void* data, size_t dataLen, size_t* dataWritten = nullptr);
    Result Seek(fsize_t ofs);
    Result Rewind();
    Result Truncate();
    Result Sync();
    Result Forward(const std::function<size_t(const uint8_t*, size_t)>& func,
                   size_t                                               cntToFwd,
                   size_t*                                              forwarded);
    Result Expand(fsize_t newSize, AllocModes mode);
    Result GetString(std::string& outStr, size_t maxLen = 128);
    Result WriteChar(uint8_t c);
    Result WriteString(const std::string& str);

    template<typename... Ts>
    Result WriteFmtString(const char* fmt, Ts... args)
    {
#        if _FS_READONLY == 0 && _USE_STRFUNC >= 1
#            if defined(DEBUG)
        if (!m_isOpen)
        {
            // File must be open and valid!
            return Result::NoFile;
        }
#            endif
        if (f_printf(&m_file, fmt, args...) <= 0)
        {
            m_status = (Result)f_error(&m_file);
        }
        else
        {
            m_status = static_cast<Result>(0);
        }

        return m_status;

#        else
        NILAI_ASSERT(false, "This function is not enabled");
        return 0;
#        endif
    }

    fsize_t              Tell();
    bool                 AtEoF();
    fsize_t              GetSize();
    bool                 HasError();
    [[nodiscard]] Result GetError() const { return m_status; }

    [[nodiscard]] bool IsOpen() const { return m_isOpen; }
                       operator bool() const { return IsOpen(); }

private:
    std::string m_path;
    FileModes   m_mode = FileModes::Read | FileModes::OpenExisting;
    file_t      m_file;
    bool        m_isOpen = false;
    Result      m_status = {};
};
}    // namespace Nilai::Filesystem

#    endif
#endif
/* END OF FILE */
#endif /* NILAI_FILE_H_ */
