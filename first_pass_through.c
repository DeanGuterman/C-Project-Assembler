#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbol_table.h"
#include "utils.h"
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

int first_pass_through(char* argv, symbol_table* symbol_head) {
    char line[MAX];
    FILE *input_file;
    int temp_dc, temp_ic;
    char *symbol_name;
    symbol_table *new_symbol;
    int symbol_table_empty;

    symbol_table_empty = 1;
    temp_dc = 0;
    temp_ic = 100;
    input_file = open_file(argv, ".am");

    /* Go through every line in the file */
    while (fgets(line, MAX, input_file)) {
        /* Check if it's a symbol declaration */
        symbol_name = extract_symbol(line);
        if (symbol_name != NULL) {
            if (symbol_table_empty){
                symbol_head = insert_first_symbol(symbol_name);
                symbol_table_empty = 0;
                printf("Symbol: %s\n", symbol_name);
            }
            else if (symbol_exists(symbol_head, symbol_name) == 0){
                printf("Symbol: %s\n", symbol_name);
                new_symbol = insert_symbol(symbol_head, symbol_name);
                new_symbol->value = temp_ic;
                if (symbol_head == NULL){
                    symbol_head = new_symbol;
                }
            } else {
                printf("error: Symbol \"%s\" already exists in the table\n", symbol_name);
            }
            free(symbol_name);
        }
    }
    /*while (symbol_head != NULL) {
        symbol_table* temp;
        temp = symbol_head;
        symbol_head = symbol_head->next;
        printf("Freeing %s\n", temp->symbol);
        free(temp);
    }*/
    fclose(input_file);
    return 1;
}