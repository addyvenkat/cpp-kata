#include "RingBuffer.h"

bool rb_init(ring_buffer_t* rb, size_t size)
{
    // Size power of 2
    if((size & (size-1)) != 0 || size == 0) {
        return false;
    }

    rb->buffer = malloc(size);
    if(!rb->buffer) {
        return false;
    }

    rb->size = size;
    rb->mask = size - 1;
    rb->head = 0;
    rb->tail = 0;
    return true;
}

bool rb_is_full(ring_buffer_t* rb)
{
    return (((rb->head + 1) & rb->mask) == rb->tail);
}

bool rb_is_empty(ring_buffer_t* rb)
{
    return rb->head == rb->tail;
}

bool rb_enqueue(ring_buffer_t* rb, uint8_t data)
{
    if(rb_is_full(rb)) {
        return false;
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) & rb->mask;
    return true;
}

bool rb_dequeue(ring_buffer_t* rb, uint8_t * data)
{
    if(rb_is_empty(rb)) {
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & rb->mask;
    return true;
}