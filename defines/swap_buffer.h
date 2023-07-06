/**
 * @file    swap_buffer.h
 * @author  Samuel Martel
 * @date    2022-10-19
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

#ifndef GUARD_NILAI_DEFINES_SWAP_BUFFER_H
#define GUARD_NILAI_DEFINES_SWAP_BUFFER_H

#include <concepts>
#include <utility>
/**
 * @addtogroup Nilai
 * @{
 */
namespace Nilai
{

template<typename T>
class SwapBuffer
{
public:
    // Traits
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;

    constexpr SwapBuffer() : m_buffs {{}, {}} {}

    template<typename... Args>
    constexpr explicit SwapBuffer(Args&&... args)
        requires(std::constructible_from<value_type, Args...>)
    : m_buffs {{std::forward<Args>(args)...}, {std::forward<Args>(args)...}}
    {
    }

    [[nodiscard]] constexpr const_pointer operator->() const noexcept { return &m_buffs[m_active]; }
    [[nodiscard]] constexpr pointer       operator->() noexcept { return &m_buffs[m_active]; }

    [[nodiscard]] constexpr const_reference operator*() const noexcept { return m_buffs[m_active]; }
    [[nodiscard]] constexpr reference       operator*() noexcept { return m_buffs[m_active]; }

    [[nodiscard]] constexpr const_reference GetActive() const noexcept { return m_buffs[m_active]; }
    [[nodiscard]] constexpr reference       GetActive() noexcept { return m_buffs[m_active]; }

    /**
     * Swap the active buffer with the inactive buffer.
     * @return
     */
    constexpr void Swap() noexcept { m_active = m_active == 0 ? 1 : 0; }

    /**
     * Executes a function func on both buffers.
     * @tparam Func A function that takes a reference to a buffer.
     * @param func
     * @return
     */
    template<std::invocable<reference> Func>
    constexpr void ApplyToBoth(Func&& func)
    {
        func(m_buffs[0]);
        func(m_buffs[1]);
    }

private:
    value_type  m_buffs[2] = {};
    std::size_t m_active   = 0;
};

template<typename T, typename... Args>
auto MakeSwap(Args&&... args)
{
    return SwapBuffer<T>(std::forward<Args>(args)...);
}
}    // namespace Nilai
//!@}


#endif    // GUARD_NILAI_DEFINES_SWAP_CIRCULAR_BUFFER_H
