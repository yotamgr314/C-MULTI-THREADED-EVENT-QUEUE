#include <event_queue.h>

shared_event_queue* queue_init(void)
{
    shared_event_queue* new_event_queue = malloc(sizeof(shared_event_queue));
    if(new_event_queue == NULL)
    {
        printf("meme_fail");
        exist(0);
    }

    new_event_queue->head = NULL;
    new_event_queue->tail = NULL;
    // pthread_cond_init(pthread_cond_t * cond, const pthread_condattr_t * attr); sending NULL to *attr means default cond intizliztion.
    pthread_cond_init(&new_event_queue->empty_cond, NULL);
    // pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutex_t *attr); sending NULL to *attr means default mutex initzlizion.
    pthread_mutex_init(&new_event_queue->queue_mutex,NULL);

    return new_event_queue;
}



void queue_add_event_node(shared_event_queue* shared_event_queue, char* new_message)
{
    event_node* new_event_node = initlize_node(new_message);

    if(shared_event_queue->tail == NULL) // if the queue is empty. 
    {
        shared_event_queue->head = new_event_node;
        shared_event_queue->tail = new_event_node;
    }

    shared_event_queue->tail = new_event_node;

    //pthread_cond_signal(signal the consumer thread that an event as been added).
}


