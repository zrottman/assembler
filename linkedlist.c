#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

Node* create_node(char* key, int val)
{
    Node* node  = malloc(sizeof(Node));

    node->key   = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->val   = val;
    node->next  = NULL;

    return node;
}

LinkedList* create_linked_list(void)
{
    LinkedList* linkedlist = malloc(sizeof(LinkedList));
    linkedlist->head = NULL;
    linkedlist->tail = NULL;
    linkedlist->len  = 0;

    return linkedlist;
}

void print_linked_list(LinkedList* linkedlist)
{
    for (Node* cur = linkedlist->head; cur != NULL; cur = cur->next)
        printf("%s/%d -> ", cur->key, cur->val);
    printf("\n");
}

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

int search(LinkedList* linkedlist, char* target_key, int* default_val)
{
    for (Node* cur = linkedlist->head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->key, target_key) == 0)
            return cur->val;
    }
    
    append(linkedlist, create_node(target_key, *default_val));

    return (*default_val)++;
}

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
