#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#define MAX 100


int first_pass_through(char * argv){
    /* Opening the file */
    char file_name[MAX];
    char line[MAX];
    strcpy(file_name, argv);
    strcat(file_name, ".am");
    printf("Going through file: %s\n", file_name);
    FILE * input_file = fopen(file_name, "r");


    while(fgets(line, MAX, input_file)){
        if(is_symbol(line)){

        }
    }
}