#include <stdio.h>

#define NUM_THREADS 3

Enqueue()
{

}

void init_thread_func()
{
    pthread_t consumer_threads[NUM_THREADS];
    pthread_t producer_threads[NUM_THREADS];

    for(int i = 0; i<NUM_THREADS; i++)
    {
        pthread_create(consumer_threads[i],NULL,Enqueue(), &LL_queue);
    }
}

int main ()
{

// to put inside a initThreadsFunc()
    return 0;
}