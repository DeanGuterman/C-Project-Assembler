#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

extern int error_free;

typedef struct symbol_table {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    int value;
    struct symbol_table* next;
} symbol_table;

/* Function to return the next symbol in the symbol table */
symbol_table* get_next_symbol(symbol_table* head) {
    return head->next;
}

/* Function to return the symbol */
char* get_symbol(symbol_table* head) {
    return head->symbol;
}

/* Function to check if a symbol already exists in the symbol table */
int symbol_exists(symbol_table* head, const char* symbol, int line_number) {
    while (head != NULL) {
        if (strcmp(head->symbol, symbol) == 0) {
            printf("Error in line %d: Symbol %s already exists in the symbol table\n", line_number, symbol);
            error_free = 0;
            return 1; /* Symbol exists in the table */
        }
        head = head->next;
    }
    return 0; /* Symbol does not exist in the table */
}

int check_symbol_legality(const char* symbol, int line_number){
    int i;
    int symbol_length;
    extern char* reserved_names[];

    symbol_length = strlen(symbol);

    /* Check if the symbol is too long */
    if (symbol_length > MAX_SYMBOL_LENGTH){
        printf("Error in line %d: Symbol %s is too long\n", line_number, symbol);
        error_free = 0;
        return 0;
    }

    /* Check if the symbol name is reserved */
    for ( i = 0; i < 28; i++) {
        if (strcmp(symbol, reserved_names[i]) == 0) {
            printf("Error in line %d illegal symbol name: %s is a reserved name\n", line_number, symbol);
            return 0;
        }
    }
    /* Check if the symbol contains only letters and numbers */
    for (i = 0; i < symbol_length; i++){
        if (!isalnum(symbol[i])){
            printf("Error in line %d: Symbol %s contains illegal characters\n", line_number, symbol);
            error_free = 0;
            return 0;
        }
    }
    return 1;
}


/* Function to add a new node to the symbol table */
symbol_table* insert_symbol(symbol_table* head, const char* symbol, int ic_value, int line_number) {
    symbol_table* new_symbol;
    symbol_table* temp;

    if (line_number != -1 && check_symbol_legality(symbol, line_number) == 0){
        return NULL;
    }

    /*If the symbol already exists, return NULL*/
    if (symbol_exists(head, symbol, line_number)){
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



