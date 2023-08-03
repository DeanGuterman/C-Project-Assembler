#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbol_table.h"
#define MAX 100

char* extract_symbol(const char line[]) {
    int index;
    int symbol_index;
    char* symbol_name;
    int i;
    index = 0;
    symbol_index = 0;

    /* Skip leading whitespace */
    while (isspace(line[index]))
        index++;

    /* Calculate the length of the symbol name */
    while (!isspace(line[index]) && line[index] != '\n' && line[index] != ':') {
        symbol_index++;
        index++;
    }

    /* Allocate memory for the symbol name */
    symbol_name = (char*)malloc((symbol_index + 1) * sizeof(char));
    if (symbol_name == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    /* Copy the symbol name into the allocated memory */
    index -= symbol_index;
    for (i = 0; i < symbol_index; i++) {
        symbol_name[i] = line[index];
        index++;
    }
    symbol_name[symbol_index] = '\0'; /* Null-terminate the symbol name */

    /* Check if it's a valid symbol declaration */
    if (line[index] != ':') {
        free(symbol_name); /* Free the memory before returning NULL */
        return NULL; /* No symbol declaration found */
    }

    return symbol_name;
}

int first_pass_through(char* argv) {
    char file_name[MAX];
    char line[MAX];
    FILE* input_file;
    int temp_dc, temp_ic;
    symbol_table* tail;
    char* symbol_name;
    symbol_table* new_node;
    temp_dc = 0;
    temp_ic = 100;
    tail = NULL; /* Initialize head to NULL */

    /* Opening the file */
    strcpy(file_name, argv);
    strcat(file_name, ".am");
    printf("Going through file: %s\n", file_name);
    input_file = fopen(file_name, "r");

    /* Go through every line in the file */
    while (fgets(line, MAX, input_file)) {
        /* Check if it's a symbol declaration */
        symbol_name = extract_symbol(line);
        if (symbol_name != NULL) {
            if (tail == NULL) {
                new_node = (symbol_table*)malloc(sizeof(symbol_table));
                if (new_node == NULL) {
                    printf("Memory allocation failed!\n");
                }
                strncpy(new_node->symbol, symbol_name, sizeof(new_node->symbol) - 1);
                new_node->symbol[sizeof(new_node->symbol) - 1] = '\0'; /* Ensure null-termination */
                new_node->value = 0;
                new_node->prev = NULL;
                tail = new_node;
            } else if (check_if_exists(tail, symbol_name) == 0) {
                symbol_table* new_symbol;
                printf("Symbol: %s\n", symbol_name);
                new_symbol = add(tail, symbol_name);
                new_symbol->value = temp_ic;
            } else {
                printf("error: Symbol \"%s\" already exists in the table\n", symbol_name);
            }
            free (symbol_name);
        }
    }

    fclose(input_file);
    /* Free the memory for the symbol table nodes when done using the linked list */
    while (tail != NULL) {
        symbol_table* temp;
        temp = tail;
        tail = tail->prev;
        free(temp);
    }

    return 0;
}