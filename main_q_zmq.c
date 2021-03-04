

#include "settings.h"
#include "utils.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>

// QUEUE

// SHARED

// THREAD

pthread_t make_thread(void *(*entry)(void *), void *shared)
{
    int       ret;
    pthread_t thread;

    ret = pthread_create(&thread, NULL, entry, (void *)shared);
    if (ret != 0)
    {
        perror_exit("pthread_create failed");
    }
    return thread;
}

void join_thread(pthread_t thread)
{
    int ret = pthread_join(thread, NULL);
    if (ret == -1)
    {
        perror_exit("pthread_join failed");
    }
}

// PRODUCER-CONSUMER

void *producer_entry(void *arg)
{
    int   i;
    void *context = (void *)arg;

    void *xmitter = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(xmitter, "inproc://queue");

    for (i = 0; i < NUM_ITEMS; i++)
    {
        // printf("adding item %d\n", i);
        zmq_send(xmitter, &i, 1, 0);
    }

    zmq_close(xmitter);
    pthread_exit(NULL);
}

void *consumer_entry(void *arg)
{
    enum
    {
        cap = 16
    };
    int   i;
    int   item;
    void *context = (void *)arg;
    char  buffer[cap];

    void *receiver = zmq_socket(context, ZMQ_PAIR);
    zmq_bind(receiver, "inproc://queue");

    for (i = 0; i < NUM_ITEMS; i++)
    {
        zmq_recv(receiver, buffer, cap - 1, 0);
        item = (int)buffer[0];
        // printf("consuming item %d\n", item);
    }
    zmq_close(receiver);
    pthread_exit(NULL);
}

// TEST CODE

int main()
{
    int       i;
    pthread_t child[NUM_CHILDREN];
    void *    context = zmq_ctx_new();

    child[0] = make_thread(producer_entry, context);
    child[1] = make_thread(consumer_entry, context);

    for (i = 0; i < NUM_CHILDREN; i++)
    {
        join_thread(child[i]);
    }

    return 0;
}
