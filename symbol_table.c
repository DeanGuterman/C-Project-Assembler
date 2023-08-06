#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef struct symbol_table {
    char symbol[MAX];
    int value;
    struct symbol_table* next;
} symbol_table;

symbol_table* insert_first_symbol(const char* symbol){
    symbol_table* new_node;
    new_node = (symbol_table*)malloc(sizeof(symbol_table));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        free (new_node);
        return NULL;
    }
    strncpy(new_node->symbol, symbol, sizeof(new_node->symbol) - 1);
    new_node->symbol[sizeof(new_node->symbol) - 1] = '\0';  /*Ensure null-termination*/
    new_node->value = 0;
    new_node->next = NULL;
    return new_node;
}

/* Function to add a new node to the symbol table */
symbol_table* insert_symbol(symbol_table* head, const char* symbol) {
    symbol_table* new_node;
    symbol_table* temp;
    new_node = (symbol_table*)malloc(sizeof(symbol_table));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        free (new_node);
        return NULL;
    }
    strncpy(new_node->symbol, symbol, sizeof(new_node->symbol) - 1);
    new_node->symbol[sizeof(new_node->symbol) - 1] = '\0'; /* Ensure null-termination */
    new_node->value = 0;
    new_node->next = NULL;

    if (head != NULL){
        temp = head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_node;
    }

    return new_node;
}

/* Function to add a new node to the symbol table *//*
symbol_table* add(symbol_table* tail, const char* symbol) {
    symbol_table* new_node;
    symbol_table* temp = tail;
    new_node = (symbol_table*)malloc(sizeof(symbol_table));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strncpy(new_node->symbol, symbol, sizeof(new_node->symbol) - 1);
    new_node->symbol[sizeof(new_node->symbol) - 1] = '\0';
    new_node->value = 0;
    new_node->prev = NULL;

    while (temp->prev != NULL){
        temp = temp->prev;
    }
    temp->prev = new_node;
    return new_node;
} */

/* Function to check if a symbol exists in the symbol table */
int symbol_exists(symbol_table* head, const char* symbol) {
    while (head != NULL) {
        if (strcmp(head->symbol, symbol) == 0) {
            return 1; /* Symbol exists in the table */
        }
        head = head->next;
    }
    return 0; /* Symbol does not exist in the table */
}

