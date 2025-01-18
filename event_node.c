#include "event_node.h"
#include <stdlib.h>
#include <stdio.h>

event_node* initlize_node(char* message) {
    event_node* new_event_node = malloc(sizeof(event_node));
    if (new_event_node == NULL) {
        printf("malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_event_node->message = message;
    new_event_node->next = NULL;
    return new_event_node;
}
