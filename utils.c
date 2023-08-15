#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "utils.h"

void free_tables(struct symbol_table* symbol_head){
    while (symbol_head != NULL) {
        struct symbol_table* temp;
        printf("current symbol: %s current value: %d\n", get_symbol(symbol_head), get_symbol_value(symbol_head));
        temp = symbol_head;
        symbol_head = get_next_symbol(symbol_head);
        free(temp);
    }
    free(symbol_head);
}

int get_instruction_decimal_code(char* instruction_name){
    int i;
    for (i = 0; i < 16; i++){
        if (strcmp(instruction_name, instruction_names[i]) == 0){
            return i;
        }
    }
    return -1;
}

FILE* open_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    return fopen(complete_file_name, "r");
}

FILE* create_output_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    return fopen(complete_file_name, "w");
}

