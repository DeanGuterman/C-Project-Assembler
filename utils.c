#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

void free_tables(symbol_table* symbol_head){
    printf("Freeing tables\n");
    while (symbol_head != NULL) {
        symbol_table* temp;
        temp = symbol_head;
        symbol_head = symbol_head->next;
        printf("Freeing %s\n", temp->symbol);
        free(temp);
    }
}

FILE* open_file(char* file_name, char* file_type){
    char complete_file_name[FILENAME_MAX];
    strcpy(complete_file_name, file_name);
    strcat(complete_file_name, file_type);
    printf("Going through file: %s\n", file_name);
    return fopen(complete_file_name, "r");
}

