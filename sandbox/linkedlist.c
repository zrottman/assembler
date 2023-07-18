#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


typedef struct Node {
    char*        symbol;
    int          value;
    struct Node* next;
} Node;

Node* create_node(char* symbol, int val)
{
    Node* node = malloc(sizeof(Node));

    node->symbol = symbol;
    //node->symbol = malloc(strlen(symbol) + 1);
    //strcpy(node->symbol, symbol);
    node->value = val;
    node->next  = NULL;

    return node;
}

void print_linked_list(Node* head)
{
    while (head != NULL) {
        printf("%s[%d] -> ", head->symbol, head->value);
        head = head->next;
    }
    printf("\n");
}

void insert_node(Node* head, Node* new_node)
{
    while (head->next != NULL)
        head = head->next;

    head->next = new_node;
}

void delete_node(Node* head, int target)
{
    Node* tmp;
    while (head->next != NULL) {
        if (head->next->value == target) {
            tmp = head->next->next;
            //free(head->next->symbol);
            free(head->next);
            head->next = tmp; 
            return;
        }
        head = head->next;
    }
}

int search(Node* head, char* target, int* default_val)
{
    for (Node* cur = head; cur != NULL; cur = cur->next) {
        //if (strcmp(cur->symbol, target) == 0)
        if(cur->symbol == target) {
            if (strcmp(cur->symbol, target) == 0)
                printf("Strcmp == 0\n");
            printf("target '%s' (@%p) == cur->symbol '%s' (@%p)\n", target, target, cur->symbol, cur->symbol);
            return cur->value;
        }
    }
    
    insert_node(head, create_node(target, (*default_val)++));

    return *default_val;
}

int main()
{
    char* symbols[] = { 
        "SP", "LCL", "ARG", "THIS", "THAT", "R0", "R1", "R2", "R3", "R4", 
        "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", 
        "R15", "SCREEN", "KBD"
    };

    int nums[]     = {
        0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 16384, 24576
    };
    
    int len         = sizeof(nums) / sizeof(nums[0]);
    int default_val = 16;

    int search_res;
    char* target = "THIS";

    Node* head = create_node(symbols[0], nums[0]);

    for (int i = 1; i < len; ++i) {
        insert_node(head, create_node(symbols[i], nums[i]));
    }

    printf("Displaying linked list: ");
    print_linked_list(head);

    delete_node(head, 3);

    print_linked_list(head);

    search_res = search(head, "THIS", &default_val);
    printf("Searching for 'THIS': %d\n", search_res);

    search_res = search(head, target, &default_val);
    printf("Searching for 'THIS': %d\n", search_res);

    search_res = search(head, "my_var", &default_val);
    printf("Searching for 'my_var': %d\n", search_res);

    search_res = search(head, "i", &default_val);
    printf("Searching for 'i': %d\n", search_res);

    print_linked_list(head);

    return 0;
}

