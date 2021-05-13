/**
 * @addtogroup rn2903Module.cpp
 * @{
 *******************************************************************************
 * @file	rn2903Module.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */
#if defined(NILAI_USE_RN2903)
#include "rn2903Module.h"

#include "shared/defines/macros.hpp"

#define RN2903_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#define RN2903_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

Rn2903Module::Rn2903Module(const std::string& label, UartModule* uart)
: m_label(label), m_uart(uart)
{
    CEP_ASSERT(m_uart != nullptr, "[RN2903]: UART Handle is null!");

    RN2903_INFO("Initialized.");
}

bool Rn2903Module::DoPost()
{
#warning Do RN2903 POST
    return true;
}

void Rn2903Module::Run()
{
}

void Rn2903Module::Transmit(const std::string& pkt)
{
}

#endif
