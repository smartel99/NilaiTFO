/**
 * @file    handle.h
 * @author  Samuel Martel
 * @date    2022-08-16
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

#ifndef GUARD_NILAI_MOCK_DRIVERS_HANDLE_H
#define GUARD_NILAI_MOCK_DRIVERS_HANDLE_H

#if defined(NILAI_TEST)
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup nilai_tests Tests
 * @{
 */

/**
 * @addtogroup nilai_tests_drivers Drivers
 * @{
 */

#    include "../../../defines/types/function.h"

#    include <concepts>
#    include <cstdint>
#    include <optional>
#    include <vector>

namespace Nilai::Test::Drivers
{
/**
 * @brief Concept describing the requirements for a type that can be used as the low-level
 * interface.
 * @tparam T a type that has a function called Matches returning a bool and that can be invoked
 * with the passed arguments.
 * @tparam Args... The arguments to pass to Matches
 */
template<typename T, typename... Args>
concept CanCallMatches = requires(T t, Args... args) {
                             {
                                 t.Matches(args...)
                                 } -> std::same_as<bool>;
                         };

/**
 * @brief Concept describing the requirements for a type that can be used as a frame by the handle.
 * @tparam T A type that can be default constructed and either copy constructible or move
 * constructible.
 */
template<typename T>
concept FrameLike = std::is_default_constructible_v<T> &&
                    (std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>);

/**
 * @class Handle
 * @brief Abstract class describing a hardware handle.
 * @tparam T
 */
template<FrameLike T>
class Handle
{
    //! Container holding frames.
    using buffer = std::vector<T>;

public:
    /**
     * @brief Controls the usage of the TX queue.
     *
     * When blocked, @ref QueueTxFrame will always return false and no frames can be queued in
     * the list.
     *
     * @param block Whether the transmission queue should be blocked or not.
     */
    void BlockTx(bool block) noexcept { m_txBlocked = block; }
    /**
     * @brief Controls the usage of the RX queue.
     *
     * When blocked, @ref GetRxFrame will never return anything.
     *
     * @param block Whether the reception queue should be blocked or not.
     */
    void BlockRx(bool block) noexcept { m_rxBlocked = block; }

    /**
     * @brief Queues a frame to be transmitted.
     * @tparam Args The arguments to construct the frame with.
     * @param args The arguments to construct the frame with.
     * @returns true if the frame was successfully queued.
     * @returns false if the frame was not successfully queued.
     */
    template<typename... Args>
    bool QueueTxFrame(Args&&... args)
        requires std::constructible_from<T, Args...>
    {
        if (m_txBlocked)
        {
            return false;
        }

        m_txQueue.emplace_back(std::forward<Args>(args)...);
        return true;
    }

    /**
     * @brief Receives a frame from the reception queue that matches the criteria passed as
     * arguments.
     *
     * If a frame matches the criteria, it is popped from the queue.
     *
     * @tparam Args The types of the criteria.
     * @param args The search criteria to find a frame.
     * @return If found, the frame is returned. Otherwise, nothing is returned.
     */
    template<typename... Args>
    std::optional<T> GetRxFrame(Args&&... args)
        requires CanCallMatches<T, Args...>
    {
        if (m_rxBlocked)
        {
            return {};
        }

        auto it = std::find_if(m_rxQueue.begin(),
                               m_rxQueue.end(),
                               [&args...](const auto& o)
                               { return o.Matches(std::forward<Args>(args)...); });

        if (it != m_rxQueue.end())
        {
            // Frame matches criteria, create a copy and pop it from the queue.
            auto copy = *it;
            m_rxQueue.erase(it);
            return copy;
        }

        // No item matches the criteria...
        return {};
    }

protected:
    bool m_txBlocked = false;
    bool m_rxBlocked = false;

    buffer m_txQueue = {};
    buffer m_rxQueue = {};
};

}    // namespace Nilai::Test::Drivers
//!@}
//!@}
//!@}

#endif
#endif    // GUARD_NILAI_MOCK_DRIVERS_HANDLE_H
