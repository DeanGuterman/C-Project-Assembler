
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "instruction_handling.h"

extern int error_free;
extern const char* instruction_names[];

int find_instruction_index(char* token_instruction, int line_number){

    int i;
    for (i = 0; i < 16; i++){
        if (strcmp(token_instruction, instruction_names[i]) == 0){
            return i;
        }
    }
    printf("Error: line %d has an invalid instruction name\n", line_number);
    error_free = 0;
    return -1;
}

int handle_zero_operand(char line[], int line_number, int instruction_index, int num_of_tokens){
    if (num_of_tokens != 1){
        printf("Error: line %d has too many arguments, instruction '%s' should be argument-less\n", line_number, instruction_names[instruction_index]);
        error_free = 0;
        return 0;
    }
    else return 1;
}

int get_instruction_line_amount(char line[], int line_number, int index){
    /* instruction names variables */
    char line_copy[MAX_LINE_LENGTH];
    char* token;
    char* tokens[3];
    int num_of_tokens;
    int instruction_index;

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
        return 0;
    }
    instruction_index = find_instruction_index(tokens[0], line_number);
    if (instruction_index == -1){
        return 0;
    }
    else if (instruction_index >= 14){
        return handle_zero_operand(line, line_number, instruction_index, num_of_tokens);
    }

}
