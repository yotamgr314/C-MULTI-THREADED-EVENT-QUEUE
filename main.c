#include <stdio.h>
#include "shared_event_queue.h"


#define NUM_THREADS 3


void* producer_thread_func(void *args)
{
    // recivies shared_event_queueu as arguments (passed behind the scenese to void *args parameter.)
    shared_event_queue* shared_event_queue = (shared_event_queue*) args;
    // int asprintf(char **pointer-to-Char*, const char *format, ...); NOTE: asprintf automatically allocates memory to fit the formatted string, including the null terminator (\0).
    char* message; 
    // it will allocate enough space in message for "hey, i am thread num <thread_id>, and this is message number <current_amount_of_messages_in_the_q>". (replaces malloc, strcpy, strcat).
    asprintf(&message, "hey, I am thread num %ld and this message number %d", (long) pthread_self(), queue->count); // The `asprintf` function handles both the memory allocation and string formatting in a single step.


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






// TO INSERT INSIDE THE PTHREAD_PRODUCER_FUNC

void queue_enqueue_event_node(shared_event_queue* shared_event_queue, char* new_message) 
{
    event_node* new_event_node = initlize_node(new_message);

    // mutex lock
    if(shared_event_queue->tail == NULL) // if the queue is empty. 
    {
        shared_event_queue->head = new_event_node;
        shared_event_queue->tail = new_event_node;
        pthread_cond_signal(queue_not_empty); // signal the waiting thread that the queue is no longer empty. 
        shared_event_queue->tail = new_event_node;
    }else {
            shared_event_queue->tail = new_event_node;
    }
    // mutex unlock
}



// TO INSERT INSIDE THE PTHREAD_CONSUMER_FUNC
void queue_dequeue_event_node(shared_event_queue* shared_event_queue, char* new_message)
{
    // mutex lock
    while(shared_event_queue->head == NULL)
    {
        pthread_cond_wait(&mutex,shared_event_queue->cond_event_queue_not_empty);
    }
    
    event_node* new_event_node = shared_event_queue->head; 
    // consume task 
    // remove string allocated by consumer thread.
    // remove dynmic allocated node. 

}
