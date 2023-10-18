#include "channel.h"

#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

typedef struct channel_posix_s
{
    char *name;
    mqd_t id;
    uint32_t size;
} channel_t;

static void *channel_calloc(uint32_t size, uint32_t count)
{
    return calloc(count, size);
}

static void *channel_free(void *ptr)
{
    free(ptr);
}

channel channel_new(const char *name, uint32_t size, uint32_t count)
{
    mqd_t new_mq;
    struct mq_attr attr;

    channel_t *ch = (channel_t *)channel_calloc(sizeof(channel_t), 1);
    if (ch == NULL) {
        return NULL;
    }

    // Set the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = count;
    attr.mq_msgsize = size;
    attr.mq_curmsgs = 0;

    // Create the queue
    new_mq = mq_open(name, O_CREAT | O_RDWR, 0644, &attr);
    if (new_mq == (mqd_t) -1) {
        perror("mq_open");
        return -1;
    }

    ch->name = name;
    ch->id = new_mq;
    ch->size = size;

    return (channel)ch;
}

int32_t channel_del(channel c)
{
    channel_t *ch = (channel_t *)c;

    if (ch == NULL) {
        return CHANNEL_ERR;
    }

    // Close the queue
    if (mq_close(ch->id) == -1) {
        perror("mq_close");
        return CHANNEL_ERR;
    }

    // Delete the queue
    if (mq_unlink(ch->name) == -1) {
        perror("mq_unlink");
        return CHANNEL_ERR;
    }

    return CHANNEL_OK;
}

int32_t channel_push(channel c, void *data)
{
    channel_t *ch = (channel_t *)c;

    if (ch == NULL) {
        return CHANNEL_ERR;
    }


}

int32_t channel_pop(channel c, void *data)
{
    channel_t *ch = (channel_t *)c;

    if (ch == NULL) {
        return CHANNEL_ERR;
    }


}
