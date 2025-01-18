#include <stdio.h>
#include "shared_event_queue.h"

#define NUM_THREADS 3


void* producer_thread_func(void *args)
{
    shared_event_queue* shared_events_queue = (shared_event_queue*) args;

    pthread_mutex_lock(&shared_events_queue->dm_queue_mutex);

    char* newMessage; 
    asprintf(&newMessage, "hey, I am thread num %ld and this message number %d", (long) pthread_self(), shared_events_queue->dm_count); // The `asprintf` function handles both the memory allocation and string formatting in a single step. it will allocate enough space in message for "hey, i am thread num <thread_id>, and this is message number <current_amount_of_messages_in_the_q>". (replaces malloc, strcpy, strcat).

    event_node* new_event_node = initlize_node(newMessage);

    if(shared_events_queue->dm_tail == NULL) // if the queue is empty. 
    {
        shared_events_queue->dm_tail = new_event_node;
    }else{
        shared_events_queue->dm_tail->next = new_event_node;
    }




    // dynamicly allocate memory for the newly event_task(eg.. message)
    // enqueue a new task_event by calling the enqueue function to the shared_event_queue. (eg.. inserting the new task.)
}




void *consumer_thread_func(void *args)
{
    
    // recivies shared_event_queueu as arguments (passed behind the scenese to void *args parameter.)

    shared_event_queue* shared_events_queue = (shared_event_queue*)args;

    // deqeueu an event by calling the dequeue function.
    // consumes the event_task which the dequeued event_node contained...(eg.. print the message to the screen.)
    // free the dynamic memory allocated for the consumed task. (eg.. pthread_cleanup_pop())


        while(shared_events_queue->dm_tail == NULL)
        {
        pthread_cond_wait(&shared_events_queue->dm_cond_queue_not_empty,&shared_events_queue->dm_queue_mutex);

        }
}






void init_thread_func(shared_event_queue** shared_events_queue)
{
    pthread_t consumer_threads[NUM_THREADS];
    pthread_t producer_threads[NUM_THREADS];

    // producer init
    for(int i = 0; i<NUM_THREADS; i++)
    {
        pthread_create(consumer_threads[i],NULL,producer_thread_func, &shared_events_queue);
    }

    // consumer init
    for(int j= 0; j <NUM_THREADS; j++)
    {
        pthread_create(producer_threads[j],NULL,consumer_thread_func, &shared_events_queue);
    }
}



int main ()
{
    shared_event_queue* shared_event_queue =queue_init();

    init_thread_func(&shared_event_queue);
    // sleep()

    return 0;
}





