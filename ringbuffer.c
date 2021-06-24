#include <stdio.h>

#include "ringbuffer.h"

#if defined(CONFIG_RINGBUFFER_LOCK)
#include <pthread.h>
pthread_mutex_t ringbuffer_lock;
#endif

void ringbuffer_init(ringbuffer_t* buffer)
{
    buffer->tail_index = 0;
    buffer->head_index = 0;
    ringbuffer_lock_init();
}

void ringbuffer_queue(ringbuffer_t* buffer, uint8_t data)
{
    ringbuffer_lock();
    /* overwrite the oldest byte if the buffer is full */
    if (ringbuffer_is_full(buffer)) {
        buffer->tail_index = ((buffer->tail_index + 1) & RINGBUFFER_MASK);
    }

    buffer->buffer[buffer->head_index] = data;
    buffer->head_index = ((buffer->head_index + 1) & RINGBUFFER_MASK);
    ringbuffer_unlock();
}

void ringbuffer_queue_arr(ringbuffer_t* buffer, const uint8_t* data,
                          ringbuffer_size_t size)
{
    for (ringbuffer_size_t i = 0; i < size; i++) {
        ringbuffer_queue(buffer, data[i]);
    }
}

uint8_t ringbuffer_dequeue(ringbuffer_t* buffer, uint8_t* data)
{
    ringbuffer_lock();
    if (ringbuffer_is_empty(buffer)) {
        ringbuffer_unlock();
        return 0;
    }

    *data = buffer->buffer[buffer->tail_index];
    buffer->tail_index = ((buffer->tail_index + 1) & RINGBUFFER_MASK);
    ringbuffer_unlock();
    return 1;
}

ringbuffer_size_t ringbuffer_dequeue_arr(ringbuffer_t* buffer, uint8_t* data,
                                         ringbuffer_size_t len)
{
    if (ringbuffer_is_empty(buffer)) {
        return 0;
    }

    uint8_t* data_ptr = data;
    ringbuffer_size_t cnt = 0;
    while ((cnt < len) && ringbuffer_dequeue(buffer, data_ptr)) {
        cnt++;
        data_ptr++;
    }

    return cnt;
}

uint8_t ringbuffer_peek(ringbuffer_t* buffer, uint8_t* data,
                        ringbuffer_size_t index)
{
    ringbuffer_lock();
    if (index >= ringbuffer_length(buffer)) {
        ringbuffer_unlock();
        return 0;
    }

    /* Add index to pointer */
    ringbuffer_size_t data_index
        = ((buffer->tail_index + index) & RINGBUFFER_MASK);
    *data = buffer->buffer[data_index];
    ringbuffer_unlock();
    return 1;
}
