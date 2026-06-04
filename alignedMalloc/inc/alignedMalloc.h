// A custom malloc wrapper returning memory aligned to N-byte boundaries.

#ifndef ALIGNED_MALLOC_H
#define ALIGNED_MALLOC_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Enforce structured metadata layout
typedef struct {
    void* original_ptr;
    uint32_t canary;
} MallocHeader;

void * alignedMalloc(size_t size, size_t alignment);
void alignedFree(void * aligned_ptr);

#ifdef __cplusplus
}
#endif

#endif // ALIGNED_MALLOC_H