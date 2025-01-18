typedef struct event_node {
char* message;
struct event_node* next;

}event_node;


event_node *initlize_node(char* message);
