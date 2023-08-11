
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "instruction_handling.h"

extern int error_free;

void tokenize_instruction(char line[], int line_number, int index){
    char line_copy[MAX_LINE_LENGTH];
    char* token;
    char* tokens[3];
    int num_of_tokens;
    strcpy(line_copy, line + index);
    token = strtok(line_copy, " \t\n");
    num_of_tokens = 0;
    while (token != NULL){
        tokens[num_of_tokens] = token;
        num_of_tokens++;
        printf("Token %d: %s\n", num_of_tokens, token);
        token = strtok(NULL, ",");
    }
    if (num_of_tokens > 3){
        printf("Error: line %d has too many arguments\n", line_number);
        error_free = 0;
        return;
    }
}
