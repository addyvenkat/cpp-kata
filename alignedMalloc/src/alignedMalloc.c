// A custom malloc wrapper returning memory aligned to N-byte boundaries.

#include "alignedMalloc.h"

void * alignedMalloc(size_t size, size_t alignment) {
    // Validate alignment is a power of 2 and greater than 0
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        return NULL;
    }
    
    // Calculate total allocation size
    size_t total_size = size + sizeof(void*) + (alignment - 1);
    
    // Standard malloc call
    void * original_ptr = malloc(total_size);
    if (original_ptr == NULL) {
        return NULL;
    }
    
    // Calculate aligned pointer position
    void * aligned_ptr = (void*)(((uintptr_t)original_ptr + sizeof(void*) + (alignment - 1)) & ~(alignment - 1));

    // Store bookkeeping pointer right before aligned_ptr
    ((void**)aligned_ptr)[-1] = original_ptr;

    return aligned_ptr;
}

void alignedFree(void * aligned_ptr) {
    if(aligned_ptr == NULL)
        return;

    // Fetch the stored pointer
    void * original_ptr = ((void**)aligned_ptr)[-1];
    if(original_ptr)
        free(original_ptr);
    return;
}