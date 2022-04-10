#include <stdlib.h>
#include "thread_pool.h"

void *thread_job(void *arg);

ThreadPool thread_pool_constructor(int num_threads)
{
    ThreadPool thread_pool;
    thread_pool.num_threads = num_threads;
    thread_pool.active = 1;
    thread_pool.pool = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    for (int i = 0; i < num_threads; i++)
    {
        // Verify if is necesary pass reference of pool index
        pthread_create(&thread_pool.pool[i], NULL, thread_job, NULL);
    }
    thread_pool.work = queue_constructor();
    thread_pool.lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    thread_pool.signal = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    return thread_pool;
}