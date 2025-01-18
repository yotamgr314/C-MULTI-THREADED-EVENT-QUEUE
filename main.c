#include <stdio.h>
#include "shared_event_queue.h"

#define NUM_THREADS 3
#define NUM_CYCLES 10 


void* producer_thread_func(void *args)
{
    for(int k=0; k < NUM_CYCLES; k++)
    {
        for(int j = 0; j<NUM_THREADS; j ++)
        {
            shared_event_queue* shared_events_queue = (shared_event_queue*) args;

            pthread_mutex_lock(&shared_events_queue->dm_queue_mutex);

            char* newMessage; 
            asprintf(&newMessage, "hey, I am thread num %ld and this message number %d", (long) pthread_self(), shared_events_queue->dm_count); // The `asprintf` function handles both the memory allocation and string formatting in a single step. it will allocate enough space in message for "hey, i am thread num <thread_id>, and this is message number <current_amount_of_messages_in_the_q>". (replaces malloc, strcpy, strcat).

            event_node* new_event_node = initlize_node(newMessage);

            if(shared_events_queue->dm_tail == NULL) // if the queue is empty 
            {
                shared_events_queue->dm_tail = new_event_node;
                shared_events_queue->dm_head = new_event_node;
            }else{
                shared_events_queue->dm_tail->next = new_event_node; // adds new node to the end of the queue.
            }

            shared_events_queue->dm_count++;
            pthread_cond_signal(&shared_events_queue->dm_cond_queue_not_empty); // signal the waiting thread that the queue is no longer empty. 

            pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);

        }
        sleep(1);

    }

}



void *consumer_thread_func(void *args)
{
    shared_event_queue* shared_events_queue = (shared_event_queue*)args;

    while(shared_events_queue->dm_tail == NULL) // we insert new events to the end of the quque - hence no way the queue is not empty if dm_tail == NULL.
    {
        pthread_cond_wait(&shared_events_queue->dm_cond_queue_not_empty,&shared_events_queue->dm_queue_mutex);

    }

    char* messageTask = shared_events_queue->dm_head->message;
	printf(messageTask);

    pthread_cleanup_push(&clean_up_callback_func,&shared_events_queue);
    pthread_cleanup_pop(1); // pops the clean_up_callback_func and execute it --> free the dynamic node alocation.
    free(messageTask); // free the dynamic string task allocation created by the consumer thread. 

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


void clean_up_callback_func(void* arg1) {  // define a cleanup call backfucn which will be called upon thread termination/cancelation.
    shared_event_queue* shared_events_queue = (shared_event_queue*)arg1; 
    event_node* event_node_to_delete = shared_events_queue->dm_head;
    shared_events_queue->dm_head = shared_events_queue->dm_head->next;
    free(event_node_to_delete);
}



int print(char *message) {
	printf("%s\n", message);
	return 0;
}


int main ()
{
    shared_event_queue* shared_event_queue =queue_init();

    init_thread_func(&shared_event_queue);
    // sleep()

    return 0;
}





