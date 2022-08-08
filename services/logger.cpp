/**
 * @file	logger.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Oct 13, 2020
 *******************************************************************************
 */

#if defined(NILAI_USE_LOGGER) && !defined(NILAI_TEST)
#    include "logger.h"
#    include "../defines/macros.h"
#    if defined(NILAI_USE_UART)
#        include "../drivers/uart_module.h"
#    endif

#    include <cstdarg>
#    include <cstdio>
#    include <utility>

namespace Nilai::Services
{
Logger* Logger::s_instance = nullptr;

#    if defined(NILAI_USE_UART)
Logger::Logger(Ref<Drivers::UartModule> uart, const LogFunc& logFunc)
{
    NILAI_ASSERT(s_instance == nullptr, "Can only have one instance of Logger!");
    NILAI_ASSERT(uart != nullptr, "Uart handle is null!");
    s_instance = this;
    m_uart     = std::move(uart);
    if (logFunc)
    {
        m_logFunc = logFunc;
    }
}
#    else
Logger::Logger(const LogFunc& logFunc)
{
    CEP_ASSERT(s_instance == nullptr, "Can only have one instance of Logger!");
    s_instance = this;
    if (logFunc)
    {
        m_logFunc = logFunc;
    }
}
#    endif

Logger::~Logger()
{
    s_instance = nullptr;
}

void Logger::Log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    VLog(fmt, args);

    va_end(args);
}

void Logger::VLog(const char* fmt, va_list args)
{
    static char buff[1024] = {};

    size_t s = vsnprintf(buff, SIZEOF_ARRAY(buff), fmt, args);

    NILAI_ASSERT(s < SIZEOF_ARRAY(buff), "vsnprintf error!");
#    if defined(NILAI_USE_UART)
    m_uart->Transmit(buff, s);
#    endif
    m_logFunc(buff, s);
}

void Logger::SetLogFunc(const LogFunc& logFunc)
{
    if (logFunc)
    {
        m_logFunc = logFunc;
    }
}
}    // namespace Nilai::Services
#endif
