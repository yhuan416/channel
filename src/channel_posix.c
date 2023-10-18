#include "channel.h"

#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

typedef struct channel_posix_s
{
    mqd_t id;
    uint32_t msg_size;
    char name[0];
} channel_t;

static void *channel_calloc(uint32_t size, uint32_t count)
{
    return calloc(count, size);
}

static void *channel_free(void *ptr)
{
    free(ptr);
}

/*
if name[0] == '/', then name is absolute path
else name is relative path
*/
static int is_absolute_name(const char *name)
{
    // if name[0] == '/', then name is absolute path
    if (name[0] != '/') {
        return 0;
    }

    return 1;
}

channel channel_new(const char *name, uint32_t size, uint32_t count)
{
    mqd_t new_mq;
    struct mq_attr attr;
    uint32_t malloc_size = sizeof(channel_t) + strlen(name) + 1;
    int is_absolute_flag = is_absolute_name(name);

    if (!is_absolute_flag) {
        malloc_size += 1;
    }

    channel_t *ch = (channel_t *)channel_calloc(malloc_size, 1);
    if (ch == NULL) {
        return NULL;
    }

    // Set the name
    if (is_absolute_flag) {
        snprintf(ch->name, malloc_size - sizeof(channel_t), "%s", name);
    } else {
        snprintf(ch->name, malloc_size - sizeof(channel_t), "/%s", name);
    }

    // Set the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = count;
    attr.mq_msgsize = size;
    attr.mq_curmsgs = 0;

    // Create the queue
    new_mq = mq_open(ch->name, O_CREAT | O_RDWR | O_EXCL, 0644, &attr);
    if (new_mq == (mqd_t) -1) {

        if (errno ==  EEXIST) {
            // The queue already exists
            perror("mq_open");
            return NULL;
        }

        perror("mq_open");
        return NULL;
    }

    ch->id = new_mq;
    ch->msg_size = size;

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
    }

    // Delete the queue
    if (mq_unlink(ch->name) == -1) {
        perror("mq_unlink");
    }

    // Free the memory
    channel_free(ch);

    return CHANNEL_OK;
}

int32_t channel_push(channel c, void *data, uint32_t size, int32_t timeout)
{
    int ret = 0;
    channel_t *ch = (channel_t *)c;
    const struct timespec *abs_timeout = NULL;

    if (ch == NULL) {
        return CHANNEL_ERR;
    }

    // Send the message
    ret = mq_timedsend(ch->id, data, size, 0, abs_timeout);
    if (ret == -1) {
        // on error, return -1, on success return 0
        if (errno == ETIMEDOUT) {
            return CHANNEL_TIMEOUT;
        }

        return CHANNEL_ERR;
    }

    return CHANNEL_OK;
}

int32_t channel_pop(channel c, void *data, uint32_t size, int32_t timeout)
{
    int ret = 0;
    channel_t *ch = (channel_t *)c;
    const struct timespec *abs_timeout = NULL;

    if (ch == NULL) {
        return CHANNEL_ERR;
    }

    // Receive the message
    ret = mq_timedreceive(ch->id, data, size, NULL, abs_timeout);
    if (ret == -1) {
        // on error, return -1, on success return received bytes
        if (errno == ETIMEDOUT) {
            return CHANNEL_TIMEOUT;
        }

        return CHANNEL_ERR;
    }

    return CHANNEL_OK;
}
