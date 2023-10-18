#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum channel_ret {
    CHANNEL_OK = 0,
    CHANNEL_ERR = -1,
    CHANNEL_TIMEOUT = -2,
};

#define CHANNEL_WAIT_FOREVER    ((int32_t)-1)

typedef void *channel;

/**
 * @brief Create a new channel object
 * 
 * @param name The name of the channel
 * @param size The size of the singal message
 * @param count The maximum number of messages
 * 
 * @return channel
*/
channel channel_new(const char *name, uint32_t size, uint32_t count);

/**
 * @brief Delete the channel object
 * 
 * @param c The channel object
 * 
 * @return @{enum channel_ret}
*/
int32_t channel_del(channel c);

/**
 * @brief Push a message to the channel
 * 
 * @param c The channel object
 * @param data The message to be sent
 * @param size The size of the message
 * @param timeout The timeout value
 * 
 * @return @{enum channel_ret}
*/
int32_t channel_push(channel c, void *data, uint32_t size, int32_t timeout);

/**
 * @brief Pop a message from the channel
 * 
 * @param c The channel object
 * @param data The buffer to store the message
 * @param size The size of the buffer
 * @param timeout The timeout value
 * 
 * @return @{enum channel_ret}
*/
int32_t channel_pop(channel c, void *data, uint32_t size, int32_t timeout);

#endif // !_CHANNEL_H_
