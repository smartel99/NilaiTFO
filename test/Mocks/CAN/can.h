#include <array>
#include <cstdint>

/**
 * @brief  CAN Rx message header structure definition
 */
typedef struct {
    uint32_t StdId; /*!< Specifies the standard identifier.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF. */

    uint32_t ExtId; /*!< Specifies the extended identifier.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF. */

    uint32_t IDE; /*!< Specifies the type of identifier for the message that will be transmitted.
                       This parameter can be a value of @ref CAN_identifier_type */

    uint32_t RTR; /*!< Specifies the type of frame for the message that will be transmitted.
                       This parameter can be a value of @ref CAN_remote_transmission_request */

    uint32_t DLC; /*!< Specifies the length of the frame that will be transmitted.
                       This parameter must be a number between Min_Data = 0 and Max_Data = 8. */

    uint32_t Timestamp; /*!< Specifies the timestamp counter value captured on start of frame reception.
                            @note: Time Triggered Communication Mode must be enabled.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 0xFFFF. */

    uint32_t FilterMatchIndex; /*!< Specifies the index of matching acceptance filter element.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF. */

} CAN_RxHeaderTypeDef;

namespace CEP_CAN {
struct Frame {
    CAN_RxHeaderTypeDef    frame;
    std::array<uint8_t, 8> data;
    uint32_t               timestamp = 0;

    bool operator==(const Frame& other) {
        // If both have the same extended ID:
        if (frame.ExtId == other.frame.ExtId) {
            // For each byte of data:
            for (size_t i = 0; i < data.size(); i++) {
                // If the two bytes are not the same:
                if (data[i] != other.data[i]) {
                    // Frames are different.
                    return false;
                }
            }
            // Everything is identical, true.
            return true;
        }
        // ID don't match
        return false;
    }

    bool operator!=(const Frame& other) {
        return !(*this == other);
    }
};
}    // Namespace CEP_CAN