/**
 ******************************************************************************
 * @file    Logger.h
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-11-29
 *
 ******************************************************************************
 */
#ifndef GUARD_LOGGER_H
#define GUARD_LOGGER_H

#include <cstdarg>
#include <cstdio>
#include <functional>

#define LOG_DEBUG(msg, ...)                                                                        \
    do                                                                                             \
    {                                                                                              \
        Logger().Log("[Debug]: " msg, ##__VA_ARGS__);                                              \
    } while (0)
#define LOG_INFO(msg, ...)                                                                         \
    do                                                                                             \
    {                                                                                              \
        Logger().Log("[Info]: " msg "\n\r", ##__VA_ARGS__);                                        \
    } while (0)
#define LOG_WARNING(msg, ...)                                                                      \
    do                                                                                             \
    {                                                                                              \
        Logger().Log("[Warning]: " msg "\n\r", ##__VA_ARGS__);                                     \
    } while (0)
#define LOG_ERROR(msg, ...)                                                                        \
    do                                                                                             \
    {                                                                                              \
        Logger().Log("[Error]: " msg "\n\r", ##__VA_ARGS__);                                       \
    } while (0)
#define LOG_CRITICAL(msg, ...)                                                                     \
    do                                                                                             \
    {                                                                                              \
        Logger().Log("[Critical]: " msg "\n\r", ##__VA_ARGS__);                                    \
    } while (0)

using LogFunc = std::function<void(const char*, size_t)>;
class Logger
{
public:
    explicit Logger([[maybe_unused]] void*          uart    = nullptr,
                    [[maybe_unused]] const LogFunc& logFunc = {})
    {
        s_instance = this;
    }

    void Log(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        VLog(fmt, args);

        va_end(args);
    }
    void VLog(const char* fmt, va_list args) { vprintf(fmt, args); }

    void*          GetUart() { return nullptr; }
    static Logger* Get() { return s_instance; }

    void SetLogFunc([[maybe_unused]] const LogFunc& logFunc) {}

private:
    static Logger* s_instance;
};

#endif    // GUARD_LOGGER_H
