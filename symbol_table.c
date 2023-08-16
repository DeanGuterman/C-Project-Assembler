#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "symbol_table.h"


struct symbol_table {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    int value; /* ic or dc value */
    int type; /*0 for instruction, 1 for data */
    int is_extern_or_entry; /* 1 for external, 2 for entry*/
    int pre_defined_entry; /* 1 for pre-defined entry, 0 for not */
    struct symbol_table* next;
};

/* Function to return the next symbol in the symbol table */
symbol_table* get_next_symbol(symbol_table* node) {
    if(node == NULL) {
        return NULL;
    }
    return node->next;
}

/* Function to return a symbol */
char* get_symbol(symbol_table* node) {
    if(node == NULL) {
        return NULL;
    }
    return node->symbol;
}

/* Function to get a symbol's value */
int get_symbol_value(symbol_table* node) {
    if(node == NULL) {
        return 0;
    }
    return node->value;
}

/* Function to set a symbol's value */
int set_symbol_value(symbol_table* node, int value) {
    if(node == NULL) {
        return 0;
    }
    node->value = value;
    return 1;
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

/* Function to set a symbol's external or entry value */
void set_symbol_external_or_entry(symbol_table* node, int extern_or_entry) {
    if(node == NULL) {
        return;
    }
    node->is_extern_or_entry = extern_or_entry;
}

/* Function to get a symbol's external or entry value */
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

/* Function to get a symbol's type */
int get_symbol_type(symbol_table* node) {
    if (node == NULL) {
        return 0;
    }
    return node->type;
}

/* Function to set a symbol's pre-defined entry value */
void set_symbol_pre_defined_entry(symbol_table* node, int pre_defined_entry) {
    if(node == NULL) {
        return;
    }
    node->pre_defined_entry = pre_defined_entry;
}

/* Function to get a symbol's pre-defined entry value */
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
                return 2; /* Symbol exists in the table and is a pre-defined entry */
            }
            else {
                printf("Error: line %d symbol %s already exists in the symbol table\n", line_number, symbol);
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
    size_t symbol_length;

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
    int already_exists;

    already_exists = symbol_exists(head, symbol, line_number);

    if (line_number != -1 && check_symbol_legality(symbol, line_number) == 0){
        return NULL;
    }

    /*If the symbol already exists, return NULL*/
    if (already_exists == 1){
        return NULL;
    }

    else if (already_exists == 2){
        new_symbol = search_symbol(head, symbol);
        set_symbol_value(new_symbol, value);
        set_symbol_external_or_entry(new_symbol, 2);
        return new_symbol;
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
    new_symbol->type = 0;
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



