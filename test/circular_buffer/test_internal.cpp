/**
 * @file    test_internal.cpp
 * @author  Samuel Martel
 * @date    2022-10-16
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
#include "defines/circular_buffer.h"
#include <gtest/gtest.h>

#include <array>

using namespace Nilai;

TEST(NilaiCircularBufferInternal, DefaultInit)
{
    CircularBuffer cb = CircularBuffer<int, 10>();
    EXPECT_EQ(cb.Size(), 0);
    EXPECT_EQ(cb.Capacity(), 10);
    EXPECT_FALSE(cb.Full());
    EXPECT_TRUE(cb.Empty());
}

TEST(NilaiCircularBufferInternal, PartialInit)
{
    CircularBuffer cb = CircularBuffer<int, 10>(0, 1, 2, 3, 4);
    EXPECT_EQ(cb.Size(), 5);
    EXPECT_EQ(cb.Capacity(), 10);
    EXPECT_FALSE(cb.Full());
    EXPECT_FALSE(cb.Empty());

    for (size_t i = 0; i < cb.Size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
}

TEST(NilaiCircularBufferInternal, FullInit)
{
    CircularBuffer cb = CircularBuffer<int, 10>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    EXPECT_EQ(cb.Size(), 10);
    EXPECT_EQ(cb.Capacity(), 10);
    EXPECT_TRUE(cb.Full());
    EXPECT_FALSE(cb.Empty());

    for (size_t i = 0; i < cb.Size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PushSingleToFull)
{
    CircularBuffer cb = CircularBuffer<int, 1>();
    EXPECT_EQ(cb.Size(), 0);
    EXPECT_EQ(cb.Capacity(), 1);
    EXPECT_TRUE(cb.Empty());
    cb.Push(1);
    cb.Push(2);

    EXPECT_FALSE(cb.Empty());
    EXPECT_TRUE(cb.Full());
    EXPECT_EQ(cb.Size(), 1);

    EXPECT_EQ(cb[0], 2);
}

TEST(NilaiCircularBufferInternal, PushManyToFullInitializerList)
{
    CircularBuffer cb = CircularBuffer<int, 3>();
    cb.PushMany({4, 5, 0, 1, 2});
    EXPECT_FALSE(cb.Empty());
    EXPECT_TRUE(cb.Full());
    EXPECT_EQ(cb.Size(), 3);

    for (size_t i = 0; i < cb.Size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PushManyToFullVector)
{
    CircularBuffer   cb  = CircularBuffer<int, 3>();
    std::vector<int> vec = {4, 5, 0, 1, 2};
    cb.PushMany(vec);
    EXPECT_FALSE(cb.Empty());
    EXPECT_TRUE(cb.Full());
    EXPECT_EQ(cb.Size(), 3);

    for (size_t i = 0; i < cb.Size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PushManyToFullArray)
{
    CircularBuffer cb  = CircularBuffer<int, 3>();
    std::array     arr = {4, 5, 0, 1, 2};
    cb.PushMany(arr);
    EXPECT_FALSE(cb.Empty());
    EXPECT_TRUE(cb.Full());
    EXPECT_EQ(cb.Size(), 3);

    for (size_t i = 0; i < cb.Size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PushManyToFullCArray)
{
    CircularBuffer cb    = CircularBuffer<int, 3>();
    int            arr[] = {4, 5, 0, 1, 2};
    cb.PushMany(arr, 5);
    EXPECT_FALSE(cb.Empty());
    EXPECT_TRUE(cb.Full());
    EXPECT_EQ(cb.Size(), 3);

    for (size_t i = 0; i < cb.Size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PopSingleEmpty)
{
    CircularBuffer cb = CircularBuffer<int, 3>();

    auto v = cb.Pop();
    EXPECT_EQ(cb.Size(), 0);
    EXPECT_TRUE(cb.Empty());
    EXPECT_FALSE(cb.Full());
    EXPECT_FALSE(v);
}

TEST(NilaiCircularBufferInternal, PopSingle)
{
    CircularBuffer cb = CircularBuffer<int, 3>(0, 1, 2);

    auto v = cb.Pop();
    EXPECT_EQ(cb.Size(), 2);
    EXPECT_FALSE(cb.Empty());
    EXPECT_FALSE(cb.Full());
    EXPECT_EQ(*v, 0);

    // buffer should contain {1, 2}.
    size_t expected = 1;
    for (auto&& val : cb)
    {
        EXPECT_EQ(val, expected++);
    }
}

TEST(NilaiCircularBufferInternal, PopManyPartial)
{
    CircularBuffer cb = CircularBuffer<int, 3>(0, 1, 2);

    auto v = cb.PopMany(2);
    EXPECT_EQ(cb.Size(), 1);
    EXPECT_FALSE(cb.Empty());
    EXPECT_FALSE(cb.Full());
    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(cb[0], 2);
}

TEST(NilaiCircularBufferInternal, PopManyComplete)
{
    CircularBuffer cb = CircularBuffer<int, 3>(0, 1, 2);

    auto v = cb.PopMany(3);
    EXPECT_EQ(cb.Size(), 0);
    EXPECT_TRUE(cb.Empty());
    EXPECT_FALSE(cb.Full());
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
}

TEST(NilaiCircularBufferInternal, PopManyOversize)
{
    CircularBuffer cb = CircularBuffer<int, 3>(0, 1, 2);

    auto v = cb.PopMany(5);
    EXPECT_EQ(cb.Size(), 0);
    EXPECT_TRUE(cb.Empty());
    EXPECT_FALSE(cb.Full());
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
}

TEST(NilaiCircularBufferInternal, Reset)
{
    CircularBuffer cb = CircularBuffer<int, 3>(0, 1, 2);
    EXPECT_EQ(cb.Size(), 3);
    EXPECT_TRUE(cb.Full());
    EXPECT_FALSE(cb.Empty());

    cb.Reset();

    // Buffer should be empty.
    EXPECT_TRUE(cb.Empty());
    EXPECT_EQ(cb.Size(), 0);
    EXPECT_FALSE(cb.Full());

    for ([[maybe_unused]] auto&& v : cb)
    {
        EXPECT_EQ(v, -1);
    }
}

TEST(NilaiCircularBufferInternal, ResizeShrink)
{
    CircularBuffer cb = CircularBuffer<int, 10>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    EXPECT_EQ(cb.Size(), 10);
    EXPECT_EQ(cb.Capacity(), 10);

    cb.Resize(5);
    EXPECT_EQ(cb.Size(), 5);
    EXPECT_EQ(cb.Capacity(), 5);
    EXPECT_TRUE(cb.Full());
    EXPECT_FALSE(cb.Empty());

    // Try to pop 10 elements, should actually only pop 5.
    const auto& buff = cb.PopMany(10);
    ASSERT_EQ(buff.size(), 5);

    for (size_t i = 0; i < buff.size(); i++)
    {
        EXPECT_EQ(buff[i], i);
    }

    // Buffer should be empty.
    EXPECT_TRUE(cb.Empty());
    for ([[maybe_unused]] auto&& v : cb)
    {
        EXPECT_EQ(v, -1);
    }
}

TEST(NilaiCircularBufferInternal, ResizeValidGrowth)
{
    CircularBuffer cb = CircularBuffer<int, 10>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    EXPECT_EQ(cb.Size(), 10);
    EXPECT_EQ(cb.Capacity(), 10);

    cb.Resize(5);    // Shrink to 5.
    cb.Resize(7);    // Grow back up to 7.

    EXPECT_EQ(cb.Size(), 7);
    EXPECT_EQ(cb.Capacity(), 7);
    EXPECT_TRUE(cb.Full());
    EXPECT_FALSE(cb.Empty());

    // Try to pop 10 elements, should actually only pop 7.
    const auto& buff = cb.PopMany(10);
    ASSERT_EQ(buff.size(), 7);

    for (size_t i = 0; i < buff.size(); i++)
    {
        EXPECT_EQ(buff[i], i);
    }

    // Buffer should be empty.
    EXPECT_TRUE(cb.Empty());
    for ([[maybe_unused]] auto&& v : cb)
    {
        EXPECT_EQ(v, -1);
    }
}

TEST(NilaiCircularBufferInternal, ResizeInvalidGrowth)
{
    CircularBuffer cb = CircularBuffer<int, 10>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    EXPECT_EQ(cb.Size(), 10);
    EXPECT_EQ(cb.Capacity(), 10);

    cb.Resize(15);    // Should not do anything, size is not valid.

    EXPECT_EQ(cb.Size(), 10);
    EXPECT_EQ(cb.Capacity(), 10);
    EXPECT_TRUE(cb.Full());
    EXPECT_FALSE(cb.Empty());

    // Try to pop 10 elements, should actually only pop 10.
    const auto& buff = cb.PopMany(10);
    ASSERT_EQ(buff.size(), 10);

    for (size_t i = 0; i < buff.size(); i++)
    {
        EXPECT_EQ(buff[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PeekEmpty)
{
    CircularBuffer cb = CircularBuffer<int, 10>();
    EXPECT_EQ(cb.Size(), 0);

    auto v = cb.Peek();
    EXPECT_FALSE(v);
}

TEST(NilaiCircularBufferInternal, Peek)
{
    CircularBuffer cb = CircularBuffer<int, 10>(1);
    EXPECT_EQ(cb.Size(), 1);

    auto v = cb.Peek();
    EXPECT_TRUE(v);
    EXPECT_EQ(*v, 1);
}

TEST(NilaiCircularBufferInternal, PeekManyEmpty)
{
    CircularBuffer cb = CircularBuffer<int, 10>();
    EXPECT_EQ(cb.Size(), 0);

    auto v = cb.PeekMany(5);
    EXPECT_TRUE(v.empty());
}

TEST(NilaiCircularBufferInternal, PeekManyPartial)
{
    CircularBuffer cb = CircularBuffer<int, 10> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int> v = cb.PeekMany(5);
    ASSERT_EQ(v.size(), 5);

    for (size_t i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PeekManyFull)
{
    CircularBuffer cb = CircularBuffer<int, 10> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int> v = cb.PeekMany(10);
    ASSERT_EQ(v.size(), 10);

    for (size_t i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], i);
    }
}

TEST(NilaiCircularBufferInternal, PeekManyMoreThanFull)
{
    CircularBuffer cb = CircularBuffer<int, 10> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int> v = cb.PeekMany(15);
    ASSERT_EQ(v.size(), 10);

    for (size_t i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], i);
    }
}
