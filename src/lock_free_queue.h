#ifndef LITE_LOCK_FREE_QUEUE_H
#define LITE_LOCK_FREE_QUEUE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct LockFreeQueue LockFreeQueue;
struct LockFreeQueue {
    uint32_t write;
    uint32_t read;
    uint32_t itemSize;
    uint32_t totalSize;
    uint8_t buffer[0];
};
LockFreeQueue *LFQUE_Create(int size, int count);

int LFQUE_IsEmpty(LockFreeQueue *queue);

int LFQUE_IsFull(LockFreeQueue *queue);

int LFQUE_Push(LockFreeQueue *queue, const void *element, uint8_t pri);

int LFQUE_Pop(LockFreeQueue *queue, void *element, uint8_t *pri);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif // LITE_LOCK_FREE_QUEUE_H
