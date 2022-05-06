/**
 * @addtogroup esp32Module.cpp
 * @{
 *******************************************************************************
 * @file	esp32Module.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#include "esp32_module.h"

#if defined(NILAI_USE_ESP32)
#    if !defined(NILAI_USE_UART)
#        error The UART module must be enabled in order to use the ESP32 Module
#    endif
#    include "defines/internalConfig.h"
#    include NILAI_HAL_HEADER

#    define ESP_DEBUG(msg, ...) LOG_DEBUG("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ESP_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ESP_WARN(msg, ...)  LOG_WARNING("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#    define ESP_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

EspModule::EspModule(const std::string&     label,
                     UartModule*            uart,
                     uint8_t*               userData,
                     size_t                 dataLen,
                     const CEP_ESP32::Pins& pins)
: m_label(label), m_uart(uart), m_pins(pins)
{
    CEP_ASSERT(uart != nullptr, "UART handle is NULL!");

    // Set TPIN to high to allow the ESP to boot.
    m_pins.tpin.Set(true);

    // Enable the ESP32 in normal boot mode.
    m_pins.enable.Set(false);
    m_pins.boot.Set(true);
    m_pins.enable.Set(true);

    m_userData = userData;
    m_dataLen  = dataLen;

    m_uart->SetExpectedRxLen(512);

    m_uart->SetStartOfFrameSequence("\x01\x02");
    m_uart->SetEndOfFrameSequence("\x03\x04");

    // Wait for ESP32 to be done booting, which takes around 400ms...
    HAL_Delay(450);

    ESP_INFO("Initialized.");
}

/**
 * To pass the POST, the following things must all succeed:
 *  - The `tpout` must be HIGH
 *  - The ESP must respond with "OK".
 * @return True if the POST passes, false otherwise.
 */
bool EspModule::DoPost()
{
    // Make sure that TPOUT is high.
    if (m_pins.tpout.Get() == false)
    {
        ESP_ERROR("Error in POST: TPOUT is LOW!");
        return false;
    }

    SendUserData();

    // Wait for its response.
    size_t start = HAL_GetTick();
    while (m_uart->GetNumberOfWaitingFrames() == 0)
    {
        // Data is only processed in the Run function.
        m_uart->Run();
        if (HAL_GetTick() >= start + EspModule::TIMEOUT)
        {
            // Timed out.
            ESP_ERROR("Error in POST: No response from ESP!");
            return false;
        }
    }

    // We got the response, check it.
    CEP_UART::Frame resp = m_uart->Receive();

    if (resp != "OK")
    {
        ESP_ERROR("Error in POST: Invalid response from ESP! ('%s')", resp.ToStr().c_str());
        return false;
    }

    // POST OK!
    ESP_INFO("POST OK");

    return true;
}

void EspModule::Run()
{
    // If TPOUT is LOW, there's a problem, reset the ESP.
    if (m_pins.tpout.Get() == false)
    {
        ESP_WARN("TPOUT is LOW, resetting!");
        m_pins.enable.Set(false);
        HAL_Delay(1);
        m_pins.enable.Set(true);

        SendUserData();

        // Wait for its response.
        size_t start = HAL_GetTick();
        while (m_uart->GetNumberOfWaitingFrames() == 0)
        {
            if (HAL_GetTick() >= start + EspModule::TIMEOUT)
            {
                // Timed out.
                ESP_ERROR("No response from ESP!");
                return;
            }
        }

        // We got the response, check it.
        CEP_UART::Frame resp = m_uart->Receive();

        if (resp != "OK")
        {
            ESP_ERROR("Invalid response from ESP! ('%s')", resp.data);
            return;
        }
    }
}

void EspModule::Enable()
{
    m_pins.enable.Set(true);
    m_enabled = true;
}

void EspModule::Disable()
{
    m_pins.enable.Set(false);
    m_enabled = false;
}

void EspModule::SetBootMode(CEP_ESP32::BootMode mode)
{
    switch (mode)
    {
        case CEP_ESP32::BootMode::Normal: m_pins.boot.Set(true); break;
        case CEP_ESP32::BootMode::Bootloader: m_pins.boot.Set(false); break;
        default: CEP_ASSERT(false, "Invalid boot mode"); break;
    }
}

bool EspModule::ProgramEsp(const std::string& filepath)
{
#    if !defined(NILAI_USE_FILESYSTEM)
    UNUSED(filepath);
    CEP_ASSERT(false, "The SD module must be enabled to use this function");
    return false;
#    else
    UNUSED(filepath);
#        warning Implement EspModule::ProgramEsp!
    return false;
#    endif
}

void EspModule::Transmit(const char* msg, size_t len)
{
    m_uart->Transmit(msg, len);
}

void EspModule::Transmit(const std::string& msg)
{
    m_uart->Transmit(msg);
}

void EspModule::Transmit(const std::vector<uint8_t>& msg)
{
    m_uart->Transmit(msg);
}

size_t EspModule::GetNumberOfWaitingFrames() const
{
    return m_uart->GetNumberOfWaitingFrames();
}

CEP_UART::Frame EspModule::Receive()
{
    return m_uart->Receive();
}

void EspModule::SetExpectedRxLen(size_t len)
{
    m_uart->SetExpectedRxLen(len);
}

void EspModule::ClearExpectedRxLen()
{
    m_uart->ClearExpectedRxLen();
}

void EspModule::SetFrameReceiveCpltCallback(const std::function<void()>& cb)
{
    m_uart->SetFrameReceiveCpltCallback(cb);
}

void EspModule::ClearFrameReceiveCpltCallback()
{
    m_uart->ClearFrameReceiveCpltCallback();
}

void EspModule::SetStartOfFrameSequence(const char* sof, size_t len)
{
    m_uart->SetStartOfFrameSequence(const_cast<uint8_t*>((const uint8_t*)sof), len);
}

void EspModule::SetStartOfFrameSequence(const std::string& sof)
{
    m_uart->SetStartOfFrameSequence(sof);
}

void EspModule::SetStartOfFrameSequence(const std::vector<uint8_t>& sof)
{
    m_uart->SetStartOfFrameSequence(sof);
}

void EspModule::ClearStartOfFrameSequence()
{
    m_uart->ClearStartOfFrameSequence();
}

void EspModule::SetEndOfFrameSequence(const char* eof, size_t len)
{
    m_uart->SetEndOfFrameSequence(const_cast<uint8_t*>((const uint8_t*)eof), len);
}

void EspModule::SetEndOfFrameSequence(const std::string& eof)
{
    m_uart->SetEndOfFrameSequence(eof);
}

void EspModule::SetEndOfFrameSequence(const std::vector<uint8_t>& eof)
{
    m_uart->SetEndOfFrameSequence(eof);
}

void EspModule::ClearEndOfFrameSequence()
{
    m_uart->ClearEndOfFrameSequence();
}

void EspModule::SendUserData()
{
    m_uart->Transmit((const char*)&m_dataLen,
                     1);    // Report the data size 256 is max length for now
    if (m_dataLen != 0)
        m_uart->Transmit((const char*)m_userData, m_dataLen);    // Send the user data
}

#endif
