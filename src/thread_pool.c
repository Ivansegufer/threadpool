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

ThreadJob thread_job_constructor(thread_job_t job_function, void *args)
{
    ThreadJob job;
    job.job = job_function;
    job.args = args;
    return job;
}

void thread_pool_destructor(ThreadPool *thread_pool)
{
    thread_pool->active = 0;
    for (int i = 0; i < thread_pool->num_threads; i++)
    {
        pthread_cond_signal(&thread_pool->signal);
    }
    for (int i = 0; i < thread_pool->num_threads; i++)
    {
        pthread_join(thread_pool->pool[i], NULL);
    }
    free(thread_pool->pool);
    queue_destructor(&thread_pool->work);
}

void *thread_job(void *arg)
{
    ThreadPool *thread_pool = (ThreadPool *)arg;
    while (thread_pool->active == 1)
    {
        pthread_mutex_lock(&thread_pool->lock);
        pthread_cond_wait(&thread_pool->signal, &thread_pool->lock);
        ThreadJob job = *(ThreadJob *)thread_pool->work.peek(&thread_pool->work);
        thread_pool->work.pop(&thread_pool->work);
        pthread_mutex_unlock(&thread_pool->work);
        if (job.job)
        {
            job.job(job.args);
        }
    }
    return NULL;
}