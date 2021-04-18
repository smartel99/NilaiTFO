/**
 * @addtogroup logger.cpp
 * @{
 *******************************************************************************
 * @file	logger.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Oct 13, 2020
 *******************************************************************************
 */

#if defined(NILAI_USE_LOGGER)
#include "logger.hpp"
#include "defines/macros.hpp"
#if defined(NILAI_USE_UART)
#include "drivers/uartModule.hpp"
#endif

#include <cstdarg>
#include <cstdio>

Logger* Logger::s_instance = nullptr;

#if defined(NILAI_USE_UART)
Logger::Logger(UartModule* uart, const std::function<void(const char*, size_t)> logFunc)
{
    CEP_ASSERT(s_instance == nullptr, "Can only have one instance of Logger!");
    s_instance = this;
    m_uart     = uart;
    m_logFunc  = logFunc;
}
#else

Logger::Logger(const std::function<void(const char*, size_t)> logFunc)
{
    CEP_ASSERT(s_instance == nullptr, "Can only have one instance of Logger!");
    s_instance = this;
    m_logFunc  = logFunc;
}
#endif

Logger::~Logger() = default;

void Logger::Log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    VLog(fmt, args);

    va_end(args);
}

void Logger::VLog(const char* fmt, va_list args)
{
    static char buff[1024] = {0};

    size_t s = vsnprintf(buff, sizeof_array(buff), fmt, args);

    CEP_ASSERT(s < sizeof_array(buff), "vsnprintf error!");
#if defined(NILAI_USE_UART)
    if (m_uart != nullptr)
    {
        m_uart->Transmit(buff, s);
    }
#endif
    if (m_logFunc)
    {
        m_logFunc(buff, s);
    }
}

#endif
