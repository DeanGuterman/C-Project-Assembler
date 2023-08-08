#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

int error_free;
const char* reserved_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "data", "string", "entry", "extern", "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};

void free_tables(symbol_table* symbol_head){
    printf("Freeing tables\n");
    while (symbol_head != NULL) {
        symbol_table* temp;
        temp = symbol_head;
        symbol_head = symbol_head->next;
        printf("Freeing %s\n", temp->symbol);
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

