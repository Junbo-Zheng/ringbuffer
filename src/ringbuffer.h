#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <inttypes.h>

// #define CONFIG_RINGBUFFER_LOCK
#define RINGBUFFER_SIZE 256

#define RINGBUFFER_MASK (RINGBUFFER_SIZE - 1)

#if (RINGBUFFER_SIZE & (RINGBUFFER_SIZE - 1)) != 0
#error "RINGBUFFER_SIZE must be a power of two"
#endif

#if defined(CONFIG_RINGBUFFER_LOCK)
#define ringbuffer_lock_init() pthread_mutex_init(&ringbuffer_lock, NULL)
#define ringbuffer_lock()      pthread_mutex_lock(&ringbuffer_lock);
#define ringbuffer_unlock()    pthread_mutex_unlock(&ringbuffer_lock);
#else
#define ringbuffer_lock_init()
#define ringbuffer_lock()
#define ringbuffer_unlock()
#endif

typedef uint8_t ringbuffer_size_t;

typedef struct _ringbuffer_t {
    uint8_t buffer[RINGBUFFER_SIZE];
    ringbuffer_size_t tail_index;
    ringbuffer_size_t head_index;
} ringbuffer_t;

void ringbuffer_init(ringbuffer_t* buffer);

void ringbuffer_queue(ringbuffer_t* buffer, uint8_t data);

void ringbuffer_queue_arr(ringbuffer_t* buffer, const uint8_t* data,
                          ringbuffer_size_t size);

uint8_t ringbuffer_dequeue(ringbuffer_t* buffer, uint8_t* data);

ringbuffer_size_t ringbuffer_dequeue_arr(ringbuffer_t* buffer, uint8_t* data,
                                         ringbuffer_size_t len);

uint8_t ringbuffer_peek(ringbuffer_t* buffer, uint8_t* data,
                        ringbuffer_size_t index);

static inline uint8_t ringbuffer_is_empty(ringbuffer_t* buffer)
{
    return (buffer->head_index == buffer->tail_index);
}

static inline uint8_t ringbuffer_is_full(ringbuffer_t* buffer)
{
    return ((buffer->head_index - buffer->tail_index) & RINGBUFFER_MASK)
        == RINGBUFFER_MASK;
}

static inline ringbuffer_size_t ringbuffer_length(ringbuffer_t* buffer)
{
    return ((buffer->head_index - buffer->tail_index) & RINGBUFFER_MASK);
}

#endif /* _RINGBUFFER_H_ */
