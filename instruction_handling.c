
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "instruction_handling.h"

char* reserved_names[] = RESERVED_NAMES;
char* instruction_names[] = INSTRUCTION_NAMES;
char* register_names[] = REGISTER_NAMES;



int find_instruction_index(char* token_instruction, int line_number, int check_errors){

    int i;
    for (i = 0; i < 16; i++){
        if (strcmp(token_instruction, instruction_names[i]) == 0){
            return i;
        }
    }
    if(check_errors == 1) {
        printf("Error: line %d has an invalid instruction name\n", line_number);
        error_free = 0;
    }
    return -1;
}

void delete_whitespaces(char* token){
    int i;
    int j;
    int token_length = strlen(token);
    for (i = 0; i < token_length; i++){
        if (token[i] == '\n'){
            token[i] = '\0';
        }
        if (token[i] == ' ' || token[i] == '\t'){
            for (j = i; j < token_length; j++){
                token[j] = token[j + 1];
            }
            token_length--;
            i--;
        }
    }
}

int is_valid_register(char* token){
    int i;
    for (i = 0; i < 8; i++){
        if (strcmp(token, register_names[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int is_valid_symbol(char* token, struct symbol_table *symbol_head) {
    if (search_symbol(symbol_head, token) != NULL) {
        return 1;
    }
    else {
        return 0;
    }
}

int is_valid_number(char* token){
    if (*token == '+' || *token == '-'){
        token++;
    }

    while (*token != '\0'){
        if (!isdigit(*token)){
            return 0;
        }
        token++;
    }
    return 1;
}

int check_two_operand(int line_number, int instruction_index, int num_of_tokens, char* tokens[], struct symbol_table *symbol_head, int check_errors){
    if (num_of_tokens != 3){
        if (check_errors == 1) {
            printf("Error: line %d has an inadequate amount of arguments, instruction '%s' should have two arguments\n",
                   line_number, instruction_names[instruction_index]);
            error_free = 0;
        }
        return 0;
    }
    /* delete whitespaces from the second and third token */
    delete_whitespaces(tokens[1]);
    delete_whitespaces(tokens[2]);

    /* Check if the operands are valid */
    if (!strcmp(tokens[0], "lea")){
        if (is_valid_symbol(tokens[1], symbol_head)){
            if (is_valid_register(tokens[2]) || is_valid_symbol(tokens[2], symbol_head)){
                return 3;
            }
        }
        if (check_errors == 1){
            printf("Error: line %d one or more invalid arguments\n", line_number);
            error_free = 0;
        }
        return 0;
    }
    else if (is_valid_register(tokens[1]) && is_valid_register(tokens[2])){
        return 2; /*In this case, both operands are registers*/
    }
    else if(is_valid_symbol(tokens[1], symbol_head) && is_valid_symbol(tokens[2], symbol_head)){
        return 3;
    }
    else if (is_valid_register(tokens[1]) && is_valid_symbol(tokens[2], symbol_head)){
        return 3;
    }
    else if(is_valid_symbol(tokens[1], symbol_head) && is_valid_register(tokens[2])){
        return 3;
    }
    else if(is_valid_number(tokens[1]) && is_valid_number(tokens[2]) && !strcmp(tokens[0], "cmp")){
        return 3;
    }
    else if(is_valid_number(tokens[1]) && is_valid_symbol(tokens[2], symbol_head)){
        return 3;
    }
    else if(is_valid_number(tokens[1]) && is_valid_register(tokens[2])){
        return 3;
    }
    else {
        if(check_errors == 1) {
            printf("Error: line %d has one or more invalid arguments\n", line_number);
        }
        return 0;
    }
}

int check_one_operand(int line_number, int instruction_index, int num_of_tokens, char* tokens[], struct symbol_table *symbol_head, int check_errors){
    if (num_of_tokens != 2){
        if(check_errors == 1) {
            printf("Error: line %d has an inadequate amount of arguments, instruction '%s' should have one argument\n",
                   line_number, instruction_names[instruction_index]);
            error_free = 0;
        }
        return 0;
    }
    /* delete whitespaces from the second token */
    delete_whitespaces(tokens[1]);

    /* Check if the operand is valid */
    if (is_valid_register(tokens[1])){
        return 2;
    }
    else if (is_valid_symbol(tokens[1], symbol_head)){
        return 2;
    }
    else if(is_valid_number(tokens[1]) && !strcmp(instruction_names[instruction_index], "prn")){
        return 2;
    }
    else{
        if(check_errors == 1) {
            printf("Error: line %d has an invalid argument\n", line_number);
            error_free = 0;
        }
        return 0;
    }
}

int check_zero_operand(int line_number, int instruction_index, int num_of_tokens, int check_errors){
    if (num_of_tokens != 1){
        if (check_errors == 1) {
            printf("Error: line %d has an inadequate amount of arguments, instruction '%s' should be argument-less\n",
                   line_number, instruction_names[instruction_index]);
            error_free = 0;
        }
        return 0;
    }
    else return 1;
}

int get_instruction_line_amount(char line[], int line_number, int index, struct symbol_table *symbol_head, int check_errors) {
    char line_copy[MAX_LINE_LENGTH];
    char* token;
    char* tokens[3];
    int num_of_tokens;
    int instruction_index;

    /* Copy the line content starting from the given index */
    strcpy(line_copy, line + index);

    /* Tokenize the line to count the number of operands */
    token = strtok(line_copy, " \t\n");
    num_of_tokens = 0;
    while (token != NULL) {
        tokens[num_of_tokens] = token;
        num_of_tokens++;
        token = strtok(NULL, ",");
    }

    /* Check for excessive number of operands */
    if (num_of_tokens > 3) {
        if (check_errors == 1) {
            printf("Error: instruction at line %d has too many arguments\n", line_number);
            error_free = 0;
        }
        return 0;
    }

    /* Find the index of the instruction in the instruction list */
    instruction_index = find_instruction_index(tokens[0], line_number, check_errors);

    /* Determine the number of operands based on the instruction type */
    if (instruction_index >= 0 && instruction_index <= 4) {
        if (instruction_index == 4)
            instruction_index = 6;
        if (check_errors == 0) {
            if (check_two_operand(line_number, instruction_index, num_of_tokens, tokens, symbol_head, check_errors) == 2) {
                return 2;
            } else {
                return 3;
            }
        } else {
            return check_two_operand(line_number, instruction_index, num_of_tokens, tokens, symbol_head, check_errors);
        }
    } else if (instruction_index >= 5 && instruction_index <= 13) {
        if (instruction_index == 6)
            instruction_index = 4;
        if (check_errors == 0) {
            return 2;
        } else {
            return check_one_operand(line_number, instruction_index, num_of_tokens, tokens, symbol_head, check_errors);
        }
    } else if (instruction_index >= 14) {
        if (check_errors == 0) {
            return 1;
        } else {
            return check_zero_operand(line_number, instruction_index, num_of_tokens, check_errors);
        }
    } else {
        return 0;
    }
}
