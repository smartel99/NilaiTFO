/**
 * @file    canModule.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#include "can_module.h"

#if defined(NILAI_USE_CAN)
#    include <algorithm>

CanModule::CanModule(void*, std::string label) : m_label(std::move(label))
{
    m_callbacks = Callbacks({{CEP_CAN::Irq::TxMailboxEmpty, []() {}},
                             {CEP_CAN::Irq::Fifo0MessagePending, []() {}},
                             {CEP_CAN::Irq::Fifo0Full, []() {}},
                             {CEP_CAN::Irq::Fifo0Overrun, []() {}},
                             {CEP_CAN::Irq::Fifo1MessagePending, []() {}},
                             {CEP_CAN::Irq::Fifo1Full, []() {}},
                             {CEP_CAN::Irq::Fifo1Overrun, []() {}},
                             {CEP_CAN::Irq::Wakeup, []() {}},
                             {CEP_CAN::Irq::SleepAck, []() {}},
                             {CEP_CAN::Irq::ErrorWarning, []() {}},
                             {CEP_CAN::Irq::ErrorPassive, []() {}},
                             {CEP_CAN::Irq::BusOffError, []() {}},
                             {CEP_CAN::Irq::LastErrorCode, []() {}},
                             {CEP_CAN::Irq::ErrorStatus, []() {}}});
}

CEP_CAN::Frame CanModule::ReceiveFrame()
{
    CEP_CAN::Frame frame = m_rxQueue.back();
    m_rxQueue.pop_back();
    return frame;
}

CEP_CAN::Status CanModule::TransmitFrame(uint32_t                    addr,
                                         const std::vector<uint8_t>& data,
                                         bool                        forceExtended)
{
    m_txQueue.emplace_back(addr, data, forceExtended);
    return m_txStatus;
}

CEP_CAN::Status CanModule::TransmitFrame(uint32_t       addr,
                                         const uint8_t* data,
                                         size_t         len,
                                         bool           forceExtended)
{
    if (data && len > 0)
    {
        m_txQueue.emplace_back(addr, std::vector(data, data + len), forceExtended);
    }
    else
    {
        m_txQueue.emplace_back(addr, std::vector<uint8_t> {}, forceExtended);
    };
    return m_txStatus;
}

void CanModule::SetCallback(CEP_CAN::Irq irq, const std::function<void()>& cb)
{
    if (cb)
    {
        m_callbacks[irq] = cb;
    }
}

void CanModule::ClearCallback(CEP_CAN::Irq irq)
{
    m_callbacks[irq] = []() {};
}

void CanModule::EnableInterrupt(CEP_CAN::Irq irq)
{
    m_irqs.push_back(irq);
}

void CanModule::EnableInterrupts(const std::vector<CEP_CAN::Irq>& irqs)
{
    for (const auto& irq : irqs)
    {
        m_irqs.push_back(irq);
    }
}

void CanModule::DisableInterrupt(CEP_CAN::Irq irq)
{
    m_irqs.erase(
      std::remove_if(m_irqs.begin(), m_irqs.end(), [irq](const auto& i) { return i == irq; }),
      m_irqs.end());
}

void CanModule::DisableInterrupts(const std::vector<CEP_CAN::Irq>& irqs)
{
    for (const auto& irq : irqs)
    {
        DisableInterrupt(irq);
    }
}
#endif
