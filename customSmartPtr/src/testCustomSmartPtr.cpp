#include <gtest/gtest.h>
#include "customSmartPtr.hpp"

// A simple helper struct to test member selection (operator->)
struct MockObject {
    int value;
    void increment() { value++; }
};

// Test 1: Initialization and Null Pointers
TEST(CustomSmartPtrTest, DefaultAndNullConstructor) {
    customSmartPtr<int> emptyPtr;
    EXPECT_EQ(emptyPtr.use_count(), 0);

    customSmartPtr<int> nullPtr(nullptr);
    EXPECT_EQ(nullPtr.use_count(), 0);
}

// Test 2: Valid Basic Constructor
TEST(CustomSmartPtrTest, BasicConstructor) {
    customSmartPtr<int> ptr(new int(42));
    EXPECT_EQ(ptr.use_count(), 1);
    EXPECT_EQ(*ptr, 42);
}

// Test 3: Copy Constructor
TEST(CustomSmartPtrTest, CopyConstructor) {
    customSmartPtr<int> ptr1(new int(100));
    EXPECT_EQ(ptr1.use_count(), 1);

    {
        customSmartPtr<int> ptr2(ptr1); // Copy construct
        EXPECT_EQ(ptr1.use_count(), 2);
        EXPECT_EQ(ptr2.use_count(), 2);
        EXPECT_EQ(*ptr1, 100);
        EXPECT_EQ(*ptr2, 100);
    } // ptr2 goes out of scope

    EXPECT_EQ(ptr1.use_count(), 1);
}

// Test 4: Move Constructor
TEST(CustomSmartPtrTest, MoveConstructor) {
    customSmartPtr<int> ptr1(new int(200));
    EXPECT_EQ(ptr1.use_count(), 1);

    customSmartPtr<int> ptr2(std::move(ptr1)); // Move construct

    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, 200);
    
    // After a move, ptr1's internal counts should be reset
    EXPECT_EQ(ptr1.use_count(), 0);
}

// Test 5: Copy Assignment Operator
TEST(CustomSmartPtrTest, CopyAssignment) {
    customSmartPtr<int> ptr1(new int(10));
    customSmartPtr<int> ptr2(new int(20));

    EXPECT_EQ(ptr1.use_count(), 1);
    EXPECT_EQ(ptr2.use_count(), 1);

    ptr2 = ptr1; // Copy assignment (cleans up original ptr2 data)

    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_EQ(*ptr2, 10);
}

// Test 6: Self Copy Assignment
TEST(CustomSmartPtrTest, SelfCopyAssignment) {
    customSmartPtr<int> ptr(new int(50));
    
    // Suppress compiler warnings about self-assignment if any
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wself-assign-overloaded"
    ptr = ptr; 
    #pragma clang diagnostic pop

    EXPECT_EQ(ptr.use_count(), 1);
    EXPECT_EQ(*ptr, 50);
}

// Test 7: Move Assignment Operator
TEST(CustomSmartPtrTest, MoveAssignment) {
    customSmartPtr<int> ptr1(new int(300));
    customSmartPtr<int> ptr2(new int(400));

    ptr2 = std::move(ptr1); // Move assignment

    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, 300);
    EXPECT_EQ(ptr1.use_count(), 0);
}

// Test 8: Self Move Assignment
TEST(CustomSmartPtrTest, SelfMoveAssignment) {
    customSmartPtr<int> ptr(new int(75));
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wself-move"
    ptr = std::move(ptr);
    #pragma clang diagnostic pop

    EXPECT_EQ(ptr.use_count(), 1);
    EXPECT_EQ(*ptr, 75);
}

// Test 9: Operators (* and ->)
TEST(CustomSmartPtrTest, Operators) {
    customSmartPtr<MockObject> ptr(new MockObject{10});

    // Test operator->
    EXPECT_EQ(ptr->value, 10);
    ptr->increment();
    EXPECT_EQ(ptr->value, 11);

    // Test operator*
    MockObject& ref = *ptr;
    EXPECT_EQ(ref.value, 11);
}