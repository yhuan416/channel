#include "channel.h"



typedef struct channel_s 
{
    void *data;
    void *priv;
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
    channel_t *ch = (channel_t *)channel_calloc(sizeof(channel_t), 1);
    if (ch == NULL) {
        return NULL;
    }

    return ch;
}

int32_t channel_del(channel c)
{
    channel_t *ch = (channel_t *)c;

    if (ch == NULL) {
        return CHANNEL_ERR;
    }

    channel_free(ch);
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
