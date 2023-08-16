
#include <stdio.h>
#include <ctype.h>
#include "error_detection_pass.h"
#include "utils.h"
#include "instruction_handling.h"
#include "first_pass_through.h"


void validate_symbol_instruction(char* line, int line_number, struct symbol_table* symbol_head){
    int data_or_string_value;
    int entry_or_extern_value;
    int index;

    index = 0;
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
    data_or_string_value = handle_data_or_string(line, index, line_number, 1);

    /* Check if the label is a .entry or .extern prompt */
    entry_or_extern_value = classify_extern_or_entry(line, index);

    if (data_or_string_value == 0 && entry_or_extern_value == 0){
        if (get_instruction_line_amount(line, line_number, index, symbol_head, 1) == 0){
            error_free = 0;
            return;
        }
    }

}

void validate_non_symbol_instruction(char* line, int line_number, struct symbol_table* symbol_head){
    int data_or_string_value;
    int entry_or_extern_value;
    /* Check if it's an .entry or .extern prompt */
    entry_or_extern_value = classify_extern_or_entry(line, 0);
    /* Check if it's a .data or .string prompt */
    data_or_string_value = handle_data_or_string(line, 0, line_number, 1);
    if (data_or_string_value == 0 && entry_or_extern_value == 0){
        if (get_instruction_line_amount(line, line_number, 0, symbol_head, 1) == 0){
            error_free = 0;
            return;
        }
    }
}

void error_detection_pass (char* argv, struct symbol_table* symbol_head, int IC, int DC) {
    char line[MAX_LINE_LENGTH + 1];
    FILE *input_file;
    int line_number;
    char *symbol_name;

    line_number = 0;
    input_file = open_file(argv, ".am");

    /* Go through every line in the file */
    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)) {
        line_number++;
        symbol_name = extract_symbol(line);

        if (symbol_name != NULL) {
            validate_symbol_instruction(line, line_number, symbol_head);
        }
        else{
            validate_non_symbol_instruction(line, line_number, symbol_head);
        }
        free(symbol_name);
    }
    /* Check all entries have suitable symbol declarations, and update the data symbols' value */
    while(symbol_head != NULL){
        if (get_symbol_external_or_entry(symbol_head) == 2 && get_symbol_pre_defined_entry(symbol_head) == 1){
            printf("Error: Symbol %s is declared as entry but not defined\n", get_symbol(symbol_head));
            error_free = 0;
        }
        if (get_symbol_type(symbol_head) == 1){
            set_symbol_value(symbol_head, get_symbol_value(symbol_head) + IC);
        }
        symbol_head = get_next_symbol(symbol_head);
    }

    if (IC + DC + 100 > MAX_MEMORY_SIZE){
        printf("Error: Program is too big for memory\n");
        error_free = 0;
    }
    fclose(input_file);
}
