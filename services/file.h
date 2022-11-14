/**
 * @file	file.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr. 21, 2021
 *******************************************************************************
 */

#ifndef NILAI_SERVICES_FILESYSTEM_FILE_H_
#define NILAI_SERVICES_FILESYSTEM_FILE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_FILESYSTEM)
#    include "../defines/filesystem/error_codes.h"
#    include "../defines/macros.h"

#    include "filesystem/types.h"

#    include <functional>
#    include <string>
#    include <string_view>

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Services
 * @{
 */

/**
 * @addtogroup Filesystem
 * @{
 */

namespace Nilai::Filesystem
{
/**
 * @enum AllocModes
 * @brief Modes of allocation used by FATFS
 */
enum class AllocModes
{
    PrepareToAllocate = 0,
    AllocateNow       = 1,
};

/**
 * @class File
 * @brief Structure representing a file object.
 */
class File
{
public:
    File() = default;
    File(std::string path, FileModes mode = FileModes::DEFAULT);
    ~File();

    Result Open(std::string path = "", FileModes mode = FileModes::DEFAULT);
    Result Close();
    Result Read(void* outData, size_t lenDesired, size_t* lenRead = nullptr);
    Result Write(const void* data, size_t dataLen, size_t* dataWritten = nullptr);
    Result Seek(fsize_t ofs);
    Result Rewind();
    Result Sync();
    Result GetString(std::string& outStr, size_t maxLen = 128);
    Result WriteChar(uint8_t c);
    Result WriteString(const std::string& str);

    template<typename... Ts>
    Result WriteFmtString([[maybe_unused]] const char* fmt, [[maybe_unused]] Ts... args)
    {
#    if !defined(NILAI_TEST)
#        if _FS_READONLY == 0 && _USE_STRFUNC >= 1
#            if defined(DEBUG)
        if (!m_isOpen)
        {
            // File must be open and valid!
            return Result::NoFile;
        }
#            endif
        if (f_printf(m_file, fmt, args...) <= 0)
        {
            m_status = static_cast<Result>(f_error(m_file));
        }
        else
        {
            m_status = static_cast<Result>(0);
        }

        return m_status;

#        else
        NILAI_ASSERT(false, "This function is not enabled");
        return Result::Ok;
#        endif
#    else
        fprintf(m_file, fmt, args...);
        return Result::Ok;
#    endif
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
    FileModes   m_mode   = FileModes::Read | FileModes::OpenExisting;
    file_t*     m_file   = nullptr;
    bool        m_isOpen = false;
    Result      m_status = {};
};
}    // namespace Nilai::Filesystem

//!@}
//!@}
//!@}
#endif
/* END OF FILE */
#endif /* NILAI_SERVICES_FILESYSTEM_FILE_H_ */
