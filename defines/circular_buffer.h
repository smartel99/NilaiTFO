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

#include <cstdint>
#include <type_traits>
#include <vector>

/**
 * @addtogroup Nilai
 * @{
 */

namespace Nilai
{
template<typename T>
class CircularBuffer
{
public:
    using value_type       = typename std::vector<T>::value_type;
    using size_type        = typename std::vector<T>::size_type;
    using signed_size_type = std::make_signed_t<size_type>;
    using reference        = typename std::vector<T>::reference;
    using const_reference  = typename std::vector<T>::const_reference;
    using pointer          = typename std::vector<T>::pointer;
    using const_pointer    = typename std::vector<T>::const_pointer;

public:
    CircularBuffer() noexcept = default;


    explicit CircularBuffer(size_t len, const T& v = {}) : m_buff(len, v), m_size(0) {}

    CircularBuffer(std::initializer_list<T> args) : m_buff {args}, m_size(args.size()) {}

    /**
     * Return the first element of the buffer
     */
    constexpr reference       Front() { return m_buff[m_read]; }
    constexpr const_reference Front() const { return m_buff[m_read]; }

    /**
     * Increment the read pointer by one, effectively removing the first element
     * of the buffer If the buffer size is 0, does nothing
     */
    constexpr void Pop()
    {
        if (m_size > 0)
        {
            m_read = Next(m_read);
            --m_size;
        }
    }

    /**
     * Increment the read pointer by a number n
     * effectively removing the n first elements of the buffer
     */
    constexpr void Pop(size_type n)
    {
        for (size_type i = 0; (i < n) && (m_size >= 0); i++)
        {
            Pop();
        }
    }

    /**
     * Insert an element at the end of the buffer
     * If the buffer is full, does nothing
     * @param t the element to insert
     */
    constexpr size_type Push(const_reference t)
    {
        if (m_size < m_buff.size())
        {
            m_buff[m_write] = t;
            m_write         = Next(m_write);
            ++m_size;
            return 1;
        }
        return 0;
    }

    constexpr size_type Push(const_pointer t, size_type len)
    {
        size_type pushed = 0;
        for (size_type i = 0; i < len; ++i)
        {
            pushed += Push(t[i]);
        }
        return pushed;
    }

    [[nodiscard]] constexpr size_type Push(const std::vector<value_type>& buff)
    {
        return Push(buff.data(), buff.size());
    }

    [[nodiscard]] constexpr const_reference Read()
    {
        reference obj = m_buff[m_read];
        m_read        = Next(m_read);
        return obj;
    }

    constexpr size_type Read(pointer buff, size_type len = 0)
    {
        len = std::clamp(len, static_cast<size_type>(0), m_size);

        for (size_type i = 0; i < len; ++i)
        {
            buff[i] = this->operator[](i);
        }
        m_read = Next(m_read + len - 1);
        m_size -= len;
        return len;
    }

    [[nodiscard]] constexpr const_reference Peek() const noexcept
    {
        const_reference obj = m_buff[m_read];
        return obj;
    }

    constexpr size_type Peek(pointer buff, size_type len = 0)
    {
        len = std::clamp(len, static_cast<size_type>(0), m_size);

        for (size_type i = 0; i < len; ++i)
        {
            buff[i] = this->operator[](i);
        }

        return len;
    }

    constexpr size_type DmaCounter(signed_size_type counter)
    {
        size_type capacity = m_buff.capacity();
        // DMA counter is number of bytes remaining
        // We want the number of bytes written
        counter = capacity - counter;

        if (static_cast<signed_size_type>(counter) < m_lastDmaCounter)
        {
            m_lastDmaCounter -= capacity;
        }
        signed_size_type diff = counter - m_lastDmaCounter;

        m_write += diff;
        m_size += diff;

        // Wrap if needed
        while (m_write >= static_cast<signed_size_type>(capacity))
        {
            m_write -= capacity;
        }

        // Check overload
        if (m_size > capacity)
        {
            m_read = Next(m_read + m_size - capacity - 1);
            m_size = capacity;
        }
        m_lastDmaCounter = static_cast<int32_t>(counter);

        return diff;
    }

    [[nodiscard]] constexpr signed_size_type GetWritePos() const noexcept { return m_write; }

    [[nodiscard]] constexpr signed_size_type GetReadPos() const noexcept { return m_read; }

    constexpr void SetReadPos(size_type readPos)
    {
        while (readPos > m_buff.capacity())
        {
            readPos -= m_buff.capacity();
        }
        m_read = static_cast<signed_size_type>(readPos);
        m_size = m_write - m_read;
        if (m_size < 0)
        {
            m_size += m_buff.capacity();
        }
    }

    /**
     * Return the number of elements inserted in the buffer
     * Does not return the max size of the buffer
     * Elements inserted using operator [] are ignored
     */
    [[nodiscard]] constexpr size_type Size() const noexcept { return m_size; }

    [[nodiscard]] constexpr size_type Capacity() const noexcept { return m_buff.capacity(); }

    /**
     * Reset the cursor and size of the buffer to 0
     * Effectively clearing the buffer
     */
    constexpr void Clear() noexcept
    {
        m_size  = 0;
        m_read  = 0;
        m_write = 0;
    }

    /**
     * Access to the underlying array
     */
    constexpr pointer       Data() noexcept { return m_buff.data(); }
    constexpr const_pointer Data() const noexcept { return m_buff.data(); }

    /**
     * array access operator for writing
     * Note: does not update cursors position nor array size counter;
     */
    constexpr reference       operator[](signed_size_type i) { return m_buff[Idx(i)]; }
    constexpr const_reference operator[](signed_size_type i) const { return m_buff[Idx(i)]; }

private:
    std::vector<T> m_buff = {};

    // This is used so we have different sized buffer
    signed_size_type m_read  = 0;
    signed_size_type m_write = 0;
    size_type        m_size  = 0;

    signed_size_type m_lastDmaCounter = 0;

    [[nodiscard]] constexpr size_type Next(size_type origin) const
    {
        size_type r = origin + 1;
        while (r >= m_buff.capacity())
        {
            r -= m_buff.capacity();
        }
        return r;
    }

    [[nodiscard]] constexpr size_type Idx(size_type idx) const
    {
        size_type r = m_read + idx;
        while (r >= m_buff.capacity())
        {
            r -= m_buff.capacity();
        }
        return r;
    }
};
}    // namespace Nilai
//!@}
#endif
