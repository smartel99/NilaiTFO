#include <defines/circular_buffer.hpp>
#include <gtest/gtest.h>


TEST(CircularBuffer, Default) {
    CircularBuffer<int> cb(0);
    EXPECT_EQ(0, cb.size());
}

TEST(CircularBuffer, Insert_Nominal) {
    CircularBuffer<int> cb(5);
    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j <= i; ++j) {
            cb.push(i *5 + j);
            EXPECT_EQ(j +1, cb.size());
            for(int k = 0; k < j; ++k) {
                EXPECT_EQ(i * 5 + k, cb[k]);
            }
        }
        for(int j = 0; j <= i; ++j) {
            cb.pop();
        }
    }
}

TEST(CircularBuffer, Insert_Full) {
    CircularBuffer<int> cb(1);
    cb.push(1);
    cb.push(2);
    EXPECT_EQ(1, cb.size());
    EXPECT_EQ(1, cb.front());
}

TEST(CircularBuffer, Insert_Turnover) {
    CircularBuffer<int> cb(5);
    for(int i = 0; i < 5; ++i) {
        cb.push(i);
    }
    for(int i = 5; i < 10; ++i) {
        cb.pop();
        cb.push(i);
    }
    for(int i = 5; i < 10; ++i) {
        EXPECT_EQ(i, cb[i]);
    }

}

TEST(CircularBuffer, Pop_Single) {
    CircularBuffer<int> cb(5);
    for(int i = 0; i < 5; ++i) {
        cb.push(i);
    }
    cb.pop();
    EXPECT_EQ(1, cb.front());
    EXPECT_EQ(4, cb.size());
}

TEST(CircularBuffer, Pop_Multiple_Part_Reduce) {
    CircularBuffer<int> cb(5); 
    for(int i = 0; i < 5; ++i) {
        cb.push(i);
    }
    cb.pop(3);
    EXPECT_EQ(2, cb.size());
    EXPECT_EQ(3, cb.front());
}

TEST(CircularBuffer, Pop_Multple_Part_Clear) {
    CircularBuffer<int> cb(5); 
    for(int i = 0; i < 3; ++i) {
        cb.push(i);
    }
    cb.pop(4);
    EXPECT_EQ(0, cb.size());
}

TEST(CircularBuffer, Pop_Multiple_Full) {
    CircularBuffer<int> cb(5);
    for(int i = 0; i < 5; ++i) {
        cb.push(i);
    }
    cb.pop(6);
    EXPECT_EQ(0, cb.size());
}
