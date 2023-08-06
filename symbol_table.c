#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 80
#define MAX_SYMBOL_LENGTH 30

typedef struct symbol_table {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    int value;
    struct symbol_table* next;
} symbol_table;

/* Function to check if a symbol already exists in the symbol table */
int symbol_exists(symbol_table* head, const char* symbol) {
    while (head != NULL) {
        if (strcmp(head->symbol, symbol) == 0) {
            return 1; /* Symbol exists in the table */
        }
        head = head->next;
    }
    return 0; /* Symbol does not exist in the table */
}


/* Function to add a new node to the symbol table */
symbol_table* insert_symbol(symbol_table* head, const char* symbol, int ic_value) {
    symbol_table* new_symbol;
    symbol_table* temp;

    /*If the symbol already exists, return NULL*/
    if (symbol_exists(head, symbol)){
        printf("Symbol already exists!\n");
        return NULL;
    }

    new_symbol = (symbol_table*)malloc(sizeof(symbol_table));
    /* Check if memory allocation failed */
    if (new_symbol == NULL) {
        printf("Memory allocation failed!\n");
        free (new_symbol);
        return NULL;
    }

    /* Copy the symbol name into the allocated memory */
    strncpy(new_symbol->symbol, symbol, sizeof(new_symbol->symbol) - 1);
    new_symbol->symbol[sizeof(new_symbol->symbol) - 1] = '\0'; /* Ensure null-termination */
    new_symbol->value = ic_value;
    new_symbol->next = NULL;

    if (head != NULL){
        temp = head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_symbol;
    }

    printf("Symbol %s added to the symbol table\n", new_symbol->symbol);
    return new_symbol;
}



