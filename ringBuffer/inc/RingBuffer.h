#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *buffer;
    size_t size;      // Must be a power of two
    size_t mask;      // size - 1
    size_t head;      // Write pointer
    size_t tail;      // Read pointer
} ring_buffer_t;

// Initialize buffer
bool rb_init(ring_buffer_t* rb, size_t size);

// Write data to buffer
bool rb_enqueue(ring_buffer_t* rb, uint8_t data);

// Read data from buffer
bool rb_dequeue(ring_buffer_t* rb, uint8_t* data);

// Check if buffer is full
bool rb_is_full(ring_buffer_t* rb);

// Check if buffer is empty
bool rb_is_empty(ring_buffer_t* rb);

#ifdef __cplusplus
}
#endif

#endif