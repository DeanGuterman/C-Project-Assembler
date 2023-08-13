#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "second_pass_through.h"

int error_free;
int contains_extern;
int contains_entry;
const char* reserved_names[] = RESERVED_NAMES;
const char* instruction_names[] = INSTRUCTION_NAMES;
const char* register_names[] = REGISTER_NAMES;

void free_tables(struct symbol_table* symbol_head){
    printf("Freeing tables\n");
    while (symbol_head != NULL) {
        struct symbol_table* temp;
        temp = symbol_head;
        symbol_head = get_next_symbol(symbol_head);
        free(temp);
    }
    free(symbol_head);
}


FILE* open_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    printf("Going through file: %s\n", complete_file_name);
    return fopen(complete_file_name, "r");
}

FILE* create_output_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    printf("Creating output file: %s\n", complete_file_name);
    return fopen(complete_file_name, "w");
}

