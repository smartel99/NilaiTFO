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

#if defined(NILAI_USE_LOGGER)
#    if !defined(NILAI_USE_UART)
#        error UART Module must be enabled to use the Logging module!
#    else

/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#        include <cstdarg>
#        include <cstddef>
#        include <cstdint>

#        define GET_CUR_MS( )  (HAL_GetTick( ))
#        define GET_CUR_SEC( ) (GET_CUR_MS( ) / 1000)
#        define GET_CUR_MIN( ) (GET_CUR_SEC( ) / 60)
#        define GET_CUR_H( )   (GET_CUR_MIN( ) / 60)

#        define LOG_HELPER(msg, ...)                                                               \
            do                                                                                     \
            {                                                                                      \
                Logger::Get( ) != nullptr ? Logger::Get( )->Log("[%02i:%02i:%02i.%03i]" msg,       \
                                                                GET_CUR_H( ),                      \
                                                                GET_CUR_MIN( ) % 60,               \
                                                                GET_CUR_SEC( ) % 60,               \
                                                                GET_CUR_MS( ) % 1000,              \
                                                                ##__VA_ARGS__)                     \
                                          : (void)0;                                               \
            } while (0)

/** Prints a debug message */
#        define LOG_DEBUG(msg, ...)    LOG_HELPER("[DEBUG   ]: " msg "\n\r\0", ##__VA_ARGS__)

/** Prints a information message */
#        define LOG_INFO(msg, ...)     LOG_HELPER("[INFO    ]: " msg "\n\r\0", ##__VA_ARGS__)

/** Prints a warning message */
#        define LOG_WARNING(msg, ...)  LOG_HELPER("[WARN    ]: " msg "\n\r\0", ##__VA_ARGS__)

/** Prints an error message */
#        define LOG_ERROR(msg, ...)    LOG_HELPER("[ERROR   ]: " msg "\n\r\0", ##__VA_ARGS__)

/** Prints a critical error message
 */
#        define LOG_CRITICAL(msg, ...) LOG_HELPER("[CRITICAL]: " msg "\n\r\0", ##__VA_ARGS__)

class UartModule;
class Logger
{
public:
    Logger(UartModule* uart);

    ~Logger( );

    void Log(const char* fmt, ...);
    void VLog(const char* fmt, va_list args);

    UartModule*    GetUart( ) { return m_uart; }
    static Logger* Get( ) { return s_instance; }

private:
    static Logger* s_instance;

    UartModule* m_uart = nullptr;
};
#    endif
#endif
/*************************************************************************************************/
/* LOG_CRITICAL("Have a wonderful day! :)"); */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
