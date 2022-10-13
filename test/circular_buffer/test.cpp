#include <defines/circular_buffer.h>
#include <gtest/gtest.h>

using namespace Nilai;

TEST(CircularBuffer, Default)
{
    CircularBuffer<int> cb = CircularBuffer<int>(0);
    EXPECT_EQ(0, cb.Size());
}

TEST(CircularBuffer, Insert_Nominal)
{
    CircularBuffer<int> cb = CircularBuffer<int>(5);
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            cb.Push(i * 5 + j);
            EXPECT_EQ(j + 1, cb.Size()) << i << j;
            for (int k = 0; k < j; ++k)
            {
                EXPECT_EQ(i * 5 + k, cb[k]);
            }
        }
        for (int j = 0; j <= i; ++j)
        {
            cb.Pop();
        }
    }
}

TEST(CircularBuffer, Insert_Full)
{
    CircularBuffer<int> cb = CircularBuffer<int>(1);
    cb.Push(1);
    cb.Push(2);
    EXPECT_EQ(1, cb.Size());
    EXPECT_EQ(1, cb.Front());
}

TEST(CircularBuffer, Insert_Turnover)
{
    CircularBuffer<int> cb = {0, 1, 2, 3, 4};

    for (int i = 5; i < 10; ++i)
    {
        cb.Pop();
        cb.Push(i);
    }
    for (int i = 5; i < 10; ++i)
    {
        EXPECT_EQ(i, cb[i]);
    }
}

TEST(CircularBuffer, Pop_Single)
{
    CircularBuffer<int> cb = {0, 1, 2, 3, 4};
    cb.Pop();
    EXPECT_EQ(1, cb.Front());
    EXPECT_EQ(4, cb.Size());
}

TEST(CircularBuffer, Pop_Multiple_Part_Reduce)
{
    CircularBuffer<int> cb = {0, 1, 2, 3, 4};
    cb.Pop(3);
    EXPECT_EQ(2, cb.Size());
    EXPECT_EQ(3, cb.Front());
}

TEST(CircularBuffer, Pop_Multple_Part_Clear)
{
    CircularBuffer<int> cb = {0, 1, 2};
    EXPECT_EQ(3, cb.Size());
    cb.Pop(4);
    EXPECT_EQ(0, cb.Size());
}

TEST(CircularBuffer, Pop_Multiple_Full)
{
    CircularBuffer<int> cb = {0, 1, 2, 3, 4};
    cb.Pop(6);
    EXPECT_EQ(0, cb.Size());
}

TEST(CircularBuffer, Dma)
{
    int                 buff[5];
    CircularBuffer<int> cb = CircularBuffer<int>(5);

    // 5 -> 5 = 0 diff
    EXPECT_EQ(cb.DmaCounter(5), 0);

    // 5 -> 0 = 5 diff
    EXPECT_EQ(cb.DmaCounter(0), 5);

    // 0 -> 0 = 0 diff
    EXPECT_EQ(cb.DmaCounter(0), 0);

    // 5 -> 3 = 2 diff
    EXPECT_EQ(cb.DmaCounter(3), 2);

    // 3 -> 4 <=> 3 + 5 -> 4 <=> 8 -> 4 = 4
    EXPECT_EQ(cb.DmaCounter(4), 4);

    // 4 -> 3 = 1
    EXPECT_EQ(cb.DmaCounter(3), 1);
}
