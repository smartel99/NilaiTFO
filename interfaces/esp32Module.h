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
#if defined(NILAI_USE_ESP32)
#if !defined(NILAI_USE_UART)
#error The UART module must be enabled to use the ESP32 Module
#endif
#include "defines/internalConfig.h"
#include NILAI_HAL_HEADER
#include "defines/module.hpp"
#include "defines/pin.h"
#include "drivers/uart/it.hpp"

#include <array>
#include <map>

/***********************************************/
/* Defines */
namespace CEP_ESP32 {
/**
 * Boot modes for the ESP32.
 */
enum class BootMode
{
    Bootloader = 0,    //!< Bootloader
    Normal     = 1,    //!< Normal
};

/**
 * Contains all the control pins of the ESP32 module.
 */
struct Pins {
    cep::Pin enable = {};    //!< Enable pin of the ESP32. When high, the ESP32 is enabled.
    cep::Pin boot   = {};    /**< Boot selection pin.
                              *   When high, normal boot.
                              *   When low, runs the bootloader.
                              */
    cep::Pin tpout = {};     //!< Heartbeat pin from the ESP32 to the STM32.
    cep::Pin tpin  = {};     //!< Debug signal from the STM32 to the ESP32, currently not used.
};
}    // namespace CEP_ESP32

/***********************************************/
/* Function declarations */

class EspModule : public cep::Module {
  public:
    EspModule(
      const std::string&     label,
      UartModuleIt*          uart,
      uint8_t*               userData,
      size_t                 dataLen,
      const CEP_ESP32::Pins& pins);
    virtual ~EspModule() override = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const {
        return m_label;
    }

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
    bool IsEnabled() const {
        return m_enabled;
    }

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
     * Sends a packet of data.
     * @param msg The packet to send
     * @param len The number of bytes to send.
     */
    void Transmit(const char* msg, size_t len);

    /**
     * Sends a packet of data.
     * @param msg The packet to send
     */
    void Transmit(const std::string& msg);

    /**
     * Sends a packet of data.
     * @param msg The packet to send
     */
    void Transmit(const std::vector<uint8_t>& msg);

    /**
     * Gets the number of frames that have not been received yet.
     * @return The number of packets waiting.
     */
    size_t GetNumberOfWaitingFrames() const;

    /**
     * Receives a frame of data.
     * @return The received frame
     */
    CEP_UART::Frame Receive();

    /**
     * Sets the number of bytes that are expected to be received.
     * @param len The number of bytes.
     */
    void SetExpectedRxLen(size_t len);

    /**
     * Resets the number of bytes expected back to zero.
     */
    void ClearExpectedRxLen();

    /**
     * Sets the function that will be called upon receiving a frame.
     * @param cb The function to call
     */
    void SetFrameReceiveCpltCallback(const std::function<void()>& cb);

    /**
     * Removes the frame received callback.
     */
    void ClearFrameReceiveCpltCallback();

    /**
     * Sets the start of frame identifier sequence.
     * @param sof The start of frame sequence
     * @param len The number of bytes contained in the sequence
     */
    void SetStartOfFrameSequence(const char* sof, size_t len);
    void SetStartOfFrameSequence(const std::string& sof);
    void SetStartOfFrameSequence(const std::vector<uint8_t>& sof);

    /**
     * Removes the start of frame sequence.
     */
    void ClearStartOfFrameSequence();

    /**
     * Sets the end of frame identifier sequence.
     * @param eof The end of frame sequence
     * @param len The number of bytes contained in the sequence
     */
    void SetEndOfFrameSequence(const char* eof, size_t len);
    void SetEndOfFrameSequence(const std::string& eof);
    void SetEndOfFrameSequence(const std::vector<uint8_t>& eof);

    /**
     * Removes the end of frame sequence.
     */
    void ClearEndOfFrameSequence();

  private:
    std::string     m_label    = "";
    UartModuleIt*   m_uart     = nullptr;
    uint8_t*        m_userData = nullptr;
    size_t          m_dataLen  = 0;
    CEP_ESP32::Pins m_pins     = {};
    bool            m_enabled  = false;

    static constexpr size_t TIMEOUT = 500;

    void SendUserData();
};

/**
 * @}
 */
/* END OF FILE */
#endif
#endif /* ESP32MODULE_H_ */
