/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  can
 * @{
 * @file        canModule.hpp
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
#            include "defines/internalConfig.h"
#            include NILAI_HAL_HEADER
#            if defined(HAL_CAN_MODULE_ENABLED)
#                include "defines/macros.hpp"
#                include "defines/misc.hpp"
#                include "defines/module.hpp"

#                include "CAN/Enums.h"
#                include "CAN/Structs.h"

#                include <array>
#                include <cstdint>
#                include <functional>
#                include <map>
#                include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Enumerated Types
 * ----------------------------------------------------------------------------
 */
class CanModule : public cep::Module
{
public:
    CanModule(CAN_HandleTypeDef* handle, std::string label);
    ~CanModule() override;

    [[nodiscard]] bool               DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    void Reset();

    void ConfigureFilter(const CEP_CAN::FilterConfiguration& config);

    [[nodiscard]] size_t GetNumberOfAvailableFrames() const { return m_framesReceived.size(); }
    [[nodiscard]] CEP_CAN::Frame ReceiveFrame();
    CEP_CAN::Status              TransmitFrame(uint32_t                    addr,
                                               const std::vector<uint8_t>& data = std::vector<uint8_t>(),
                                               bool                        forceExtended = false);
    CEP_CAN::Status              TransmitFrame(uint32_t       addr,
                                               const uint8_t* data          = nullptr,
                                               size_t         len           = 0,
                                               bool           forceExtended = false);

    void SetCallback(CEP_CAN::Irq irq, const std::function<void()>& callback);
    void ClearCallback(CEP_CAN::Irq irq);

    void EnableInterrupt(CEP_CAN::Irq irq);
    void EnableInterrupts(const std::vector<CEP_CAN::Irq>& irqs)
    {
        for (const auto& irq : irqs)
        {
            EnableInterrupt(irq);
        }
    }
    void DisableInterrupt(CEP_CAN::Irq irq);
    void DisableInterrupts(const std::vector<CEP_CAN::Irq>& irqs)
    {
        for (const auto& irq : irqs)
        {
            DisableInterrupt(irq);
        }
    }

    void HandleIrq();

private:
    static CAN_FilterTypeDef AssertAndConvertFilterStruct(
      const CEP_CAN::FilterConfiguration& config);
    bool                       WaitForFreeMailbox();
    void                       HandleFrameReception(CEP_CAN::RxFifo fifo);
    bool                       HandleTxMailbox0Irq(uint32_t ier);
    bool                       HandleTxMailbox1Irq(uint32_t ier);
    bool                       HandleTxMailbox2Irq(uint32_t ier);
    bool                       HandleRxFifo0Irq(uint32_t ier);
    bool                       HandleRxFifo1Irq(uint32_t ier);
    bool                       HandleSleepIrq(uint32_t ier);
    bool                       HandleWakeupIrq(uint32_t ier);
    bool                       HandleErrorIrq(uint32_t ier);
    static CAN_TxHeaderTypeDef BuildTxHeader(uint32_t addr, size_t len, bool forceExtended);

private:
    CAN_HandleTypeDef* m_handle = nullptr;
    std::string        m_label;
    CEP_CAN::Status    m_status = CEP_CAN::Status::ERROR_NONE;

    std::vector<CEP_CAN::Frame>                      m_framesReceived;
    std::map<CEP_CAN::Irq, std::function<void()>>    m_callbacks;
    std::map<uint64_t, CEP_CAN::FilterConfiguration> m_filters;

    static constexpr uint32_t s_timeout = 15;
};
#            else
#                if WARN_MISSING_STM_DRIVERS
#                    warning NilaiTFO CAN module enabled, but HAL_CAN_MODULE_ENABLED is not defined!
#                endif
#            endif
#        endif
#    else
#        include "../test/Mocks/drivers/canModule.hpp"
#    endif
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
