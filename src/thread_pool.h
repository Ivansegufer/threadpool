#ifndef thread_pool_h
#define thread_pool_h

#include "../lib/Queue.h"

#include <pthread.h>

struct ThreadPool
{
    int num_threads;
    int active;
    struct Queue work;
    pthread_t *pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;
};

#endif