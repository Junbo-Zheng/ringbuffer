#include <assert.h>
#include <stdio.h>

#include "./src/ringbuffer.h"

int main(void)
{
    int i, cnt;
    uint8_t buf;
    uint8_t buf_arr[50];

    ringbuffer_t ringbuffer;
    ringbuffer_init(&ringbuffer);

    for (i = 0; i < 50; i++) {
        ringbuffer_queue(&ringbuffer, i);
    }
    printf("## length is %d\r\n", ringbuffer_length(&ringbuffer));

    /* Peek the third element */
    cnt = ringbuffer_peek(&ringbuffer, &buf, 3);
    printf("## cnt is %d, buf is %d\r\n", cnt, buf);

    printf("##ringbuffer_dequeue\n");
    for (cnt = 0; ringbuffer_dequeue(&ringbuffer, &buf) > 0; cnt++) {
        printf("%d ", buf);
    }
    printf("\r\n");
    printf("## length is %d\r\n", ringbuffer_length(&ringbuffer));

    printf("## Add array for \"Hello, Ring Buffer!\"\r\n");
    ringbuffer_queue_arr(&ringbuffer, (const uint8_t*)("Hello, Ring Buffer!"),
                         19);
    printf("## length is %d\r\n", ringbuffer_length(&ringbuffer));

    /* Dequeue all elements */
    printf("## ringbuffer_dequeue\r\n");
    while (ringbuffer_dequeue(&ringbuffer, &buf) > 0) {
        /* Print contents */
        printf("%c ", buf);
    }
    printf("\r\n");
    printf("## leng is %d\r\n", ringbuffer_length(&ringbuffer));

    printf("## Add array for \"Hello again, Ring Buffer!\"\r\n");
    ringbuffer_queue_arr(&ringbuffer,
                         (const uint8_t*)("Hello again, Ring Buffer!"), 25);
    printf("## length is %d\r\n", ringbuffer_length(&ringbuffer));

    /* Dequeue array in two parts */
    cnt = ringbuffer_dequeue_arr(&ringbuffer, buf_arr, 13);
    printf("## dequeue_arr cnt is is %d\n", cnt);

    buf_arr[13] = '\0';
    printf("%s\n", buf_arr);

    /* Dequeue remaining */
    cnt = ringbuffer_dequeue_arr(&ringbuffer, buf_arr, 13);
    printf("## dequeue_arr cnt is is %d\n", cnt);
    printf("%s\r\n", buf_arr);

    printf("----------overfill buffer test----------\r\n");
    for (i = 0; i < 300; i++) {
        ringbuffer_queue(&ringbuffer, (i % RINGBUFFER_SIZE));
    }
    printf("## length is %d\r\n", ringbuffer_length(&ringbuffer));

    if (ringbuffer_is_full(&ringbuffer)) {
        cnt = ringbuffer_length(&ringbuffer);
        printf("Buffer is full and ringbuffer_length %d\r\n", cnt);
    }

    /* Dequeue all elements */
    printf("## ringbuffer_dequeue:");
    while (ringbuffer_dequeue(&ringbuffer, &buf) > 0) {
        printf("%d ", buf);
    }
    printf("\r\n");

    printf("## length is %d\r\n", ringbuffer_length(&ringbuffer));

    return 0;
}
