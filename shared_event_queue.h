#include "event_node.h"
#include <pthread.h> // for pthread_mutex_t and pthread_cond_t.

typedef struct shared_event_queue{
event_node* head;
event_node* tail;
pthread_mutex_t queue_mutex; // becuse the queue is gonna be shared through all threads. 
pthread_cond_t empty_cond;
}shared_event_queue;


shared_event_queue* queue_init(void);

void queue_add_event_node(shared_event_queue* shared_event_queue, char* new_message);
