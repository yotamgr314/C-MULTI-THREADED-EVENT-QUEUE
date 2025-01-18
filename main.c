#include <stdio.h>
#include "shared_event_queue.h"
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_CYCLES 10 
#define MESSAGES_PER_THREAD 3
#define threadsKeepRunning 1

// Define a print function to print the message (simulates the printer task).
int print(char *message) {
    printf("%s\n", message);
    return 0;
}

// Define a cleanup callback function, which will be called upon thread termination/cancelation.
void clean_up_callback_func(void *arg) {
    event_node *event_node_to_delete = (event_node *)arg;
    if (event_node_to_delete != NULL) {
        free(event_node_to_delete->message); // Freeing the producer dynamically allocated string.
        free(event_node_to_delete);
    }
}

// Producer thread function
void* producer_thread_func(void *args) {
    shared_event_queue* shared_events_queue = (shared_event_queue*)args;

    for (int k = 0; k < NUM_CYCLES; k++) {
        for (int m = 0; m < MESSAGES_PER_THREAD; m++) {
            pthread_mutex_lock(&shared_events_queue->dm_queue_mutex);
/*             asprintf(&newMessage, "hey, I am thread num %ld and this message number %d", (long) pthread_self(), shared_events_queue->dm_count); // The `asprintf` function handles both the memory allocation and string formatting in a single step. it will allocate enough space in message for "hey, i am thread num <thread_id>, and this is message number <current_amount_of_messages_in_the_q>". (replaces malloc, strcpy, strcat).
 */
            char* newMessage;
            if (asprintf(&newMessage, "hey, I am thread num %ld and this message number %d",
                         (long)pthread_self(), shared_events_queue->dm_count) == -1) {
                printf("asprintf failed\n");
                pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);
                continue;
            }

            event_node* new_event_node = initlize_node(newMessage);

            if (shared_events_queue->dm_tail == NULL) // we insert new events to the end of the quque - hence no way the queue is not empty if dm_tail == NULL. (in case we sent a request to the server and he is bussy ATM, we just wait untill he is available to process our request)
            {
                shared_events_queue->dm_tail = new_event_node;
                shared_events_queue->dm_head = new_event_node;
            } else {
                shared_events_queue->dm_tail->next = new_event_node;
                shared_events_queue->dm_tail = new_event_node;
            }

            shared_events_queue->dm_count++;
            pthread_cond_signal(&shared_events_queue->dm_cond_queue_not_empty);  // signal the condwaiting thread that the queue is no longer empty. 

            pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);
        }

        sleep(1); // Sleep after producing all messages for this cycle.
    }
    return NULL;
}

// Consumer thread function
void* consumer_thread_func(void *args)
{
    shared_event_queue* shared_events_queue = (shared_event_queue*)args;

    while (threadsKeepRunning) {
        pthread_mutex_lock(&shared_events_queue->dm_queue_mutex);

        while (shared_events_queue->dm_head == NULL && threadsKeepRunning) { 
            pthread_cond_wait(&shared_events_queue->dm_cond_queue_not_empty, &shared_events_queue->dm_queue_mutex);
        }

        if (!threadsKeepRunning) {
            pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);
            break;
        }

        event_node* node = shared_events_queue->dm_head;
        shared_events_queue->dm_head = shared_events_queue->dm_head->next;

        if (shared_events_queue->dm_head == NULL) {
            shared_events_queue->dm_tail = NULL;
        }

        pthread_mutex_unlock(&shared_events_queue->dm_queue_mutex);

        pthread_cleanup_push(clean_up_callback_func, node);
        print(node->message);
        pthread_cleanup_pop(1);
    }

    return NULL;
}

// Initialize threads (producers and consumers).
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

// Main function
int main() {
    shared_event_queue* shared_event_queue = queue_init();

    init_thread_func(shared_event_queue);

    return 0;
}
