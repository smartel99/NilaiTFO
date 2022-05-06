/**
 * @addtogroup fileLoggerModule.h
 * @{
 *******************************************************************************
 * @file	fileLoggerModule.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#ifndef FILELOGGERMODULE_H_
#define FILELOGGERMODULE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_FILE_LOGGER)
#    include "defines/module.hpp"

#    include "services/file.h"
#    include "services/filesystem.h"

#    include <functional>


/***********************************************/
/* Defines */
class FileLogger : public Nilai::Module
{
public:
    FileLogger() = default;
    FileLogger(const std::string& label, const std::string& path = "log.txt");
    virtual ~FileLogger() override;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const { return m_label; }

    std::function<void(const char*, size_t)> GetLogFunc()
    {
        using namespace std::placeholders;
        return std::bind(&FileLogger::Log, this, _1, _2);
    }

    void Flush();

private:
    void Log(const char* msg, size_t len);

private:
    std::string m_label = "";
    std::string m_path  = "";

    Nilai::Filesystem::File m_logFile;

    static constexpr size_t CACHE_SIZE          = NILAI_FILE_LOGGER_CACHE_SIZE;
    static constexpr size_t SYNC_TIME           = NILAI_FILE_LOGGER_SYNC_INTERVAL;
    char                    m_cache[CACHE_SIZE] = {0};
    size_t                  m_cacheLoc          = 0;
};

/***********************************************/
/* Function declarations */



/**
 * @}
 */
/* END OF FILE */
#endif
#endif /* FILELOGGERMODULE_H_ */
