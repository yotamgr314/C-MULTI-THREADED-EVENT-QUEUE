
// TO INSERT INSIDE THE PTHREAD_PRODUCER_FUNC

void queue_enqueue_event_node(shared_event_queue* shared_event_queue) 
{
    event_node* new_event_node = initlize_node(new_message);

    // mutex lock
    if(shared_event_queue->dm_tail == NULL) // if the queue is empty. 
    {
        shared_event_queue->dm_head = new_event_node;
        shared_event_queue->dm_tail = new_event_node;
        pthread_cond_signal(&shared_event_queue->dm_cond_queue_not_empty); // signal the waiting thread that the queue is no longer empty. 
        shared_event_queue->dm_tail = new_event_node;
    }else {
            shared_event_queue->dm_tail = new_event_node;
    }
    // mutex unlock
}



// TO INSERT INSIDE THE PTHREAD_CONSUMER_FUNC
void queue_dequeue_event_node(shared_event_queue* shared_event_queue, char* new_message)
{
    // mutex lock
    while(shared_event_queue->dm_head == NULL)
    {
        pthread_cond_wait(&shared_event_queue->dm_cond_queue_not_empty, &shared_event_queue->dm_queue_mutex);
    }
    
    event_node* new_event_node = shared_event_queue->dm_head; 
    // consume task 
    // remove string allocated by consumer thread.
    // remove dynmic allocated node. 

}
