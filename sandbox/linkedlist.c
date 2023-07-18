#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main()
{

    // default key/value pairs for initializing symbols linkedlist
    char* keys[]    = { 
        "SP", "LCL", "ARG", "THIS", "THAT", "R0", "R1", "R2", "R3", "R4", 
        "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", 
        "R15", "SCREEN", "KBD"
    };
    int nums[]      = {
        0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 16384, 24576
    };
    int default_val = 16;
    int len         = sizeof(nums) / sizeof(nums[0]);

    // create symbols linked list
    LinkedList* symbols = create_linked_list();

    // populate linked list with default symbols
    for (int i=0; i<len; ++i)
        append(symbols, create_node(keys[i], nums[i]));

    // tests
    int search_res;
    char* target = "THIS";

    printf("symbols length: %d\n", symbols->len);
    print_linked_list(symbols);

    default_val = 16;

    target = "THIS";
    search_res = search(symbols, target, &default_val);
    printf("%s -> %d\n", target, search_res);

    target = "my_var";
    search_res = search(symbols, target, &default_val);
    printf("%s -> %d\n", target, search_res);

    target = "R9";
    search_res = search(symbols, target, &default_val);
    printf("%s -> %d\n", target, search_res);

    target = "your_var";
    search_res = search(symbols, target, &default_val);
    printf("%s -> %d\n", target, search_res);

    print_linked_list(symbols);

    target = "R8";
    search_res = delete_node(symbols, target);
    printf("Deleted '%s'? %d\n", target, search_res);

    target = "new_Var";
    search_res = delete_node(symbols, target);
    printf("Deleted '%s'? %d\n", target, search_res);

    print_linked_list(symbols);

    return 0;
}

