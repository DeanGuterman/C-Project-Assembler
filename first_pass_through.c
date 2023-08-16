#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "symbol_table.h"
#include "processing_helpers.h"
#include "instruction_handling.h"
#include "first_pass_through.h"


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
    while (line[index] != ':' && line[index] != '\0') {
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

void handle_symbol(struct symbol_table *symbol_head, char line[], int line_number, char symbol_name[MAX_SYMBOL_LENGTH + 1], int *temp_ic, int *temp_dc){
    struct symbol_table *new_symbol;
    int entry_or_extern_value;
    int data_or_string_value;
    int index;

    index = 0;
    new_symbol = insert_symbol(symbol_head, symbol_name, *temp_ic, line_number);
    if (new_symbol == NULL) {
        error_free = 0;
        return;
    }
    /* Skip the symbol name */
    while(isspace(line[index])){
        index++;
    }
    while(!isspace(line[index])){
        index++;
        if (line[index] == ':'){
            index++;
            break;
        }
    }

    /* Check if the label is a .data or .string prompt */
    data_or_string_value = handle_data_or_string(line, index, line_number, 0);

    /* Check if the label is a .entry or .extern prompt */
    entry_or_extern_value = classify_extern_or_entry(line, index);

    if (data_or_string_value == -1){ /* If there was an error in the .data or .string prompt */
        error_free = 0;
        return;
    }
    else if (data_or_string_value > 0){ /* If it's a .data or .string prompt */
        set_symbol_value(new_symbol, *temp_dc);
        *temp_dc += data_or_string_value;
    }

    else if(entry_or_extern_value == 1){ /* If it's a .extern prompt */
        if (get_symbol_external_or_entry(new_symbol) != -1){
            printf("Error: Symbol %s is already defined as an extern or entry at line %d\n", get_symbol(new_symbol), line_number);
            error_free = 0;
            return;
        }
        set_symbol_external_or_entry(new_symbol, 1);
    }

    else if(data_or_string_value == 0){ /* If it's not a .data, .string, .extern, or .entry  prompt */
        *temp_ic += get_instruction_line_amount(line, line_number, index, symbol_head, 0);
    }
}

void handle_non_symbol(struct symbol_table *symbol_head, char line[], int line_number, int *temp_ic, int *temp_dc){
    int index;
    int entry_or_extern_value;
    int data_or_string_value;

    index = 0;
    /* Check if it's an .entry or .extern prompt */
    entry_or_extern_value = classify_extern_or_entry(line, index);
    /* Check if it's a .data or .string prompt */
    data_or_string_value = handle_data_or_string(line, index, line_number, 0);

    if (data_or_string_value != 0) {
        if (data_or_string_value == -1) { /* If there was an error in the .data or .string prompt */
            error_free = 0;
            return;
        } else if (data_or_string_value > 0) { /* If it's a .data or .string prompt */
            *temp_dc += data_or_string_value;
            *temp_ic += data_or_string_value;
        }
    }
    else if (entry_or_extern_value == 1) { /* If it's a .extern prompt */
        handle_extern_or_entry_symbol(line, symbol_head, index, 1, line_number, *temp_dc);
    } else if (entry_or_extern_value == 2) { /* If it's a .entry prompt */
        handle_extern_or_entry_symbol(line, symbol_head, index, 2, line_number, *temp_dc);
    }
    else{ /* If it's an instruction */
        *temp_ic += get_instruction_line_amount(line, line_number, index, symbol_head, 0);
    }
}

void first_pass_through(char* argv, struct symbol_table* symbol_head, int *IC, int *DC) {
    char line[MAX_LINE_LENGTH + 1];
    FILE *input_file;
    int temp_dc, temp_ic;
    char *symbol_name;
    int line_number;

    symbol_name = NULL;
    temp_dc = 0;
    temp_ic = 100;
    line_number = 0;
    input_file = open_file(argv, ".am");

    /* Go through every line in the file */
    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)) {
        line_number++;

        /* Check if it's a symbol declaration */
        symbol_name = extract_symbol(line);

        if (symbol_name != NULL) {
            handle_symbol(symbol_head, line, line_number, symbol_name, &temp_ic, &temp_dc);
            free(symbol_name);
        }

        else{
            handle_non_symbol(symbol_head, line, line_number, &temp_ic, &temp_dc);
        }
    }
    *IC = temp_ic;
    *DC = temp_dc;

    fclose(input_file);
}
