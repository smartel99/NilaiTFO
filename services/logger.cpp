/**
 * @addtogroup  defines
 * @{
 * @addtogroup  logger
 * @{
 *
 * @file        Logger.cpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/06/29 - 11:52
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"


/*************************************************************************************************/
/* Module instantiation ------------------------------------------------------------------------ */
Log Log::s_instance {};


/*************************************************************************************************/
/* Public member functions definitions --------------------------------------------------------- */
void Log::Init()
{
    s_instance.m_Logger = spdlog::stdout_logger_mt("tornatest");
    
    /* %n : Name of the logger
     * %L : Short level name (C for critical, W for warn...)
     * %v : Actual messge to print
     * \r : Carriage return
     * (There is an implicit \n)
     * Example: [Slave][D] - Hello World!\r\n
     */
    s_instance.m_Logger->set_pattern("[%n][%L] - %v\r");
    s_instance.m_Logger->disable_backtrace();


    using SinkType                = spdlog::sinks::stdout_sink_base;
    spdlog::sinks::sink* baseSink = s_instance.m_Logger->sinks()[0].get();
    SinkType*            sink     = dynamic_cast<SinkType*>(baseSink);

    sink->set_custom_logger(&CustomLogHandler);
    s_instance.m_Logger->set_error_handler(&CustomErrorHandler);
    
    s_instance.Enable();
}


/*************************************************************************************************/
/* Private member functions definitions -------------------------------------------------------- */
void Log::CustomErrorHandler(const std::string& msg)
{
    uint8_t* data = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(msg.data()));

    HAL_UART_Transmit(&huart1, data, msg.size(), 100);
}

void Log::CustomLogHandler(const spdlog::memory_buf_t& formattedMsg)
{
    uint8_t* data = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(formattedMsg.data()));
    HAL_UART_Transmit(&huart1, data, formattedMsg.size(), 100);
}


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
