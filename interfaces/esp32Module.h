/**
 * @addtogroup esp32Module.h
 * @{
 *******************************************************************************
 * @file	esp32Module.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#ifndef ESP32MODULE_H_
#define ESP32MODULE_H_

/***********************************************/
/* Includes */
#if defined(NILAI_USE_ESP32) && 0
#if !defined(NILAI_USE_UART)
#error The UART module must be enabled in order to use the ESP32 Module
#endif
#include "defines/internalConfig.h"
#include NILAI_HAL_HEADER
#include "defines/module.hpp"
#include "defines/pin.h"

#include "drivers/uartModule.hpp"

#include <array>
#include <map>

/***********************************************/
/* Defines */
namespace CEP_ESP32
{
/**
 * List of all the commands that can be used with the ESP32.
 */
enum class Command
{
    GetStatus         = 0x01,    //!< GetStatus
    StartScanning     = 0x02,    //!< StartScanning
    StopScanning      = 0x03,    //!< StopScanning
    StartBroadcasting = 0x04,    //!< StartBroadcasting
    StopBroadcasting  = 0x05,    //!< StopBroadcasting
    RequestConnection = 0x06,    //!< RequestConnection
    ListDevices       = 0x07,    //!< ListDevices
    Connect           = 0x08,    //!< Connect
    Disconnect        = 0x09,    //!< Disconnect
    SendPacket        = 0x0A,    //!< SendPacket
    ReceivePacket     = 0x0B,    //!< ReceivePacket
    Reset             = 0x0C,    //!< Resets the ESP32.
    GetDeviceAddress  = 0x0D,    //!< Gets the MAC address of the connected device.
    GetErrorMessages  = 0xFE,    //!< GetErrorMessages
    Error             = 0xFF     //!< Error
};

/**
 * All the possible states that can be taken by the ESP32.
 */
enum class State : uint32_t
{
    NoErrors     = 1 << 0,    //!< No errors
    Standby      = 1 << 1,    //!< Standby
    Transmitting = 1 << 2,    //!< The module is currently transmitting a packet.
    TxComplete   = 1 << 3,    //!< The module has finished a transmission.
    Receiving    = 1 << 3,    //!< The module is currently receiving data from a connected device.
    DataAwaiting = 1 << 4,    //!< The module has received data that has not been read yet.
    Scanning     = 1 << 5,    //!< The module is currently scanning for nearby devices.
    ScanComplete = 1 << 6,    //!< The scanning period of the module is complete.
    FoundDevices = 1 << 7,    //!< The module has found at least one device during its scan.
    Broadcasting = 1 << 8,    //!< The module is currently broadcasting its MAC address.
    BroadcastComplete = 1 << 9,    //!< The broadcast period of the module is complete.
    ConnReqReceived =
      1 << 7,    //!< The module has received a connection request that has not been reviewed yet.
    Connecting     = 1 << 9,     //!< The module is currently attempting to connect to a device.
    Connected      = 1 << 10,    //!< The module is currently connected to a device.
    TxError        = 1 << 11,    //!< An error occurred during the transmission of a packet.
    RxError        = 1 << 12,    //!< An error occurred during the reception of a packet.
    ScanError      = 1 << 13,    //!< An error occurred during scanning.
    BroadcastError = 1 << 14,    //!< An error occurred during broadcasting.
    ConnError      = 1 << 15,    //!< A connection error occurred.
    MiscError      = 1 << 16,    //!< An unknown error occurred.
};

/**
 * Boot modes for the ESP32.
 */
enum class BootMode
{
    Bootloader = 0,    //!< Bootloader
    Normal     = 1,    //!< Normal
};

/**
 * Struct containing status information of the ESP32.
 */
struct Status
{
    State state       = State::NoErrors;    //!< Status of the ESP32.
    bool  isConnected = false;    //!< True if the ESP32 is connected to a bluetooth device.
    std::array<uint8_t, 6> bdAddress = {0};    //!< MAC address of the connected bluetooth device.
};

/**
 * Contains all the control pins of the ESP32 module.
 */
struct Pins
{
    Pin enable = {};    //!< Enable pin of the ESP32. When high, the ESP32 is enabled.
    Pin boot   = {};    /**< Boot selection pin.
                         *   When high, normal boot.
                         *   When low, runs the bootloader.
                         */
    Pin tpout = {};     //!< Heartbeat pin from the ESP32 to the STM32.
    Pin tpin  = {};     //!< Debug signal from the STM32 to the ESP32, currently not used.
};
}    // namespace CEP_ESP32

/***********************************************/
/* Function declarations */

class EspModule : public cep::Module
{
public:
    EspModule(const std::string& label, UartModule* uart, const CEP_ESP32::Pins& pins);
    virtual ~EspModule() override = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const { return m_label; }

    void SetEventCallback(CEP_ESP32::State event, const std::function<void()>& cb);
    void ClearEventCallback(CEP_ESP32::State event);
    /**
     * Enables the ESP32 through the enable pin.
     */
    void Enable();

    /**
     * Disables the ESP32 through the enable pin.
     */
    void Disable();
    /**
     * Checks if the ESP32 is currently enabled.
     * @return True if the ESP32 is enabled, false otherwise.
     */
    bool IsEnabled() const { return m_enabled; }

    /**
     * Sets the boot mode of the ESP32.
     *
     * For this to have any effects on the ESP32, you must follow these steps:
     *  - Disable the ESP32
     *  - Set the boot mode
     *  - Enable the ESP32
     * @param mode The desired boot mode.
     */
    void SetBootMode(CEP_ESP32::BootMode mode);

    /**
     * Programs the ESP32 with the specified binary file located on the SD card.
     *
     * If the file cannot be found on the SD card, nothing is done.
     *
     * If NILAI_USE_SD is not defined, this function will always return false.
     *
     * To program the ESP32, you must follow these steps:
     *  - Disable the ESP32
     *  - Enter the ESP32's bootloader
     *  - Enable the ESP32
     *  - Program it
     *  - Disable the ESP32
     *  - Set the boot mode to "normal"
     *  - Enable the ESP32
     *
     * @param filepath The path of the firmware to use, defaulting to "esp32.bin"
     * @return True if the programming was successful, false otherwise.
     */
    bool ProgramEsp(const std::string& filepath = "esp32.bin");

    /**
     * Gets the cached status of the module.
     * @return The status
     */
    const CEP_ESP32::Status& GetStatus() const { return m_status; }

    /**
     * Asks the ESP32 for its status.
     * @return The status.
     */
    const CEP_ESP32::Status& RefreshStatus();

    /**
     * Get a list of the ESP32's error messages that matches the filter.
     *
     * @param filter Filter mask for the messages to receive, corresponding to @ref CEP_ESP32::State
     * @param limit The maximum number of messages to receive. Set to 0 to disable that limit.
     * @return An array containing all the received strings.
     */
    std::vector<std::string> GetErrorMessages(uint16_t filter = 0xFFFF, uint8_t limit = 0);

    /**
     * Tells the ESP32 to start scanning for all nearby bluetooth devices.
     *
     * If `desiredAddress` is not 0 and the ESP32 finds the corresponding device, it will
     * automatically try to establish connection with that device.
     *
     * If the ESP32 was broadcasting its MAC address when receiving this command, it will stop doing
     * so in order to scan for devices.
     *
     * @param timeoutS The amount of time, in seconds, after which the ESP32 will stop scanning for
     * devices.
     * @param desiredAddress The MAC address of the device that, if discovered, the ESP32 should
     *      automatically connect to. If set to 0, the ESP32 will not try to connect to any
     *      discovered devices.
     * @return True if the ESP32 has successfully started scanning for devices, false otherwise.
     */
    bool StartScanning(uint16_t timeoutS, uint64_t desiredAddress = 0);

    /**
     * Tells the ESP32 to stop scanning for nearby bluetooth devices.
     * @return The number of devices that were found by the ESP32.
     */
    uint16_t StopScanning();

    /**
     * Checks if the ESP32 is currently scanning for nearby devices.
     * @return True if the ESP32 is currently scanning for devices, false otherwise.
     */
    bool IsScanning() const { return (m_status.state & CEP_ESP32::State::Scanning) ? true : false; }

    /**
     * Asks the ESP32 to send a list of all the MAC addresses that were discovered when scanning.
     * @return The list of all the discovered devices.
     */
    std::vector<uint64_t> ListDevices();

    /**
     * Tells the ESP32 to start broadcasting its MAC address to all nearby devices.
     * If `autoAccept` is set to true, the ESP32 will accept all connection request received,
     * *even if the device is blacklisted!*
     *
     * If the ESP32 was scanning for devices when receiving this command, it will stop doing so in
     * order to broadcast its MAC address.
     *
     * @param timeoutS Time, in seconds, after which the ESP32 should stop broadcasting.
     *          Set to 0 to disable the timeout.
     * @param autoAccept Set to true to let the ESP32 automatically accept any connection request
     *          received.
     * @return True if the ESP32 has successfully started broadcasting its MAC address, false
     * otherwise.
     */
    bool StartBroadcasting(uint8_t timeoutS, bool autoAccept);

    /**
     * Tells the ESP32 to stop broadcasting tis MAC address.
     * @return True if the ESP32 has successfully stopped broadcasting its MAC address, false
     *          otherwise.
     */
    bool StopBroadcasting();

    /**
     * Checks if the ESP32 is currently broadcasting its MAC address.
     * @return True if the ESP32 is broadcasting its address, false otherwise.
     */
    bool IsBroadcasting() const;

    /**
     * Checks if the ESP32 has received a connection request that it didn't automatically accept.
     * @return True if a request was received, false otherwise.
     */
    bool HasReceivedConnRqst();

    /**
     * Gets the address of the device that made the connection request.
     *
     * If no devices made a connection request, 0 will be returned instead.
     *
     * @return The MAC address of the requesting device.
     */
    uint64_t GetConnRqstAddress();

    /**
     * Checks if the specified address is on the system's blacklist.
     * @param address The address to check.
     * @return True if the address is black listed, false otherwise.
     */
    bool IsAddressBlackListed(uint64_t address);

    /**
     * Replies to a connection request. If the request is accepted, the ESP32 will establish
     * connection with the device. Otherwise, the connection will not be established.
     *
     * @param accept Pretty self-explanatory. Set to true to accept the connection request.
     */
    void ReplyToConnRqst(bool accept);

    /**
     * Tells the ESP32 to try to connect to the specified MAC address.
     * The ESP32 will attempt to establish the connection with the device `attempts` number of
     * times. If `attempts` is 0, the ESP32 will not stop trying unless it succeeds.
     *
     * @param address The MAC address of the device to connect to.
     * @param attempts The maximum number of attempts that will be made to connect to the device.
     *          Set to 0 to disable the limit of attempts.
     */
    void ConnectToDevice(uint64_t address, uint8_t attempts = 1);

    /**
     * Tells the ESP32 to gracefully break the connection with a bluetooth device.
     */
    void DisconnectFromDevice();

    /**
     * Checks if a device is connected to the ESP32 via bluetooth.
     *
     * @return True if a device is connected, false otherwise.
     */
    bool IsConnected() const;

    /**
     * Sends up to 65533 bytes of data to the device connected via bluetooth.
     *
     * If there are no devices connected to the ESP32, the packet will not be sent.
     * @param packet A string containing the data to be sent.
     * @return True if the packet is being sent by the ESP32, false otherwise.
     */
    bool SendPacket(const std::string& packet);

    /**
     * Gets a packet of data that has been received from the connected device.
     *
     * @return A string containing the packet.
     */
    std::string ReceivePacket();

    /**
     * Queries the ESP32 for the number of received packets that have not been read yet.
     *
     * @return The number of packets that have not been read yet.
     */
    size_t GetNumberOfReceivedPackets();

private:
    std::string     m_label   = "";
    UartModule*     m_uart    = nullptr;
    CEP_ESP32::Pins m_pins    = {};
    bool            m_enabled = false;

    std::map<CEP_ESP32::State, std::function<void()>> m_callbacks = {};

    CEP_ESP32::Status m_status = {};

private:
    void InitializeCallbacks();
    void CheckStatusAndCallCallback();
};

/**
 * @}
 */
/* END OF FILE */
#endif
#endif /* ESP32MODULE_H_ */
