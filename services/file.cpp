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

#    define FS_DEBUG(msg, ...)    LOG_DEBUG("[FS]: " msg __VA_OPT__(, ) __VA_ARGS__)
#    define FS_INFO(msg, ...)     LOG_INFO("[FS]: " msg __VA_OPT__(, ) __VA_ARGS__)
#    define FS_WARNING(msg, ...)  LOG_WARNING("[FS]: " msg __VA_OPT__(, ) __VA_ARGS__)
#    define FS_ERROR(msg, ...)    LOG_ERROR("[FS]: " msg __VA_OPT__(, ) __VA_ARGS__)
#    define FS_CRITICAL(msg, ...) LOG_CRITICAL("[FS]: " msg __VA_OPT__(, ) __VA_ARGS__)

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
    m_status = Fopen(&m_file, m_path.c_str(), m_mode);

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

    m_status = static_cast<Result>(Fclose(&m_file));

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
    fsize_t r = Fread(m_file, outData, lenDesired);

    if (lenRead != nullptr)
    {
        *lenRead = r;
    }
    return r == fsize_t(-1) ? Result::IntErr : Result::Ok;
}

Result File::Write(const void* data, size_t dataLen, size_t* dataWritten)
{
    ASSERT_FILE_IS_OK();
    NILAI_ASSERT(data != nullptr, "Pointer is null!");

    fsize_t r = Fwrite(m_file, data, dataLen);

    if (dataWritten != nullptr)
    {
        *dataWritten = r;
    }
    return r == fsize_t(-1) ? Result::IntErr : Result::Ok;
}

Result File::Seek(fsize_t ofs)
{
    ASSERT_FILE_IS_OK();

    return Fseek(m_file, ofs);
}

Result File::Rewind()
{
    ASSERT_FILE_IS_OK();

    return Frewind(m_file);
}

Result File::Sync()
{
    ASSERT_FILE_IS_OK();
    return Fflush(m_file);
}

Result File::GetString([[maybe_unused]] std::string& outStr, [[maybe_unused]] size_t maxLen)
{
    ASSERT_FILE_IS_OK();
    return Fgets(m_file, outStr, maxLen);
}

Result File::WriteChar([[maybe_unused]] uint8_t c)
{
    ASSERT_FILE_IS_OK();
    return Fputc(m_file, c);
}

Result File::WriteString([[maybe_unused]] const std::string& str)
{
    ASSERT_FILE_IS_OK();
    return Fputs(m_file, str.c_str());
}

fsize_t File::Tell()
{
    ASSERT_FILE_IS_OK();
    return Ftell(m_file);
}

bool File::AtEoF()
{
    ASSERT_FILE_IS_OK();
    return Feof(m_file);
}

fsize_t File::GetSize()
{
    ASSERT_FILE_IS_OK();
    return Fsize(m_file);
}

bool File::HasError()
{
    return Ferror(m_file);
}

}    // namespace Nilai::Filesystem

#endif
