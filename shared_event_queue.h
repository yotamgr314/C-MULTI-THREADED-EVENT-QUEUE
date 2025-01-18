#include "event_node.h"
#include <pthread.h> // for pthread_mutex_t and pthread_cond_t.

typedef struct shared_event_queue
{
event_node* dm_head;
event_node* dm_tail;
int dm_count;
pthread_mutex_t dm_queue_mutex; // becuse the queue is gonna be shared through all threads. 
pthread_cond_t dm_cond_queue_not_empty;

}shared_event_queue;


shared_event_queue* queue_init(void);

void queue_enqueue_event_node(shared_event_queue* shared_event_queue, char* new_message);

void queue_dequeue_event_node(shared_event_queue* shared_event_queue, char* new_message);
