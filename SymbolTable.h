#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

typedef struct Node {
    char*        key;
    int          val;
    struct Node* next;
} Node;

typedef struct SymbolTable {
    Node*  head;
    Node*  tail;
    size_t len;
    size_t default_val;
} SymbolTable;

Node*       create_node(char* key, int val);
SymbolTable* create_linked_list(void);
SymbolTable* create_symbols_table(void);
void        print_linked_list(SymbolTable* linkedlist);
int         append(SymbolTable* linkedlist, Node* new_node);
int         search(SymbolTable* linkedlist, char* target_key, int default_val);
int         delete_node(SymbolTable* linkedlist, char* target_key);
void        initialize_symbols(SymbolTable* symbols);

#endif // SYMBOLTABLE_H
