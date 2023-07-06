/**
 * @file    circular_buffer.h
 * @author  Paul Thomas
 * @date    2022-10-12
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
#ifndef NILAI_DEFINES_CIRCULAR_BUFFER_H
#define NILAI_DEFINES_CIRCULAR_BUFFER_H

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

/**
 * @addtogroup Nilai
 * @{
 */

namespace Nilai
{
template<typename T, size_t N>
class CircularBuffer
{
    static constexpr size_t s_size = N;    // Need 1 extra space so that
                                           // 1-past-end is not begin.
public:
    using array_t = std::array<T, s_size>;

    class iterator;

    constexpr CircularBuffer() {}

    template<typename... Args, size_t ArgSize = sizeof...(Args) + 1>
    constexpr explicit CircularBuffer(T&& i, Args&&... args)
        requires(ArgSize <= N)
    : m_buff {i, args...}, m_head(NextIdx(0, ArgSize)), m_full(ArgSize == m_capacity)
    {
    }

    template<typename... Args>
    constexpr void Emplace(Args&&... args) noexcept
        requires std::constructible_from<T, Args...>
    {
        Push(T {std::forward<Args>(args)...});
    }

    constexpr void Push(const T& t) noexcept
    {
        m_buff[m_head] = t;
        if (m_full)
        {
            // Buffer full, rotate the tail.
            m_tail = (m_tail + 1) % m_capacity;
        }
        m_head = (m_head + 1) % m_capacity;

        m_full = m_head == m_tail;
    }

    constexpr void PushMany(const T* items, size_t count) noexcept
    {
        for (size_t i = 0; i < count; i++)
        {
            Push(items[i]);
        }
    }

    template<typename I>
    constexpr void PushMany(const std::vector<I>& items) noexcept
    {
        return PushManyImpl(items);
    }

    template<typename I>
    constexpr void PushMany(const std::initializer_list<I>& items) noexcept
    {
        return PushManyImpl(items);
    }

    template<typename I, size_t S>
    constexpr void PushMany(const std::array<I, S>& items) noexcept
    {
        return PushManyImpl(items);
    }

    constexpr std::optional<T> Peek() noexcept
    {
        if (Empty())
        {
            return std::nullopt;
        }

        return m_buff[m_tail];
    }

    constexpr std::vector<T> PeekMany(size_t count) noexcept
    {
        count = std::min(count, Size());    // Clip count to size of buffer.
        std::vector<T> elems;
        elems.reserve(count);

        for (size_t i = 0; i < count; i++)
        {
            elems.push_back(m_buff[Idx(i)]);
        }

        return elems;
    }

    constexpr std::optional<T> Pop() noexcept
    {
        // Read data then advance the tail (+1 free slot).
        auto val = Peek();
        if (val)    // Only advance if value is valid.
        {
            m_tail = (m_tail + 1) % m_capacity;
            m_full = false;
        }
        return val;
    }

    constexpr std::vector<T> PopMany(size_t count) noexcept
    {
        count = std::min(count, Size());
        std::vector<T> elems;
        elems.reserve(count);

        for (size_t i = 0; i < count; i++)
        {
            auto v = Pop();
            if (!v)
            {
                return elems;
            }
            elems.push_back(*v);
        }

        return elems;
    }

    constexpr void Reset() noexcept
    {
        m_head = m_tail;
        m_full = false;
    }

    [[nodiscard]] constexpr bool Empty() const noexcept
    {
        // If head and tail are the same, buffer is empty.
        return !m_full && (m_head == m_tail);
    }

    [[nodiscard]] constexpr bool Full() const noexcept { return m_full; }

    constexpr void Resize(size_t newCap)
    {
        m_tail = std::min(newCap, m_tail);
        m_head = std::min(newCap, m_head);
        // Can't get bigger than N!
        m_capacity = std::min(newCap, N);
    }
    [[nodiscard]] constexpr size_t Capacity() const noexcept { return m_capacity; }

    [[nodiscard]] constexpr size_t Size() const noexcept
    {
        if (!m_full)
        {
            if (m_head >= m_tail)
            {
                return m_head - m_tail;
            }
            else
            {
                return m_capacity + m_head - m_tail;
            }
        }
        return m_capacity;
    }

    [[nodiscard]] constexpr size_t GetHeadIdx() const noexcept { return m_head; }
    [[nodiscard]] constexpr size_t GetTailIdx() const noexcept { return m_tail; }

    constexpr const array_t& RawBuffer() const noexcept { return m_buff; }
    constexpr array_t&       RawBuffer() noexcept { return m_buff; }

    constexpr std::pair<T*, size_t> DataAndRemainingSpace() noexcept
    {
        size_t remainingSpace = m_capacity - m_head;
        return std::make_pair(m_buff.data() + m_head, remainingSpace);
    }

    [[nodiscard]] constexpr size_t Idx(size_t idx) const noexcept { return NextIdx(m_tail, idx); }

    [[nodiscard]] constexpr size_t NextIdx(size_t current, size_t incBy = 1) const noexcept
    {
        return (current + incBy) % m_capacity;
    }

    class iterator
    {
        CircularBuffer& m_buff;
        size_t          m_idx = 0;

    public:
        constexpr iterator(CircularBuffer& buff, size_t idx) noexcept : m_buff(buff), m_idx(idx) {}
        constexpr iterator& operator++() noexcept
        {
            m_idx++;
            return *this;
        }
        constexpr iterator operator++(int) noexcept
        {
            iterator retval = *this;
            ++(*this);
            return retval;
        }
        constexpr bool operator==(const iterator& rhs) const noexcept { return m_idx == rhs.m_idx; }
        constexpr bool operator!=(const iterator& rhs) const noexcept { return !(*this == rhs); }
        constexpr T&   operator*() { return m_buff[m_idx]; }

        // Iterator traits.
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = const T*;
        using reference         = const T&;
        using iterator_category = std::forward_iterator_tag;
    };

    iterator begin() noexcept { return iterator(*this, 0); }
    iterator end() noexcept { return iterator(*this, Size()); }

    constexpr const T& operator[](size_t idx) const noexcept { return m_buff[Idx(idx)]; }
    constexpr T&       operator[](size_t idx) noexcept { return m_buff[Idx(idx)]; }

private:
    array_t m_buff;

    size_t m_capacity = N;

    size_t m_head = 0;    //!< End of the array.
    size_t m_tail = 0;    //!< Start of the array.

    bool m_full = false;

    template<typename I>
    constexpr void PushManyImpl(const I& items) noexcept
        requires std::constructible_from<T, typename I::value_type>
    {
        for (auto&& item : items)
        {
            Push(item);
        }
    }
};

// CTAD
template<typename... Args>
CircularBuffer(Args&&...) -> CircularBuffer<std::common_type_t<Args...>, sizeof...(Args)>;

}    // namespace Nilai
//!@}
#endif
