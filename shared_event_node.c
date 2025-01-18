#include "node.h"
#include <stdlib.h>

event_node *initlize_node(char* message)
{
    event_node* new_event_node = malloac(sizeof(event_node));
    if(new_event_node == NULL)
    {
        printf("meme_fail");
        exit(0);
    }

    new_event_node->message = NULL;
    new_event_node->next = NULL;

    return new_event_node;
}