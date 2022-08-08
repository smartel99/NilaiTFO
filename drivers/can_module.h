/**
 * @file        can_module.h
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/08/10  -  13:27
 *
 * @brief       CAN communication module
 */
#ifndef NILAI_CAN_MODULE_HPP
#    define NILAI_CAN_MODULE_HPP
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#    if !defined(NILAI_TEST)
#        if defined(NILAI_USE_CAN)
#            include "../defines/internal_config.h"
#            include NILAI_HAL_HEADER
#            if defined(HAL_CAN_MODULE_ENABLED)
#                include "../defines/macros.h"
#                include "../defines/misc.h"
#                include "../defines/module.h"

#                include "CAN/enums.h"
#                include "CAN/structs.h"

#                include <array>
#                include <cstdint>
#                include <functional>
#                include <map>
#                include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

#                if USE_HAL_CAN_REGISTER_CALLBACKS == 1
#                    define NILAI_CAN_REGISTER_CALLBACKS
#                endif

/*************************************************************************************************/
/* Enumerated Types
 * ----------------------------------------------------------------------------
 */
namespace Nilai::Drivers
{
class CanModule : public Module
{
public:
    using Callback = std::function<void(const CanModule&)>;

public:
    CanModule(CAN_HandleTypeDef* handle, std::string label);
    ~CanModule() override;

    [[nodiscard]] bool               DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    void Reset();

    void ConfigureFilter(const Can::FilterConfiguration& config);

    [[nodiscard]] size_t     GetNumberOfAvailableFrames() const { return m_framesReceived.size(); }
    [[nodiscard]] Can::Frame ReceiveFrame();
    Can::Status              TransmitFrame(uint32_t                    addr,
                                           const std::vector<uint8_t>& data = std::vector<uint8_t>(),
                                           bool                        forceExtended = false);
    Can::Status              TransmitFrame(uint32_t       addr,
                                           const uint8_t* data          = nullptr,
                                           size_t         len           = 0,
                                           bool           forceExtended = false);

    void SetCallback(Can::Irq irq, const Callback& callback);
    void ClearCallback(Can::Irq irq);

    void EnableInterrupt(Can::Irq irq);
    void EnableInterrupts(const std::vector<Can::Irq>& irqs)
    {
        for (const auto& irq : irqs)
        {
            EnableInterrupt(irq);
        }
    }
    void DisableInterrupt(Can::Irq irq);
    void DisableInterrupts(const std::vector<Can::Irq>& irqs)
    {
        for (const auto& irq : irqs)
        {
            DisableInterrupt(irq);
        }
    }

private:
    static CAN_FilterTypeDef   AssertAndConvertFilterStruct(const Can::FilterConfiguration& config);
    bool                       WaitForFreeMailbox();
    void                       HandleFrameReception(Can::RxFifo fifo);
    static CAN_TxHeaderTypeDef BuildTxHeader(uint32_t addr, size_t len, bool forceExtended);

#                if !defined(NILAI_CAN_REGISTER_CALLBACKS)
public:
#                endif
    static void CanTxMailbox0CpltCb(CAN_HandleTypeDef* can);
    static void CanTxMailbox1CpltCb(CAN_HandleTypeDef* can);
    static void CanTxMailbox2CpltCb(CAN_HandleTypeDef* can);
    static void CanTxMailbox0AbortCb(CAN_HandleTypeDef* can);
    static void CanTxMailbox1AbortCb(CAN_HandleTypeDef* can);
    static void CanTxMailbox2AbortCb(CAN_HandleTypeDef* can);
    static void CanRxFifo0MsgPendingCb(CAN_HandleTypeDef* can);
    static void CanRxFifo0FullCallback(CAN_HandleTypeDef* can);
    static void CanRxFifo1MsgPendingCb(CAN_HandleTypeDef* can);
    static void CanRxFifo1FullCallback(CAN_HandleTypeDef* can);
    static void CanSleepCallback(CAN_HandleTypeDef* can);
    static void CanWakeUpFromRxCb(CAN_HandleTypeDef* can);
    static void CanErrorCb(CAN_HandleTypeDef* can);

private:
    CAN_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;
    Can::Status        m_status = Can::Status::ERROR_NONE;

    std::vector<Can::Frame>                      m_framesReceived;
    std::map<Can::Irq, Callback>                 m_callbacks;
    std::map<uint64_t, Can::FilterConfiguration> m_filters;

    static constexpr uint32_t s_timeout = 15;
};
}    // namespace Nilai::Drivers
#            else
#                if WARN_MISSING_STM_DRIVERS
#                    warning NilaiTFO CAN module enabled, but HAL_CAN_MODULE_ENABLED is not defined!
#                endif
#            endif
#        endif
#    else
#        include "../test/Mocks/drivers/can_module.h"
#    endif
#endif
/* ----- END OF FILE ----- */
