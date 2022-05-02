/**
 * @addtogroup  defines
 * @{
 * @addtogroup  logger
 * @{
 *
 * @file        Logger.hpp
 * @author      Samuel Martel
 * @date        2020/06/29 - 11:30
 */
#pragma once

#if defined(NILAI_TEST)
#include "test/Mocks/Logger.h"
#elif defined(NILAI_USE_LOGGER)

/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#    include "../defines/internalConfig.h"
#include "../defines/Core.h"

#    include <cstdarg>    // For va_list
#    include <functional>

#    if !defined(NILAI_LOGGER_USE_RTC)
#        include NILAI_HAL_HEADER
#        define GET_CUR_MS()  (HAL_GetTick())
#        define GET_CUR_SEC() (GET_CUR_MS() / 1000)
#        define GET_CUR_MIN() (GET_CUR_SEC() / 60)
#        define GET_CUR_H()   (GET_CUR_MIN() / 60)

#        define LOG_HELPER(msg, ...)                                                               \
            do                                                                                     \
            {                                                                                      \
                Logger::Get() != nullptr ? Logger::Get()->Log("[%02i:%02i:%02i.%03i]" msg,         \
                                                              GET_CUR_H(),                         \
                                                              GET_CUR_MIN() % 60,                  \
                                                              GET_CUR_SEC() % 60,                  \
                                                              GET_CUR_MS() % 1000,                 \
                                                              ##__VA_ARGS__)                       \
                                         : (void)0;                                                \
            } while (0)
#        define INT_NILAI_LOG_IMPL_OK
#    elif !defined(NILAI_USE_RTC)
#        error NILAI_LOGGER_USE_RTC was defined, the RTC module must also be enabled!
#    else
#        include "drivers/rtcModule.h"
#        define LOG_HELPER(msg, ...)                                                               \
            do                                                                                     \
            {                                                                                      \
                Logger::Get() != nullptr                                                           \
                  ? Logger::Get()->Log("[%s %s.%03i] " msg,                                        \
                                       RtcModule::Get()->GetDate().ToStr().c_str(),                \
                                       RtcModule::Get()->GetTime().ToStr().c_str(),                \
                                       HAL_GetTick() % 1000,                                       \
                                       ##__VA_ARGS__)                                              \
                  : (void)0;                                                                       \
            } while (0)
#        define INT_NILAI_LOG_IMPL_OK
#    endif

#    if defined(NILAI_LOG_ENABLE_DEBUG) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_DEBUG(msg, ...) LOG_HELPER("[DEB ]: " msg "\n\r\0", ##__VA_ARGS__)
#    else
#        define LOG_DEBUG(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_INFO) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_INFO(msg, ...) LOG_HELPER("[INFO]: " msg "\n\r\0", ##__VA_ARGS__)
#    else
#        define LOG_INFO(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_WARNING) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_WARNING(msg, ...) LOG_HELPER("[WARN]: " msg "\n\r\0", ##__VA_ARGS__)
#    else
#        define LOG_WARNING(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_ERROR) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_ERROR(msg, ...) LOG_HELPER("[ERR ]: " msg "\n\r\0", ##__VA_ARGS__)
#    else
#        define LOG_ERROR(msg, ...)
#    endif

#    if defined(NILAI_LOG_ENABLE_CRITICAL) && defined(INT_NILAI_LOG_IMPL_OK)
#        define LOG_CRITICAL(msg, ...) LOG_HELPER("[CRIT]: " msg "\n\r\0", ##__VA_ARGS__)
#    else
#        define LOG_CRITICAL(msg, ...)
#    endif

#    if defined(NILAI_USE_UART)
class UartModule;
#    endif

using LogFunc = std::function<void(const char*, size_t)>;

class Logger
{
public:
#    if defined(NILAI_USE_UART)
    explicit Logger(cep::Ref<UartModule> uart = nullptr, const LogFunc& logFunc = {});
#    else
    Logger(const LogFunc& logFunc);
#    endif
    ~Logger();

    void Log(const char* fmt, ...);
    void VLog(const char* fmt, va_list args);

    cep::Ref<UartModule>    GetUart() { return m_uart; }
    static Logger* Get() { return s_instance; }

    void SetLogFunc(const LogFunc& logFunc) { m_logFunc = logFunc; }

private:
    static Logger* s_instance;

#    if defined(NILAI_USE_UART)
    cep::Ref<UartModule> m_uart = nullptr;
#    endif
    LogFunc m_logFunc = {};
};
#endif
/*************************************************************************************************/
/* LOG_CRITICAL("Have a wonderful day! :)"); */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
