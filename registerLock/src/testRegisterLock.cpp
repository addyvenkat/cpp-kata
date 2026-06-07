#include <gtest/gtest.h>
#include "registerLock.hpp"

// Test fixture for setup and teardown convenience
class RegisterLockTest : public ::testing::Test {
protected:
    HarwareRegister mock_reg;

    void SetUp() override {
        mock_reg.lockBit = false;
    }
};

// 1. Test standard RAII Lock and Unlock via scoping
TEST_F(RegisterLockTest, LocksOnConstructionAndUnlocksOnDestruction) {
    // Before lock
    EXPECT_FALSE(mock_reg.lockBit);

    {
        registerLock lock(&mock_reg);
        // Must be locked inside the scope
        EXPECT_TRUE(mock_reg.lockBit);
    }

    // Must be automatically unlocked outside the scope
    EXPECT_FALSE(mock_reg.lockBit);
}

// 2. Test Move Constructor
TEST_F(RegisterLockTest, MoveConstructorTransfersOwnership) {
    {
        registerLock original_lock(&mock_reg);
        EXPECT_TRUE(mock_reg.lockBit);

        // Move ownership to a new instance
        registerLock moved_lock(std::move(original_lock));
        
        // Register should still be locked because moved_lock now owns it
        EXPECT_TRUE(mock_reg.lockBit);
    }
    
    // Out of scope, moved_lock destructs and should unlock it
    EXPECT_FALSE(mock_reg.lockBit);
}

// 3. Test Move Assignment Operator
TEST_F(RegisterLockTest, MoveAssignmentTransfersOwnershipAndCleansUp) {
    HarwareRegister second_reg;
    second_reg.lockBit = false;

    {
        registerLock lock1(&mock_reg);
        registerLock lock2(&second_reg);

        EXPECT_TRUE(mock_reg.lockBit);
        EXPECT_TRUE(second_reg.lockBit);

        // Move assignment: lock1 takes over second_reg. 
        // lock1's original resource (mock_reg) should be unlocked in the process.
        lock1 = std::move(lock2);

        // mock_reg should now be unlocked because lock1 let it go
        EXPECT_FALSE(mock_reg.lockBit);
        // second_reg should still be locked because lock1 now manages it
        EXPECT_TRUE(second_reg.lockBit);
    }

    // Both should be safely unlocked now
    EXPECT_FALSE(mock_reg.lockBit);
    EXPECT_FALSE(second_reg.lockBit);
}

// 4. Test Compile-Time Deletion of Copy Semantics
// This ensures no one accidentally adds copy support back later
TEST_F(RegisterLockTest, CopySemanticsAreDeleted) {
    EXPECT_FALSE(std::is_copy_constructible<registerLock>::value);
    EXPECT_FALSE(std::is_copy_assignable<registerLock>::value);
    
    // Also verify that move semantics remain enabled
    EXPECT_TRUE(std::is_move_constructible<registerLock>::value);
    EXPECT_TRUE(std::is_move_assignable<registerLock>::value);
}