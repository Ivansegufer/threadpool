#ifndef thread_pool_h
#define thread_pool_h

#include "../lib/Queue.h"

#include <pthread.h>

typedef void *(*thread_job_t)(void *args);
typedef struct ThreadPool ThreadPool;
typedef struct ThreadJob ThreadJob;

struct ThreadPool
{
    int num_threads;
    int active;
    struct Queue work;
    pthread_t *pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;
};

struct ThreadJob
{
    thread_job_t job;
    void *args;
};

ThreadPool thread_pool_constructor(int num_threads);
ThreadJob thread_job_constructor(thread_job_t job_function, void *args);

#endif