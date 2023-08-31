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
    size_t len; // no current implementation
    size_t default_val;
} SymbolTable;

Node*        create_node(char* key, int val);
SymbolTable* create_symbol_table(void);
void         init_symbol_table(SymbolTable* symbols);
void         print_symbol_table(SymbolTable* symbols);
int          append(SymbolTable* symbols, Node* new_node);
int          search(SymbolTable* symbols, char* target_key);
int          delete_node(SymbolTable* symbols, char* target_key);

#endif // SYMBOLTABLE_H
