#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node {
    char*        key;
    int          val;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    int   len;
} LinkedList;

Node*       create_node(char* key, int val);
LinkedList* create_linked_list(void);
void        print_linked_list(LinkedList* linkedlist);
int         append(LinkedList* linkedlist, Node* new_node);
int         search(LinkedList* linkedlist, char* target_key, int default_val);
int         delete_node(LinkedList* linkedlist, char* target_key);

#endif // LINKEDLIST_H
