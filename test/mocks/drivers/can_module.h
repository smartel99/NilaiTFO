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

#if defined(NILAI_USE_CAN)
#    include "defines/module.h"

#    include "../../../drivers/CAN/enums.h"
#    include "../../../drivers/CAN/structs.h"

#    include <functional>
#    include <map>
#    include <vector>

class CanModule : Nilai::Module
{
public:
    using Callbacks = std::map<Nilai::Can::Irq, std::function<void()>>;

    CanModule(void*, std::string label);

    bool                             DoPost() override { return true; }
    void                             Run() override {}
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

    void Reset() { m_hasReset = true; }

    void ConfigureFilter(const Nilai::Can::FilterConfiguration& cfg) { m_filters.push_back(cfg); }

    [[nodiscard]] size_t            GetNumberOfAvailableFrames() const { return m_rxQueue.size(); }
    [[nodiscard]] Nilai::Can::Frame ReceiveFrame();

    Nilai::Can::Status TransmitFrame(uint32_t                    addr,
                                     const std::vector<uint8_t>& data = std::vector<uint8_t>(),
                                     bool                        forceExtended = false);
    Nilai::Can::Status TransmitFrame(uint32_t       addr,
                                     const uint8_t* data          = nullptr,
                                     size_t         len           = 0,
                                     bool           forceExtended = false);

    void SetCallback(Nilai::Can::Irq irq, const std::function<void()>& cb);
    void ClearCallback(Nilai::Can::Irq irq);

    void EnableInterrupt(Nilai::Can::Irq irq);
    void EnableInterrupts(const std::vector<Nilai::Can::Irq>& irqs);

    void DisableInterrupt(Nilai::Can::Irq irq);
    void DisableInterrupts(const std::vector<Nilai::Can::Irq>& irqs);

    ///////////////////////////////////////////////////////////////////////////
    // Methods to be used by the tests.
    bool& GetHasReset() { return m_hasReset; }

    std::vector<Nilai::Can::FilterConfiguration>& GetFilters() { return m_filters; }

    std::vector<Nilai::Can::Frame>&   GetRxQueue() { return m_rxQueue; }
    std::vector<Nilai::Can::TxFrame>& GetTxQueue() { return m_txQueue; }

    Callbacks& GetCallbacks() { return m_callbacks; }
    void       CallCallback(Nilai::Can::Irq irq) { m_callbacks[irq](); }

    std::vector<Nilai::Can::Irq>& GetIrqs() { return m_irqs; }

    void SetTransmitStatus(Nilai::Can::Status s) { m_txStatus = s; }

private:
    std::string m_label;

    bool                                         m_hasReset = false;
    std::vector<Nilai::Can::FilterConfiguration> m_filters {};

    std::vector<Nilai::Can::Frame>   m_rxQueue {};
    std::vector<Nilai::Can::TxFrame> m_txQueue {};
    Nilai::Can::Status               m_txStatus = Nilai::Can::Status::ERROR_NONE;

    Callbacks m_callbacks {};

    std::vector<Nilai::Can::Irq> m_irqs {};
};
#endif
#endif    // NILAI_CANMODULE_HPP
