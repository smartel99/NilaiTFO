/**
 * @addtogroup  defines
 * @{
 * @addtogroup  logger
 * @{
 *
 * @file        Logger.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/06/29 - 11:30
 */
#pragma once

/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "defines/_pch.hpp"
#include "Core/Inc/usart.h"


/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */
#pragma region Macros
/**
 * @brief   Checks if the logging module is enabled before entering a function's implementation. \n
 * This macro exits the function if the logging module is not enabled.                           \n
 */
#define CHECK_LOG_ENABLED()                                                                        \
    do                                                                                             \
    {                                                                                              \
        if (s_instance.IsEnabled() == false)                                                       \
        {                                                                                          \
            return;                                                                                \
        }                                                                                          \
    } while (false)


/**
 * @defgroup    LOGGING     Logging Macros
 *
 * For examples and details on to how to use these macros, refer to the README file in the
 * cep20/Tornatest repository.
 *
 * @note The usage of the `##` symbol preceding `__VA_ARGS__` is *non-standard*, and is an
 *       extension of the language as of C++17.                                                  \n
 *       This extension is however supported by gcc, llvm/clang and msbuild.
 *
 * @{
 */
#ifdef LOG_DEBUG     /* Quick fix because `LOG_DEBUG` is defined as empty in `macros.hpp` to */
#    undef LOG_DEBUG /* avoid warnings. */
#endif
/** Prints the lowest-priority message */
#define LOG_DEBUG(msg, ...) Log::Debug((msg), __FILE__, SPDLOG_FUNCTION, __LINE__, ##__VA_ARGS__)

/** Prints a low-priority message */
#define LOG_INFO(msg, ...) Log::Info((msg), __FILE__, SPDLOG_FUNCTION, __LINE__, ##__VA_ARGS__)

/** Prints a medium-priority message */
#define LOG_WARNING(msg, ...)                                                                      \
    Log::Warning((msg), __FILE__, SPDLOG_FUNCTION, __LINE__, ##__VA_ARGS__)

/** Prints a high-priority message */
#define LOG_ERROR(msg, ...) Log::Error((msg), __FILE__, SPDLOG_FUNCTION, __LINE__, ##__VA_ARGS__)

/** Prints the highest-priority message */
#define LOG_CRITICAL(msg, ...)                                                                     \
    Log::Critical((msg), __FILE__, SPDLOG_FUNCTION, __LINE__, ##__VA_ARGS__)
/**
 * @}
 */
#pragma endregion


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */

/**
 * @class   Log
 * @brief   Static logging interface wrapping 5 levels of `spdlog` loggers.
 *
 *          The levels, in order of lower priority to higher priority, are:
 *          - Debug
 *          - Info
 *          - Warning
 *          - Error
 *          - Critical
 */
class Log
{
    /*********************************************************************************************/
    /* Private member variables ---------------------------------------------------------------- */
private:
    static Log                      s_instance;
    bool                            m_isEnabled;
    std::shared_ptr<spdlog::logger> m_Logger = {}; /* Create empty logger before initialization .*/


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    Log() {}
    static void Init();

    ALWAYS_INLINE void Enable() { m_isEnabled = true; }
    ALWAYS_INLINE void Disable() { m_isEnabled = true; }
    ALWAYS_INLINE bool IsEnabled() { return m_isEnabled; }


    /*********************************************************************************************/
    /* Public static member functions ---------------------------------------------------------- */
#pragma region Public static member functions

    /**
     * @defgroup    LOGGING     Logging functions
     */
    template<typename... Args>
    static void Debug(const std::string& message,
                      const char*        file,
                      const char*        func,
                      std::uint32_t      line,
                      Args&&... args)
    {
        CHECK_LOG_ENABLED();
        spdlog::source_loc messageSourceLocation {file, static_cast<int>(line), func};
        LogImpl(messageSourceLocation,
                spdlog::level::level_enum::debug,
                message,
                std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Info(const std::string& message,
                     const char*        file,
                     const char*        func,
                     std::uint32_t      line,
                     Args&&... args)
    {
        CHECK_LOG_ENABLED();
        spdlog::source_loc messageSourceLocation {file, static_cast<int>(line), func};
        LogImpl(messageSourceLocation,
                spdlog::level::level_enum::info,
                message,
                std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(const std::string& message,
                        const char*        file,
                        const char*        func,
                        std::uint32_t      line,
                        Args&&... args)
    {
        CHECK_LOG_ENABLED();
        spdlog::source_loc messageSourceLocation {file, static_cast<int>(line), func};
        LogImpl(messageSourceLocation,
                spdlog::level::level_enum::warn,
                message,
                std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(const std::string& message,
                      const char*        file,
                      const char*        func,
                      std::uint32_t      line,
                      Args&&... args)
    {
        CHECK_LOG_ENABLED();
        spdlog::source_loc messageSourceLocation {file, static_cast<int>(line), func};
        LogImpl(messageSourceLocation,
                spdlog::level::level_enum::err,
                message,
                std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Critical(const std::string& message,
                         const char*        file,
                         const char*        func,
                         std::uint32_t      line,
                         Args&&... args)
    {
        CHECK_LOG_ENABLED();
        spdlog::source_loc messageSourceLocation {file, static_cast<int>(line), func};
        LogImpl(messageSourceLocation,
                spdlog::level::level_enum::critical,
                message,
                std::forward<Args>(args)...);
    }
    /**
     * @}
     */
#pragma endregion


    /*********************************************************************************************/
    /* Private static member functions --------------------------------------------------------- */
#pragma region Private static member functions

private:
    template<typename... Args>
    static void LogImpl(const spdlog::source_loc& source,
                        spdlog::level::level_enum logLevel,
                        const std::string&        message,
                        Args&&... args)
    {
#if DEBUG
        std::string_view file {source.filename};
        std::string_view function {source.funcname};

        s_instance.m_Logger->log(source,
                                 logLevel,
                                 {"[{}:{} ({})] " + message},
                                 file,
                                 source.line,
                                 function,
                                 std::forward<Args>(args)...);
#else
        s_instance.m_Logger->log(source, logLevel, message);
#endif
    }

    static void CustomErrorHandler(const std::string& msg);
    static void CustomLogHandler(const spdlog::memory_buf_t& formattedMsg);
#pragma endregion
};


/*************************************************************************************************/
/* LOG_CRITICAL("Have a wonderful day! :)"); */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
