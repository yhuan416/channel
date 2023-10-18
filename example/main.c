#include <stdio.h>

#include <pthread.h>

#include "channel.h"

void *routine(void *arg)
{
    printf("Hello world!\n");
}

int main(int argc, char const *argv[])
{
    channel ch = channel_new("/test", 1024, 10);
    if (ch == NULL) {
        printf("channel_new error\n");
        return -1;
    } else {
        printf("channel_new success\n");
    }

    channel_del(ch);

    // pthread_t tid;
    // pthread_create(&tid, NULL, routine, NULL);




    // pthread_join(tid, NULL);

    return 0;
}
