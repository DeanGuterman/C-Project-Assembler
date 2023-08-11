#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

extern int error_free;
extern const char* reserved_names[];


typedef struct symbol_table {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    int value; /* ic or dc value */
    int type; /* 1 for data, 2 for code */
    int is_extern_or_entry; /* 1 for external, 2 for entry*/
    int pre_defined_entry; /* 1 for pre-defined entry, 0 for not */
    struct symbol_table* next;
} symbol_table;

/* Function to return the next symbol in the symbol table */
symbol_table* get_next_symbol(symbol_table* node) {
    if(node == NULL) {
        return NULL;
    }
    return node->next;
}

/* Function to return the symbol */
char* get_symbol(symbol_table* node) {
    if(node == NULL) {
        return NULL;
    }
    return node->symbol;
}

/* Function to search a symbol in the symbol table */
symbol_table* search_symbol(symbol_table* head, const char* symbol) {
    while (head != NULL) {
        if (strcmp(head->symbol, symbol) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void set_symbol_external_or_entry(symbol_table* node, int extern_or_entry) {
    if(node == NULL) {
        return;
    }
    node->is_extern_or_entry = extern_or_entry;
}

int get_symbol_external_or_entry(symbol_table* node) {
    if(node == NULL) {
        return 0;
    }
    return node->is_extern_or_entry;
}

/* Function to set a symbol's type */
void set_symbol_type(symbol_table* node, int type) {
    if (node == NULL) {
        return;
    }
    node->type = type;
}

/* Function to set a symbol's is_extern_or_entry value */
void set_symbol_is_extern_or_entry(symbol_table* node, int is_extern_or_entry) {
    if (node == NULL) {
        return;
    }
    node->is_extern_or_entry = is_extern_or_entry;
}

/* Function to search and delete a symbol in the symbol table */
symbol_table* delete_symbol(symbol_table* head, const char* symbol) {
    symbol_table* temp;
    symbol_table* prev;
    temp = head;
    prev = NULL;

    /* If the head is the symbol to be deleted */
    if (strcmp(temp->symbol, symbol) == 0) {
        head = temp->next;
        free(temp);
        return head;
    }

    /* Search for the symbol to be deleted */
    while (temp != NULL && strcmp(temp->symbol, symbol) != 0) {
        prev = temp;
        temp = temp->next;
    }

    /* If the symbol was not found */
    if (temp == NULL) {
        return head;
    }

    /* If the symbol was found */
    prev->next = temp->next;
    free(temp);
    return head;
}

void set_symbol_pre_defined_entry(symbol_table* node, int pre_defined_entry) {
    if(node == NULL) {
        return;
    }
    node->pre_defined_entry = pre_defined_entry;
}

int get_symbol_pre_defined_entry(symbol_table* node) {
    if(node == NULL) {
        return 0;
    }
    return node->pre_defined_entry;
}


/* Function to check if a symbol already exists in the symbol table */
int symbol_exists(symbol_table* head, const char* symbol, int line_number) {
    while (head != NULL) {
        if (strcmp(head->symbol, symbol) == 0) {
            if (head->pre_defined_entry == 1){
                head->pre_defined_entry = 0;
            }
            else {
                printf("Error in line %d: Symbol %s already exists in the symbol table\n", line_number, symbol);
                error_free = 0;
                return 1; /* Symbol exists in the table */
            }
        }
        head = head->next;
    }
    return 0; /* Symbol does not exist in the table */
}

int check_symbol_legality(const char* symbol, int line_number){
    int i;
    int symbol_length;

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
        if (i == 0 && !isalpha(symbol[i])){
            printf("Error in line %d: Symbol %s must start with a letter\n", line_number, symbol);
            error_free = 0;
            return 0;
        }
        if (!isalnum(symbol[i])){
            printf("Error in line %d: Symbol %s contains illegal characters\n", line_number, symbol);
            error_free = 0;
            return 0;
        }
    }
    return 1;
}


/* Function to add a new node to the symbol table */
symbol_table* insert_symbol(symbol_table* head, const char* symbol, int value, int line_number) {
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
    new_symbol->value = value;
    new_symbol->type = -1;
    new_symbol->is_extern_or_entry = -1;
    new_symbol->pre_defined_entry = 0;
    new_symbol->next = NULL;

    if (head != NULL){
        temp = head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_symbol;
    }

    return new_symbol;
}



