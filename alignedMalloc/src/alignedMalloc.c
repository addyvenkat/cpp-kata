// A custom malloc wrapper returning memory aligned to N-byte boundaries.

#include "alignedMalloc.h"

#define CANARY_VALUE 0xDEADBEEF

void * alignedMalloc(size_t size, size_t alignment) {
    // Validate alignment is a power of 2 and greater than 0
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        return NULL;
    }
    
    // We need enough room for the metadata block plus alignment padding
    size_t total_size = size + sizeof(MallocHeader) + (alignment - 1);
    
    void * original_ptr = malloc(total_size);
    if (original_ptr == NULL) {
        return NULL;
    }
    
    // Calculate aligned pointer position, ensuring room for the Header right before it
    uintptr_t raw_addr = (uintptr_t)original_ptr + sizeof(MallocHeader);
    void * aligned_ptr = (void*)((raw_addr + (alignment - 1)) & ~(alignment - 1));

    // Safely write metadata using the structured layout
    MallocHeader* header = (MallocHeader*)((uint8_t*)aligned_ptr - sizeof(MallocHeader));
    header->original_ptr = original_ptr;
    header->canary = CANARY_VALUE;

    return aligned_ptr;
}

void alignedFree(void * aligned_ptr) {
    if (aligned_ptr == NULL)
        return;

    // Safely access metadata via the structure pointer
    MallocHeader* header = (MallocHeader*)((uint8_t*)aligned_ptr - sizeof(MallocHeader));

    // Security vulnerability check
    if (header->canary != CANARY_VALUE) {
        printf("Heap Corrupted!!!\n");
        return;
    }

    if (header->original_ptr) {
        free(header->original_ptr);
    }
}