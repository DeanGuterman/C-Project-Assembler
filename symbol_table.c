#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef struct symbol_table {
    char symbol[MAX];
    int value;
    struct symbol_table* prev;
} symbol_table;

/* Function to add a new node to the symbol table */
symbol_table* add(symbol_table** tail, const char* symbol) {
    symbol_table* new_node = (symbol_table*)malloc(sizeof(symbol_table));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strncpy(new_node->symbol, symbol, sizeof(new_node->symbol) - 1);
    new_node->symbol[sizeof(new_node->symbol) - 1] = '\0'; /* Ensure null-termination */
    new_node->value = 0;
    new_node->prev = NULL;

    if (*tail == NULL) {
        /* If the list is empty, set the new node as the tail */
        *tail = new_node;
    } else {
        /* Append the new node to the end and update the previous pointer */
        (*tail)->prev = new_node;
    }

    /* Update the tail to point to the new node */
    *tail = new_node;

    return new_node;
}

/* Function to check if a symbol exists in the symbol table */
int check_if_exists(symbol_table* tail, const char* symbol) {
    while (tail != NULL) {
        if (strcmp(tail->symbol, symbol) == 0) {
            return 1; /* Symbol exists in the table */
        }
        tail = tail->prev;
    }
    return 0; /* Symbol does not exist in the table */
}
