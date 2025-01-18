#include <stdio.h>
#include "shared_event_queue.h"
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_CYCLES 10 
#define threadsKeepRunning 1



void clean_up_callback_func(void *arg) { // define a cleanup call backfucn which will be called upon thread termination/cancelation.
    event_node *event_node_to_delete = (event_node *)arg;
    if (event_node_to_delete != NULL) {
        free(event_node_to_delete->message); // freeing the producer dynamic allocated strinv.
        free(event_node_to_delete);         
    }
}

void* producer_thread_func(void *args)
{
shared_event_queue* shared_events_queue = (shared_event_queue*) args;

    for(int k=0; k < NUM_CYCLES; k++)
    {
        for(int j = 0; j<NUM_THREADS; j ++)
        {
            pthread_mutex_lock(&shared_events_queue->dm_queue_mutex);

            char* newMessage; 
/*             asprintf(&newMessage, "hey, I am thread num %ld and this message number %d", (long) pthread_self(), shared_events_queue->dm_count); // The `asprintf` function handles both the memory allocation and string formatting in a single step. it will allocate enough space in message for "hey, i am thread num <thread_id>, and this is message number <current_amount_of_messages_in_the_q>". (replaces malloc, strcpy, strcat).
 */
            if (asprintf(&newMessage, "hey, I am thread num %ld and this message number %d", (long)pthread_self(), shared_events_queue->dm_count) == -1) {
                printf("asprintf failed\n");
                pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);
                continue;
            }

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




void *consumer_thread_func(void *args) {

    shared_event_queue *shared_events_queue = (shared_event_queue *)args;

    pthread_mutex_lock(&shared_events_queue->dm_queue_mutex);

    while (threadsKeepRunning) 
    { 

        while (shared_events_queue->dm_head == NULL) // we insert new events to the end of the quque - hence no way the queue is not empty if dm_tail == NULL. (in case we sent a request to the server and he is bussy ATM, we just wait untill he is available to process our request)
        {   
             pthread_cond_wait(&shared_events_queue->dm_cond_queue_not_empty, &shared_events_queue->dm_queue_mutex);
        }

        if (!threadsKeepRunning)
        {
        pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);
        break;
        }

        event_node *node = shared_events_queue->dm_head; // remove a node task from the head of the list.

        shared_events_queue->dm_head = shared_events_queue->dm_head->next;  // sets a new head. 

        if (shared_events_queue->dm_head == NULL)
        {
            shared_events_queue->dm_tail = NULL;
        }
        shared_events_queue->dm_count--;

        pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);

        pthread_cleanup_push(clean_up_callback_func, node); // register the callback function.
        print(node->message); // execute the task (aka send the message to the printer...)
        pthread_cleanup_pop(1);  // pops the clean_up_callback_func and execute it --> free the dynamic node alocation.

    }

    return NULL;
}









void init_thread_func(shared_event_queue* shared_events_queue) {
    pthread_t producer_threads[NUM_THREADS];
    pthread_t consumer_threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&producer_threads[i], NULL, producer_thread_func, shared_events_queue);
        pthread_create(&consumer_threads[i], NULL, consumer_thread_func, shared_events_queue);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(producer_threads[i], NULL);
        pthread_join(consumer_threads[i], NULL);
    }
}





int print(char *message) {
	printf("%s\n", message);
	return 0;
}


int main ()
{
    shared_event_queue* shared_event_queue =queue_init();

    init_thread_func(shared_event_queue);
    // sleep()

    return 0;
}





