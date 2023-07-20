#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

// Function:    create_node
// Description: Creates a new Node struct with given key and value
// Parameters:
//              key:    pointer to string that will be copied
//              val:    integer
// Returns:     Node* pointer to newly created node.
Node* create_node(char* key, int val)
{
    Node* node  = malloc(sizeof(Node));

    node->key   = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->val   = val;
    node->next  = NULL;

    return node;
}

// Function:    create_linked_list
// Description: Creates empty LinkedList struct (a wrapper struct that maintains
//              pointers to head and tail as well as a length count).
// Parameters:  void
// Returns:     LinkedList* pointer to newly created linked list. 
LinkedList* create_linked_list(void)
{
    LinkedList* linkedlist = malloc(sizeof(LinkedList));
    linkedlist->head = NULL;
    linkedlist->tail = NULL;
    linkedlist->len  = 0;

    return linkedlist;
}

// Function:    print_linked_list
// Description: Prints linked list to stdout
// Parameters:
//              linkedlist: pointer to linked list to print
// Returns:     void
void print_linked_list(LinkedList* linkedlist)
{
    for (Node* cur = linkedlist->head; cur != NULL; cur = cur->next)
        printf("%s/%d -> ", cur->key, cur->val);
    printf("\n");
}

// Function:    append
// Description: Appends new node to tail of linked list and points `tail` member
//              to it. If linked list has no head, function points `head` and 
//              `tail` to this node.
// Parameters:
//              linkedlist: pointer to linked list
//              new_node:   pointer to node to append to linked list
// Returns:     int new length of linked list.
int append(LinkedList* linkedlist, Node* new_node)
{
    if (linkedlist->head == NULL) {
        linkedlist->head = linkedlist->tail = new_node;
    } else {
        linkedlist->tail->next = new_node;
        linkedlist->tail = linkedlist->tail->next;
    }
    return ++linkedlist->len;
}

// Function:    search
// Description: Searches for target key (string) in linked list and returns
//              matching value if found, else appends new node to linked list
//              and returns default value assigned to it.
// Parameters:
//              linkedlist:     linked list to search
//              target_key:     pointer to search term
//              default_val:    integer value to assign in event that target_key
//                              is not found and new node is appeneded.
// Returns:     int value matching target_key
int search(LinkedList* linkedlist, char* target_key, int default_val)
{
    for (Node* cur = linkedlist->head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->key, target_key) == 0) return cur->val;
    }
    
    append(linkedlist, create_node(target_key, default_val));

    return default_val;
}

// Function:    delete_node
// Description: Deleted node from linked list where node->key matches target_key.
// Parameters:  
//              linkedlist: pointer to linked list to search
//              target_key: pointer to string to match
// Returns:     0 on success; -1 if node not found
// Note:        Does not currently support head deletions. TODO.
int delete_node(LinkedList* linkedlist, char* target_key)
{
    Node* tmp;
    for (Node* cur = linkedlist->head; cur->next != NULL; cur = cur->next) {
        if (strcmp(cur->next->key, target_key) == 0) {
            tmp = cur->next->next;
            free(cur->next->key);
            free(cur->next);
            cur->next = tmp;
            return 0;           // return 0 if found
        }
    }

    return -1;                  // return -1 if not found
}
