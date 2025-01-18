#include "shared_event_queue.h"
#include <stdlib.h>
#include <stdio.h>

shared_event_queue* queue_init(void)
{
    shared_event_queue* new_event_queue = malloc(sizeof(shared_event_queue));
    if (new_event_queue == NULL) {
        printf("malloc failed\n");
        exit(1);
    }


    new_event_queue->dm_head = NULL;
    new_event_queue->dm_tail = NULL;
    new_event_queue->dm_count = 0;


    if (pthread_cond_init(&new_event_queue->dm_cond_queue_not_empty, NULL) != 0 ||
    pthread_mutex_init(&new_event_queue->dm_queue_mutex, NULL) != 0) {
    printf("pthread_cond_init or pthread_mutex_init failed\n");
    free(new_event_queue);
    exit(1);
}

    return new_event_queue;
}




