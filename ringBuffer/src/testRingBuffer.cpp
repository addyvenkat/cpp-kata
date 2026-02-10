#include <gtest/gtest.h>
#include "RingBuffer.h"

class RingBufferTest : public ::testing::Test {
protected:
    ring_buffer_t rb;

    void SetUp() override {
        rb_init(&rb, 8);
    }

    void TearDown() override {
        free(rb.buffer);
    }
};

TEST_F(RingBufferTest, StartsEmpty) {
    EXPECT_TRUE(rb_is_empty(&rb));
    EXPECT_FALSE(rb_is_full(&rb));
}

TEST_F(RingBufferTest, EnqueueDequeue) {
    uint8_t val = 0;
    EXPECT_TRUE(rb_enqueue(&rb, 42));
    EXPECT_TRUE(rb_dequeue(&rb, &val));
    EXPECT_EQ(val, 42);
    EXPECT_TRUE(rb_is_empty(&rb));
}

TEST_F(RingBufferTest, FullCondition) {
    // Fill 7 elements (Size 8 - 1)
    for(int i=0; i<7; i++) {
        EXPECT_TRUE(rb_enqueue(&rb, static_cast<uint8_t>(i)));
    }
    EXPECT_TRUE(rb_is_full(&rb));
    EXPECT_FALSE(rb_enqueue(&rb, 99)); // Should fail
}