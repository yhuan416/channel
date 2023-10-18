#include <stdio.h>
#include <string.h>

#include <pthread.h>

#include "channel.h"

#define MSG_SIZE    (1024)

void *routine(void *arg)
{
    char buf[MSG_SIZE];
    int32_t ret;

    channel ch = (channel)arg;

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        ret = channel_pop(ch, buf, 1024, CHANNEL_WAIT_FOREVER);
        if (ret != CHANNEL_OK) {
            printf("channel_pop error\n");
            return NULL;
        }

        printf("recv[%d]: %s\n", ret, buf);

        if (strcmp(buf, "exit") == 0) {
            break;
        }
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    int32_t ret = 0;
    pthread_t tid;

    channel ch = channel_new("test", MSG_SIZE, 10);
    if (ch == NULL) {
        printf("channel_new error\n");
        return -1;
    }

    // Create a thread
    pthread_create(&tid, NULL, routine, ch);

    channel_push(ch, "hello world1", strlen("hello world1"), CHANNEL_WAIT_FOREVER);
    channel_push(ch, "hello world2", strlen("hello world2"), CHANNEL_WAIT_FOREVER);
    channel_push(ch, "exit", strlen("exit"), CHANNEL_WAIT_FOREVER);

    // Wait for the thread to exit
    pthread_join(tid, NULL);

    channel_del(ch);
    ch = NULL;

    return 0;
}
