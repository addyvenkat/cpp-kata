#include <gtest/gtest.h>
#include "alignedMalloc.h"

// Test 1: Invalid alignments (not powers of 2, or 0) should return NULL
TEST(AlignedMallocTest, HandlesInvalidAlignment) {
    EXPECT_EQ(alignedMalloc(100, 0), nullptr);
    EXPECT_EQ(alignedMalloc(100, 3), nullptr);
    EXPECT_EQ(alignedMalloc(100, 15), nullptr);
    EXPECT_EQ(alignedMalloc(100, 31), nullptr);
}

// Test 2: Basic successful allocation and alignment verification
TEST(AlignedMallocTest, ValidAlignments) {
    size_t alignments[] = {1, 2, 4, 8, 16, 32, 64, 128};
    size_t size = 1024;

    for (size_t alignment : alignments) {
        void* ptr = alignedMalloc(size, alignment);
        
        // Ensure allocation succeeded
        ASSERT_NE(ptr, nullptr) << "Failed to allocate for alignment: " << alignment;
        
        // Verify address alignment using bitwise AND
        uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
        EXPECT_EQ(addr & (alignment - 1), 0) << "Pointer " << ptr << " is not aligned to " << alignment;
        
        // Verify we can write to the memory safely
        std::memset(ptr, 0xAA, size);
        EXPECT_EQ(*static_cast<unsigned char*>(ptr), 0xAA);

        // Clean up
        alignedFree(ptr);
    }
}

// Test 3: Large allocations and large alignment boundaries
TEST(AlignedMallocTest, LargeAllocationAndAlignment) {
    size_t size = 1024 * 1024; // 1 MB
    size_t alignment = 4096;   // Page alignment

    void* ptr = alignedMalloc(size, alignment);
    ASSERT_NE(ptr, nullptr);

    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    EXPECT_EQ(addr & (alignment - 1), 0);

    // Write to the beginning and end of the buffer to ensure boundary validity
    char* char_ptr = static_cast<char*>(ptr);
    char_ptr[0] = 'A';
    char_ptr[size - 1] = 'Z';

    EXPECT_EQ(char_ptr[0], 'A');
    EXPECT_EQ(char_ptr[size - 1], 'Z');

    alignedFree(ptr);
}

// Test 4: Passing a NULL pointer to alignedFree should safely no-op
TEST(AlignedMallocTest, FreeHandlesNullPointer) {
    // Should not crash or segment fault
    EXPECT_NO_THROW(alignedFree(nullptr));
}

// Test 5: Corruption Test with canary value modified should safely no-op without calling free()
TEST(AlignedMallocTest, HeapCorruption) {
    
    size_t size = 1024;
    size_t alignment = 8;

    void* ptr = alignedMalloc(size, alignment);
    ASSERT_NE(ptr, nullptr);

    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    EXPECT_EQ(addr & (alignment - 1), 0);

    // Write to the beginning and end of the buffer to ensure boundary validity
    char* char_ptr = static_cast<char*>(ptr);
    char_ptr[0] = 'A';
    char_ptr[size - 1] = 'Z';

    EXPECT_EQ(char_ptr[0], 'A');
    EXPECT_EQ(char_ptr[size - 1], 'Z');

    // Corrupt heap
    char_ptr[-1] = 'W';
    char_ptr[-2] = 'Z';
    
    // Should not crash or segment fault
    EXPECT_NO_THROW(alignedFree(nullptr));
}
