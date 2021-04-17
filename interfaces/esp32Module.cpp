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

#include "esp32Module.h"

#if defined(NILAI_USE_ESP32) && 0
#if !defined(NILAI_USE_UART)
#error The UART module must be enabled in order to use the ESP32 Module
#endif
#include "defines/internalConfig.h"
#include NILAI_HAL_HEADER

#define ESP_INFO(msg, ...)  LOG_INFO("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)
#define ESP_ERROR(msg, ...) LOG_ERROR("[%s]: " msg, m_label.c_str(), ##__VA_ARGS__)

#define CHECK_STATUS_AND_CALLBACK(bit)                                                             \
    do                                                                                             \
    {                                                                                              \
        if (m_status.state & (bit))                                                                \
        {                                                                                          \
            ESP_INFO("%s event received", GetStateString((bit)));                                  \
            if (m_callbacks[(bit)])                                                                \
            {                                                                                      \
                m_callbacks[(bit)]();                                                              \
            }                                                                                      \
        }                                                                                          \
    } while (0)

static const char* GetStateString(CEP_ESP32::State state);
static std::string GetCommandString(CEP_ESP32::Command cmd);

EspModule::EspModule(const std::string& label, UartModule* uart, const CEP_ESP32::Pins& pins)
: m_label(label), m_uart(uart), m_pins(pins)
{
    CEP_ASSERT(uart != nullptr, "UART handle is NULL!");

    m_pins.enable.Set(false);
    m_pins.boot.Set(true);
    m_pins.enable(true);

    InitializeCallbacks();

    m_uart->SetStartOfFrameSequence("\x55\xAA");
    m_uart->SetEndOfFrameSequence("\x00");

    ESP_INFO("Initialized.");
}

/**
 * To pass the POST, the following things must all succeed:
 *  - The `tpin` must be HIGH
 *  - The `GetStatus` command indicates that there are no errors on the ESP32.
 * @return True if the POST passes, false otherwise.
 */
bool EspModule::DoPost()
{
    if (m_pins.tpout.Get() == false)
    {
        ESP_ERROR("Error in POST: TPOUT is LOW!");
        return false;
    }

    const CEP_ESP32::Status& status = RefreshStatus();
    if (status.state != CEP_ESP32::State::NoErrors)
    {
        ESP_ERROR("Error in POST: Status is %s", GetStateString(status.state));
        return false;
    }

    return true;
}

void EspModule::Run()
{
    // If the ESP32 has something to tell us, it will set the TPOUT pin to LOW.
    if (m_pins.tpout.Get() == false)
    {
        // Go read that status.
        RefreshStatus();

        CheckStatusAndCallCallback();
    }
}

const CEP_ESP32::Status& EspModule::RefreshStatus()
{
}

void EspModule::SetEventCallback(CEP_ESP32::State event, const std::function<void()>& cb)
{
    CEP_ASSERT(m_callbacks.find(event) != m_callbacks.end(), "Invalid event!");

    m_callbacks[event] = cb;
}

void EspModule::ClearEventCallback(CEP_ESP32::State event)
{
    CEP_ASSERT(m_callbacks.find(event) != m_callbacks.end(), "Invalid event!");

    m_callbacks[event] = {};
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
        case CEP_ESP32::BootMode::Normal:
            m_pins.boot.Set(true);
            break;
        case CEP_ESP32::BootMode::Bootloader:
            m_pins.boot.Set(false);
            break;
        default:
            CEP_ASSERT(false, "Invalid boot mode");
            break;
    }
}

bool EspModule::ProgramEsp(const std::string& filepath)
{
#if !defined(NILAI_USE_SD)
    CEP_ASSERT(false, "The SD module must be enabled to use this function");
#endif
#warning Implement EspModule::ProgramEsp!
}

const CEP_ESP32::Status& EspModule::RefreshStatus()
{
    m_uart->Transmit(GetCommandString(CEP_ESP32::Command::GetStatus));
}

void EspModule::InitializeCallbacks()
{
    using namespace CEP_ESP32;
    m_callbacks = {{State::Transmitting, {}},
                   {State::TxComplete, {}},
                   {State::Receiving, {}},
                   {State::DataAwaiting, {}},
                   {State::Scanning, {}},
                   {State::ScanComplete, {}},
                   {State::FoundDevices, {}},
                   {State::Broadcasting, {}},
                   {State::BroadcastComplete, {}},
                   {State::ConnReqReceived, {}},
                   {State::Connecting, {}},
                   {State::Connected, {}},
                   {State::TxError, {}},
                   {State::RxError, {}},
                   {State::ScanError, {}},
                   {State::BroadcastError, {}},
                   {State::ConnError, {}},
                   {State::MiscError, {}}};
}

void EspModule::CheckStatusAndCallCallback()
{
    using namespace CEP_ESP32;
    CHECK_STATUS_AND_CALLBACK(State::Transmitting);
    CHECK_STATUS_AND_CALLBACK(State::TxComplete);
    CHECK_STATUS_AND_CALLBACK(State::Receiving);
    CHECK_STATUS_AND_CALLBACK(State::DataAwaiting);
    CHECK_STATUS_AND_CALLBACK(State::Scanning);
    CHECK_STATUS_AND_CALLBACK(State::ScanComplete);
    CHECK_STATUS_AND_CALLBACK(State::FoundDevices);
    CHECK_STATUS_AND_CALLBACK(State::Broadcasting);
    CHECK_STATUS_AND_CALLBACK(State::BroadcastComplete);
    CHECK_STATUS_AND_CALLBACK(State::ConnReqReceived);
    CHECK_STATUS_AND_CALLBACK(State::Connecting);
    CHECK_STATUS_AND_CALLBACK(State::Connected);
    CHECK_STATUS_AND_CALLBACK(State::TxError);
    CHECK_STATUS_AND_CALLBACK(State::RxError);
    CHECK_STATUS_AND_CALLBACK(State::ScanError);
    CHECK_STATUS_AND_CALLBACK(State::BroadcastError);
    CHECK_STATUS_AND_CALLBACK(State::ConnError);
    CHECK_STATUS_AND_CALLBACK(State::MiscError);
}

const char* GetStateString(CEP_ESP32::State state)
{
    switch (state)
    {
        case CEP_ESP32::State::RxError:
            return "RxError";
        case CEP_ESP32::State::Scanning:
            return "Scanning";
        case CEP_ESP32::State::NoErrors:
            return "NoErrors";
        case CEP_ESP32::State::TxError:
            return "TxError";
        case CEP_ESP32::State::Connected:
            return "Connected";
        case CEP_ESP32::State::Standby:
            return "Standby";
        case CEP_ESP32::State::Connecting:
            return "Connecting";
        case CEP_ESP32::State::Broadcasting:
            return "Broadcasting";
        case CEP_ESP32::State::Transmitting:
            return "Transmitting";
        case CEP_ESP32::State::MiscError:
            return "MiscError";
        case CEP_ESP32::State::ConnError:
            return "ConnError";
        case CEP_ESP32::State::BroadcastError:
            return "BroadcastError";
        case CEP_ESP32::State::ScanError:
            return "ScanError";
        case CEP_ESP32::State::Receiving:
            return "Receiving";
        default:
            return "Unknown Error";
    }
}

std::string GetCommandString(CEP_ESP32::Command cmd)
{
    switch (cmd)
    {
        case CEP_ESP32::Command::GetStatus:
            return std::string{"\x01"};
        case CEP_ESP32::Command::StartScanning:
            return std::string{"\x02"};
        case CEP_ESP32::Command::StopScanning:
            return std::string{"\x03"};
        case CEP_ESP32::Command::StartBroadcasting:
            return std::string{"\x04"};
        case CEP_ESP32::Command::StopBroadcasting:
            return std::string{"\x05"};
        case CEP_ESP32::Command::RequestConnection:
            return std::string{"\x06"};
        case CEP_ESP32::Command::ListDevices:
            return std::string{"\x07"};
        case CEP_ESP32::Command::Connect:
            return std::string{"\x08"};
        case CEP_ESP32::Command::Disconnect:
            return std::string{"\x09"};
        case CEP_ESP32::Command::SendPacket:
            return std::string{"\x0A"};
        case CEP_ESP32::Command::ReceivePacket:
            return std::string{"\x0B"};
        case CEP_ESP32::Command::Reset:
            return std::string{"\x0C"};
        case CEP_ESP32::Command::GetDeviceAddress:
            return std::string("\x0D");
        case CEP_ESP32::Command::GetErrorMessages:
            return std::string{"\xFE"};
        case CEP_ESP32::Command::Error:
            return std::string{"\xFF"};
        default:
            CEP_ASSERT(false, "Invalid command");
            break;
    }
}

#endif
