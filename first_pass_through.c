#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbol_table.h"
#define MAX 100

char* extract_symbol(const char line[]) {
    int index = 0;
    int symbol_index = 0;

    char* symbol_name = (char*)malloc(MAX * sizeof(char));
    if (symbol_name == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    /* Skip leading whitespace */
    while (isspace(line[index]))
        index++;

    /* Extract the symbol name */
    while (!isspace(line[index]) && line[index] != '\n' && line[index] != ':') {
        symbol_name[symbol_index++] = line[index++];
        if (symbol_index >= MAX - 1) {
            printf("error: Symbol name is too long\n");
            free(symbol_name); /* Free the memory before returning NULL */
            return NULL;
        }
    }
    if (line[index] != ':') {
        free(symbol_name); /* Free the memory before returning NULL */
        return NULL; /* No symbol declaration found */
    }
    else {
        symbol_name[symbol_index] = '\0'; /* Null-terminate the symbol name */
        return symbol_name;
    }
}

int first_pass_through(char* argv) {
    char file_name[MAX];
    char line[MAX];
    FILE* input_file;
    int temp_dc = 0, temp_ic = 100;
    symbol_table* tail = NULL; /* Initialize head to NULL */

    /* Opening the file */
    strcpy(file_name, argv);
    strcat(file_name, ".am");
    printf("Going through file: %s\n", file_name);
    input_file = fopen(file_name, "r");

    /* Go through every line in the file */
    while (fgets(line, MAX, input_file)) {
        /* Check if it's a symbol declaration */
        char* symbol_name = extract_symbol(line);
        if (symbol_name != NULL) {
            /*char* symbol_name = extract_symbol(line);*/
            symbol_table* new_symbol;
            if (tail == NULL) {
                printf("Symbol: %s\n", symbol_name);
                tail = add(&tail, symbol_name);
                tail->value = temp_ic;
            }
            else if (check_if_exists(tail, symbol_name) == 0) {
                printf("Symbol: %s\n", symbol_name);
                new_symbol = add(&tail, symbol_name);
                new_symbol->value = temp_ic;

            }

            else  {printf("error: Symbol \"%s\" already exists in the table\n", symbol_name);}
            free(symbol_name);
        }
    }

    fclose(input_file);

    /* Free the memory for the symbol table nodes when done using the linked list */
    while (tail != NULL) {
        symbol_table* temp = tail;
        tail = tail->prev;
        free(temp);
    }

    return 0;
}