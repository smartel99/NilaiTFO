/**
 * @file        logger.h
 * @author      Samuel Martel
 * @date        2020/06/29 - 11:30
 */
#ifndef NILAI_LOGGER_H
#define NILAI_LOGGER_H

#if defined(NILAI_USE_LOGGER) && !defined(NILAI_TEST)

/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#    include "../defines/smart_pointers.h"
#    include "time.h"

#    include <cstdarg>    // For va_list
#    include <functional>

#    if !defined(NILAI_LOGGER_USE_RTC)
#        include NILAI_HAL_HEADER
#        define GET_CUR_MS()  (Nilai::GetTime())
#        define GET_CUR_SEC() (GET_CUR_MS() / 1000)
#        define GET_CUR_MIN() (GET_CUR_SEC() / 60)
#        define GET_CUR_H()   (GET_CUR_MIN() / 60)

#        define LOG_HELPER(color, msg, ...)                                                        \
            do                                                                                     \
            {                                                                                      \
                Nilai::Services::Logger::Get() != nullptr                                          \
                  ? Nilai::Services::Logger::Get()->Log(color "[%08i] " msg,         \
                                       GET_CUR_MS() __VA_OPT__(, ) __VA_ARGS__)             \
                  : (void)0;                                                                       \
            } while (0)
#        define INT_NILAI_LOG_IMPL_OK
#    elif !defined(NILAI_USE_RTC)
#        error NILAI_LOGGER_USE_RTC was defined, the RTC module must also be enabled!
#    else
#        include "../drivers/rtc_module.h"
#        define LOG_HELPER(color, msg, ...)                                                        \
            do                                                                                     \
            {                                                                                      \
                Nilai::Services::Logger::Get() != nullptr                                          \
                  ? Nilai::Services::Logger::Get()->Log(                                           \
                      color "[%s %s.%03i] " msg "\033[0m",                                         \
                      Nilai::Drivers::RtcModule::Get()->GetDate().ToStr().c_str(),                 \
                      Nilai::Drivers::RtcModule::Get()->GetTime().ToStr().c_str(),                 \
                      Nilai::GetTime() % 1000 __VA_OPT__(, ) __VA_ARGS__)                          \
                  : (void)0;                                                                       \
            } while (0)
#        define INT_NILAI_LOG_IMPL_OK
#    endif

#    if defined(NILAI_LOG_ENABLE_DEBUG) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_DEBUG(msg, ...)                                                                \
            LOG_HELPER("\033[37;40mD ", msg "\n\r" __VA_OPT__(, ) __VA_ARGS__)
#    else
#        define LOG_DEBUG(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_INFO) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_INFO(msg, ...)                                                                 \
            LOG_HELPER("\033[32;40mI ", msg "\n\r" __VA_OPT__(, ) __VA_ARGS__)
#    else
#        define LOG_INFO(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_WARNING) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_WARNING(msg, ...)                                                              \
            LOG_HELPER("\033[93;40mW ", msg "\n\r" __VA_OPT__(, ) __VA_ARGS__)
#    else
#        define LOG_WARNING(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_ERROR) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_ERROR(msg, ...)                                                                \
            LOG_HELPER("\033[91;40mE ", msg "\n\r" __VA_OPT__(, ) __VA_ARGS__)
#    else
#        define LOG_ERROR(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_CRITICAL) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_CRITICAL(msg, ...)                                                             \
            LOG_HELPER("\033[97;101mC ", msg "\n\r" __VA_OPT__(, ) __VA_ARGS__)
#    else
#        define LOG_CRITICAL(msg, ...)
#    endif

#    if defined(NILAI_USE_UART)
namespace Nilai::Drivers
{
class UartModule;
}
#    endif

namespace Nilai::Services
{
using LogFunc = std::function<void(const char*, size_t)>;

class Logger
{
public:
#    if defined(NILAI_USE_UART)
    explicit Logger(
        Drivers::UartModule& uart,
        LogFunc              logFunc = [](const char*, size_t)
        {
        });
#    else
    Logger(const LogFunc& logFunc);
#    endif
    ~Logger();

    void Log(const char* fmt, ...);
    void VLog(const char* fmt, va_list args);

    void SetLogFunc(const LogFunc& logFunc);
#    if defined(NILAI_USE_UART)
    Drivers::UartModule& GetUart() { return m_uart; }
#    endif

    static Logger* Get() { return s_instance; }

private:
    static Logger* s_instance;

#    if defined(NILAI_USE_UART)
    Drivers::UartModule& m_uart;
#    endif
    LogFunc m_logFunc = [](const char*, size_t)
    {
    };
};
} // namespace Nilai::Services
#else
#    define LOG_HELPER(fmt, ...)
#    define LOG_DEBUG(msg, ...)
#    define LOG_INFO(msg, ...)
#    define LOG_WARNING(msg, ...)
#    define LOG_ERROR(msg, ...)
#    define LOG_CRITICAL(msg, ...)
#endif
/*************************************************************************************************/
/* LOG_CRITICAL("Have a wonderful day! :)"); */
/* ----- END OF FILE ----- */
#endif
