#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum channel_ret {
    CHANNEL_OK = 0,
    CHANNEL_ERR = -1,
};

typedef void *channel;

channel channel_new(const char *name, uint32_t size, uint32_t count);

int32_t channel_del(channel c);

int32_t channel_push(channel c, void *data);

int32_t channel_pop(channel c, void *data);

#endif // !_CHANNEL_H_
