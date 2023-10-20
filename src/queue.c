#include "channel.h"

#include <pthread.h>
#include "lock_free_queue.h"

typedef struct LockFreeBlockQueue LockFreeBlockQueue;
struct LockFreeBlockQueue {
    pthread_mutex_t wMutex;
    pthread_mutex_t rMutex;
    pthread_cond_t cond;
    LockFreeQueue *queue;
};

channel channel_new(const char *name, uint32_t size, uint32_t count)
{
    LockFreeBlockQueue *queue = (LockFreeBlockQueue *)malloc(sizeof(LockFreeBlockQueue));
    if (queue == NULL) {
        return NULL;
    }
    queue->queue = LFQUE_Create(size, count);
    if (queue->queue == NULL) {
        free(queue);
        return NULL;
    }
    pthread_mutex_init(&queue->wMutex, NULL);
    pthread_mutex_init(&queue->rMutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
    return (channel)queue;
}

int32_t channel_push(channel c, void *data, uint32_t size, int32_t timeout)
{
    if (c == NULL || data == NULL || timeout > 0) {
        return CHANNEL_ERR;
    }
    LockFreeBlockQueue *queue = (LockFreeBlockQueue *)c;
    pthread_mutex_lock(&queue->wMutex);
    int ret = LFQUE_Push(queue->queue, data, 0);
    pthread_mutex_unlock(&queue->wMutex);
    pthread_mutex_lock(&queue->rMutex);
    pthread_cond_broadcast(&queue->cond);
    pthread_mutex_unlock(&queue->rMutex);
    return ret;
}

int32_t channel_pop(channel c, void *data, uint32_t size, int32_t timeout)
{
    if (c == NULL || data == NULL || timeout > 0) {
        return CHANNEL_ERR;
    }

    LockFreeBlockQueue *queue = (LockFreeBlockQueue *)c;
    pthread_mutex_lock(&queue->rMutex);
    while (LFQUE_Pop(queue->queue, data, 0) != CHANNEL_OK) {
        pthread_cond_wait(&queue->cond, &queue->rMutex);
    }
    pthread_mutex_unlock(&queue->rMutex);
    return CHANNEL_OK;
}

int32_t channel_del(channel c)
{
    if (c == NULL) {
        return CHANNEL_ERR;
    }

    LockFreeBlockQueue *queue = (LockFreeBlockQueue *)c;
    pthread_mutex_destroy(&queue->wMutex);
    pthread_mutex_destroy(&queue->rMutex);
    pthread_cond_destroy(&queue->cond);
    free(queue->queue);
    free(queue);
    return CHANNEL_OK;
}
