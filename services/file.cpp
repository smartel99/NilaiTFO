/**
 * @file	file.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr. 21, 2021
 *******************************************************************************
 */


#if defined(NILAI_USE_FILESYSTEM)
#    include "file.h"
#    include "filesystem.h"

#    include "../defines/macros.h"
#    include "../services/logger.h"

#    define FS_DEBUG(msg, ...)    LOG_DEBUG("[FS]: " msg, ##__VA_ARGS__)
#    define FS_INFO(msg, ...)     LOG_INFO("[FS]: " msg, ##__VA_ARGS__)
#    define FS_WARNING(msg, ...)  LOG_WARNING("[FS]: " msg, ##__VA_ARGS__)
#    define FS_ERROR(msg, ...)    LOG_ERROR("[FS]: " msg, ##__VA_ARGS__)
#    define FS_CRITICAL(msg, ...) LOG_CRITICAL("[FS]: " msg, ##__VA_ARGS__)

#    define ASSERT_FILE_IS_OK()                                                                    \
        do                                                                                         \
        {                                                                                          \
            NILAI_ASSERT(m_isOpen == true, "File is not open!");                                   \
        } while (0)

namespace Nilai::Filesystem
{
File::File(std::string path, FileModes mode) : m_path(std::move(path)), m_mode(mode), m_file {}
{
    Open(m_path, mode);
}

File::~File()
{
    //    if (m_isOpen == true)
    //    {
    //        Close();
    //    }
}

Result File::Open(std::string path, FileModes mode)
{
    // If path is empty, just reopen the file.
    if (!path.empty())
    {
        m_path = std::move(path);
    }

    m_mode   = mode;
    m_status = static_cast<Result>(
      f_open(&m_file, m_path.c_str(), std::underlying_type_t<FileModes>(m_mode)));

    if (m_status != Result::Ok)
    {
        FS_ERROR("Unable to open file '%s': %s", m_path.c_str(), ResultToStr(m_status));
        m_isOpen = false;
    }
    else
    {
        m_isOpen = true;
    }

    return m_status;
}

Result File::Close()
{
    ASSERT_FILE_IS_OK();

    m_status = static_cast<Result>(f_close(&m_file));

    if (m_status != Result::Ok)
    {
        FS_ERROR("Unable to close file '%s': %s", m_path.c_str(), ResultToStr(m_status));
    }
    else
    {
        m_isOpen = false;
    }

    return m_status;
}

Result File::Read(void* outData, size_t lenDesired, size_t* lenRead)
{
    ASSERT_FILE_IS_OK();
    NILAI_ASSERT(outData != nullptr, "Pointer is null!");
    UINT br = 0;
    auto r  = static_cast<Result>(f_read(&m_file, outData, lenDesired, &br));

    if (lenRead != nullptr)
    {
        *lenRead = br;
    }
    return r;
}

Result File::Write(const void* data, size_t dataLen, size_t* dataWritten)
{
    ASSERT_FILE_IS_OK();
    NILAI_ASSERT(data != nullptr, "Pointer is null!");

    UINT bw = 0;

    Result r = static_cast<Result>(f_write(&m_file, data, dataLen, &bw));

    if (dataWritten != nullptr)
    {
        *dataWritten = bw;
    }

    return r;
}

Result File::Seek(fsize_t ofs)
{
#    if _FS_MINIMIZE <= 2
    ASSERT_FILE_IS_OK();

    return static_cast<Result>(f_lseek(&m_file, ofs));
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::Rewind()
{
#    if _FS_MINIMIZE <= 2
    ASSERT_FILE_IS_OK();

    return static_cast<Result>(f_lseek(&m_file, 0));
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::Truncate()
{
#    if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    ASSERT_FILE_IS_OK();
    return static_cast<Result>(f_truncate(&m_file));
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::Sync()
{
#    if _FS_READONLY == 0
    ASSERT_FILE_IS_OK();
    return static_cast<Result>(f_sync(&m_file));
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::Forward([[maybe_unused]] const std::function<size_t(const uint8_t*, size_t)>& func,
                     [[maybe_unused]] size_t                                               cntToFwd,
                     [[maybe_unused]] size_t* forwarded)
{
#    if _USE_FORWARD == 1
    ASSERT_FILE_IS_OK();
#        error Not implemented!
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::Expand([[maybe_unused]] fsize_t newSize, [[maybe_unused]] AllocModes mode)
{
#    if _USE_EXPAND == 1 && _FS_READONLY == 0
    ASSERT_FILE_IS_OK();
#        error Not implemented!
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::GetString([[maybe_unused]] std::string& outStr, [[maybe_unused]] size_t maxLen)
{
#    if _USE_STRFUNC >= 1
    ASSERT_FILE_IS_OK();

    char* buff = new char[maxLen];
    NILAI_ASSERT(buff != nullptr, "Unable to allocate memory!");

    if (f_gets(buff, static_cast<int>(maxLen), &m_file) == nullptr)
    {
        m_status = static_cast<Result>(f_error(&m_file));
    }
    else
    {
        m_status = Result::Ok;
    }

    outStr = std::string {buff};

    delete[] buff;

    return m_status;
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::WriteChar([[maybe_unused]] uint8_t c)
{
#    if _FS_READONLY == 0 && _USE_STRFUNC >= 1
    ASSERT_FILE_IS_OK();

    if (f_putc(c, &m_file) <= 0)
    {
        m_status = static_cast<Result>(f_error(&m_file));
    }
    else
    {
        m_status = Result::Ok;
    }
    return m_status;
#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

Result File::WriteString([[maybe_unused]] const std::string& str)
{
#    if _FS_READONLY == 0 && _USE_STRFUNC >= 1
    ASSERT_FILE_IS_OK();

    if (f_puts(str.c_str(), &m_file) <= 0)
    {
        m_status = static_cast<Result>(f_error(&m_file));
    }
    else
    {
        m_status = Result::Ok;
    }
    return m_status;

#    else
    NILAI_ASSERT(false, "This function is not enabled");
    return Result::Ok;
#    endif
}

fsize_t File::Tell()
{
    ASSERT_FILE_IS_OK();
    return f_tell(&m_file);
}

bool File::AtEoF()
{
    ASSERT_FILE_IS_OK();
    return (f_eof(&m_file) != 0);
}

fsize_t File::GetSize()
{
    ASSERT_FILE_IS_OK();
    return f_size(&m_file);
}

bool File::HasError()
{
    return static_cast<Result>(f_error(&m_file)) != Result::Ok;
}

}    // namespace Nilai::Filesystem

#endif
