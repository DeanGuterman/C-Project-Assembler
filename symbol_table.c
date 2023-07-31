#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef struct symbol_table {
    char symbol[MAX];
    int value;
    struct symbol_table* next;
} symbol_table;

// Function to add a new node to the symbol table
void add(symbol_table** head, const char* symbol, int value) {
    symbol_table* new_node = (symbol_table*)malloc(sizeof(symbol_table));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    strncpy(new_node->symbol, symbol, sizeof(new_node->symbol) - 1);
    new_node->symbol[sizeof(new_node->symbol) - 1] = '\0'; // Ensure null-termination
    new_node->value = value;
    new_node->next = NULL;

    if (*head == NULL) {
        // If the list is empty, set the new node as the head
        *head = new_node;
    } else {
        // Find the last node and append the new node to the end
        symbol_table* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Function to check if a symbol exists in the symbol table
int checkIfExists(symbol_table* head, const char* symbol) {
    while (head != NULL) {
        if (strcmp(head->symbol, symbol) == 0) {
            return 1; // Symbol exists in the table
        }
        head = head->next;
    }
    return 0; // Symbol does not exist in the table
}
