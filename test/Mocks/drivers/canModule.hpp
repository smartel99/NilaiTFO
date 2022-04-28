/**
 * @file    canModule.hpp
 * @author  Samuel Martel
 * @date    2022-04-21
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_CANMODULE_HPP
#define NILAI_CANMODULE_HPP

#include "defines/module.hpp"

#include "../../../drivers/CAN/Enums.h"
#include "../../../drivers/CAN/Structs.h"

#include <functional>
#include <map>
#include <vector>


namespace CEP_CAN
{
struct TxFrame
{
    uint32_t             Addr = 0;
    std::vector<uint8_t> Data {};
    bool                 ForceExtended = false;

    TxFrame(uint32_t addr, const std::vector<uint8_t>& data, bool f)
    : Addr(addr), Data(data), ForceExtended(f)
    {
    }
};
}    // namespace CEP_CAN

class CanModule : cep::Module
{
public:
    using Callbacks = std::map<CEP_CAN::Irq, std::function<void()>>;

    CanModule(void*, std::string label);

    bool                             DoPost() override { return true; }
    void                             Run() override {}
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    void Reset() { m_hasReset = true; }

    void ConfigureFilter(const CEP_CAN::FilterConfiguration& cfg) { m_filters.push_back(cfg); }

    [[nodiscard]] size_t         GetNumberOfAvailableFrames() const { return m_rxQueue.size(); }
    [[nodiscard]] CEP_CAN::Frame ReceiveFrame();

    CEP_CAN::Status TransmitFrame(uint32_t                    addr,
                                  const std::vector<uint8_t>& data = std::vector<uint8_t>(),
                                  bool                        forceExtended = false);
    CEP_CAN::Status TransmitFrame(uint32_t       addr,
                                  const uint8_t* data          = nullptr,
                                  size_t         len           = 0,
                                  bool           forceExtended = false);

    void SetCallback(CEP_CAN::Irq irq, const std::function<void()>& cb);
    void ClearCallback(CEP_CAN::Irq irq);

    void EnableInterrupt(CEP_CAN::Irq irq);
    void EnableInterrupts(const std::vector<CEP_CAN::Irq>& irqs);

    void DisableInterrupt(CEP_CAN::Irq irq);
    void DisableInterrupts(const std::vector<CEP_CAN::Irq>& irqs);

    ///////////////////////////////////////////////////////////////////////////
    // Methods to be used by the tests.
    bool& GetHasReset() { return m_hasReset; }

    std::vector<CEP_CAN::FilterConfiguration>& GetFilters() { return m_filters; }

    std::vector<CEP_CAN::Frame>&   GetRxQueue() { return m_rxQueue; }
    std::vector<CEP_CAN::TxFrame>& GetTxQueue() { return m_txQueue; }

    Callbacks& GetCallbacks() { return m_callbacks; }
    void       CallCallback(CEP_CAN::Irq irq) { m_callbacks[irq](); }

    std::vector<CEP_CAN::Irq>& GetIrqs() { return m_irqs; }

    void SetTransmitStatus(CEP_CAN::Status s) { m_txStatus = s; }

private:
    std::string m_label;

    bool                                      m_hasReset = false;
    std::vector<CEP_CAN::FilterConfiguration> m_filters {};

    std::vector<CEP_CAN::Frame>   m_rxQueue {};
    std::vector<CEP_CAN::TxFrame> m_txQueue {};
    CEP_CAN::Status               m_txStatus = CEP_CAN::Status::ERROR_NONE;

    Callbacks m_callbacks {};

    std::vector<CEP_CAN::Irq> m_irqs {};
};

#endif    // NILAI_CANMODULE_HPP
