#include <stdio.h>
#include "shared_event_queue.h"


#define NUM_THREADS 3


void *producer_thread_func(void *args)
{
    // recivies shared_event_queueu as arguments (passed behind the scenese to void *args parameter.)
    shared_event_queue* shared_event_queue = (shared_event_queue*) args;
    // dynamicly allocate memory for the newly event_task(eg.. message)
    // enqueue a new task_event by calling the enqueue function to the shared_event_queue. (eg.. inserting the new task.)


}

void *consumer_thread_func(void *args)
{
    // recivies shared_event_queueu as arguments (passed behind the scenese to void *args parameter.)

    shared_event_queue* shared_event_queue = (shared_event_queue*)args;
    // deqeueu an event by calling the dequeue function.
    // consumes the event_task which the dequeued event_node contained...(eg.. print the message to the screen.)
    // free the dynamic memory allocated for the consumed task. (eg.. pthread_cleanup_pop())
}


void init_thread_func(shared_event_queue** shared_event_queue)
{
    pthread_t consumer_threads[NUM_THREADS];
    pthread_t producer_threads[NUM_THREADS];

    // producer init
    for(int i = 0; i<NUM_THREADS; i++)
    {
        pthread_create(consumer_threads[i],NULL,producer_thread_func(), &shared_event_queue);
    }

    // consumer init
    for(int j= 0; j <NUM_THREADS; j++)
    {
        pthread_create(producer_threads[j],NULL,consumer_thread_func(), &shared_event_queue);
    }
}



int main ()
{
    shared_event_queue* shared_event_queue =queue_init();

    init_thread_func(&shared_event_queue);
    // sleep()

    return 0;
}


