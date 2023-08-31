#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"

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

// Function:    create_symbol_table
// Description: Creates empty SymbolTable struct (a wrapper struct that maintains
//              pointers to head and tail as well as a length count).
// Parameters:  void
// Returns:     SymbolTable* pointer to newly created linked list. 
SymbolTable* create_symbol_table(void)
{
    SymbolTable* symbols = malloc(sizeof(SymbolTable));
    symbols->head        = NULL;
    symbols->tail        = NULL;
    symbols->len         = 0;
    symbols->default_val = 16;
    init_symbol_table(symbols);
    return symbols;
}

// Function:    print_symbol_table
// Description: Prints linked list to stdout
// Parameters:
//              symbols: pointer to linked list to print
// Returns:     void
void print_symbol_table(SymbolTable* symbols)
{
    for (Node* cur = symbols->head; cur != NULL; cur = cur->next)
        printf("%s/%d -> ", cur->key, cur->val);
    printf("\n");
}

// Function:    append
// Description: Appends new node to tail of linked list and points `tail` member
//              to it. If linked list has no head, function points `head` and 
//              `tail` to this node.
// Parameters:
//              symbols: pointer to linked list
//              new_node:   pointer to node to append to linked list
// Returns:     int new length of linked list.
int append(SymbolTable* symbols, Node* new_node)
{
    if (symbols->head == NULL) {
        symbols->head = symbols->tail = new_node;
    } else {
        symbols->tail->next = new_node;
        symbols->tail = symbols->tail->next;
    }
    ++symbols->len;
    return new_node->val;;
}

// Function:    search
// Description: Searches for target key (string) in linked list and returns
//              matching value if found, else appends new node to linked list
//              and returns default value assigned to it.
// Parameters:
//              symbols:     linked list to search
//              target_key:     pointer to search term
//              default_val:    integer value to assign in event that target_key
//                              is not found and new node is appeneded.
// Returns:     int value matching target_key
int search(SymbolTable* symbols, char* target_key)
{
    // if the target key exists, return matching value
    for (Node* cur = symbols->head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->key, target_key) == 0) return cur->val;
    }
    
    // else append new node with default value
    return append(symbols, create_node(target_key, symbols->default_val++));
}

// Function:    delete_node
// Description: Deleted node from linked list where node->key matches target_key.
// Parameters:  
//              symbols: pointer to linked list to search
//              target_key: pointer to string to match
// Returns:     0 on success; -1 if node not found
// Note:        No current implementation for assemblerg. Does not currently 
//              support head deletions.
int delete_node(SymbolTable* symbols, char* target_key)
{
    Node* tmp;
    for (Node* cur = symbols->head; cur->next != NULL; cur = cur->next) {
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

// Function:    init_symbol_table
// Description: Initializes `symbols` linked list with default symbols key/value
//              pairs.
// Parameters:  
//              symbols:    pointer to linked list
// Returns:     void
// TODO:        consider making struct that contains key and num as members,
//              then make one array instead of two to loop through
void init_symbol_table(SymbolTable* symbols)
{
    // default key/value pairs for initializing symbols symbols
    char* keys[]    = { 
        "SP", "LCL", "ARG", "THIS", "THAT", "R0", "R1", "R2", "R3", "R4", 
        "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", 
        "R15", "SCREEN", "KBD"
    };
    int nums[]      = {
        0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 16384, 24576
    };
    int len         = sizeof(nums) / sizeof(nums[0]);

    // populate linked list with default symbols
    for (int i=0; i<len; ++i)
        append(symbols, create_node(keys[i], nums[i]));
}
