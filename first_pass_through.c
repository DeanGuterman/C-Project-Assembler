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

    symbol_name = NULL;
    temp_dc = 0;
    temp_ic = 100;
    input_file = open_file(argv, ".am");

    /* Go through every line in the file */
    while (fgets(line, MAX, input_file)) {
        /* Check if it's a symbol declaration */
        symbol_name = extract_symbol(line);

        if (symbol_name != NULL) {
            new_symbol = insert_symbol(symbol_head, symbol_name);
            if (symbol_head == NULL){
                symbol_head = new_symbol;
                printf("Symbol %s is the new head\n", new_symbol->symbol);
            }
            new_symbol->value = temp_ic;
            free(symbol_name);
        }
    }
    fclose(input_file);
    return 1;
}