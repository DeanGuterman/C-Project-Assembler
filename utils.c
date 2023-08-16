#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "utils.h"

/* Free symbol table */
void free_tables(struct symbol_table* symbol_head){
    while (symbol_head != NULL) {
        struct symbol_table* temp;
        temp = symbol_head;
        symbol_head = get_next_symbol(symbol_head);
        free(temp);
    }
    free(symbol_head);
}

/* Open pre-existing file */
FILE* open_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    return fopen(complete_file_name, "r");
}

/* Create output file */
FILE* create_output_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    return fopen(complete_file_name, "w");
}

