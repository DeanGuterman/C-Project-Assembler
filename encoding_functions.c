#include <ctype.h>

#include "encoding_functions.h"

#include "second_pass_through.h"

#include "utils.h"

#include "instruction_handling.h"


int is_valid_num(int num) {
    if (num > 2047 || num < -2048) {
        return 0;
    }
    return 1;
}

int is_valid_operand_num(char num_string[]){
    int num;
    num = atoi(num_string);
    if (num > 511 || num < -512){
        return 0;
    }
    return 1;
}


int encode_single_operand_instruction(char* tokens[], struct bitfield *instruction_array[], int current_instruction, int instruction_index, struct symbol_table *symbol_head, int line_number){
    struct bitfield *instruction_opcode;
    struct bitfield *ARE;
    struct bitfield *destination_method;
    struct bitfield *operand_address;
    struct symbol_table *current_symbol;
    int operand_twos_complement;

    current_instruction <<= 5;
    instruction_opcode = num_to_bitfield(current_instruction);
    current_symbol = search_symbol(symbol_head, tokens[1]);

    if (strncmp(tokens[1], "@", 1) == 0){
        destination_method = num_to_bitfield(20);
        operand_address = num_to_bitfield(tokens[1][2]<<7);
        ARE = num_to_bitfield(0);
    }
    else if(current_symbol != NULL){
        destination_method = num_to_bitfield(12);
        operand_address = num_to_bitfield(get_symbol_value(current_symbol)<<2);
        if(get_symbol_external_or_entry(current_symbol) == 1){
            ARE = num_to_bitfield(1);
        }
        else{
            ARE = num_to_bitfield(2);
        }
    }
    else {
        if (is_valid_operand_num(tokens[1]) == 1){
            destination_method = num_to_bitfield(4);
            operand_twos_complement = twos_complement(atoi(tokens[1]));

            if (atoi(tokens[1]) < 0){
                operand_twos_complement |= (1<<9);
            }
            operand_address = num_to_bitfield(operand_twos_complement<<2);
            printf("operand address is %d\n", get_bitfield_value(operand_address));
            printf("operand is %d\n", atoi(tokens[1]));
            printf("operand twos complement is %d\n", operand_twos_complement);

            ARE = num_to_bitfield(0);
        }
        else{
            printf("Error: invalid operand at line %d\n", line_number);
            error_free = 0;
            return instruction_index;
        }
    }
    instruction_array[instruction_index] = num_to_bitfield(get_bitfield_value(instruction_opcode) | get_bitfield_value(destination_method));
    printf("instruction array at index %d is: %d\n", instruction_index, get_bitfield_value(instruction_array[instruction_index]));
    instruction_index++;
    instruction_array[instruction_index] = num_to_bitfield(get_bitfield_value(operand_address) | get_bitfield_value(ARE));
    printf("instruction array at index %d is: %d\n", instruction_index, get_bitfield_value(instruction_array[instruction_index]));
    instruction_index++;
    return instruction_index;
}

int encode_zero_operand_instruction(char* tokens[], struct bitfield *instruction_array[], int current_instruction, int instruction_index){
    struct bitfield *instruction_opcode;

    instruction_opcode = num_to_bitfield(current_instruction);
    instruction_array[instruction_index] = instruction_opcode;
    instruction_index++;

    return instruction_index;
}

int encode_instruction(const char line[], int index, struct bitfield *instruction_array[], int instruction_index, int line_number, struct symbol_table *symbol_head){
    char line_copy[MAX_LINE_LENGTH];
    char* token;
    char* tokens[3];
    int num_of_tokens;
    int current_instruction;
    int i;


    strcpy(line_copy, line + index);
    token = strtok(line_copy, " \t\n");
    num_of_tokens = 0;

    while (token != NULL){
        tokens[num_of_tokens] = token;
        num_of_tokens++;
        token = strtok(NULL, ",");
    }

    for (i = 0; i < num_of_tokens; i++){
        delete_whitespaces(tokens[i]);
    }

    current_instruction = find_instruction_index(tokens[0], line_number, 0);

    if(current_instruction >= 0 && current_instruction <= 5){
        return instruction_index /*  encode_double_operand_instruction(tokens, instruction_array, current_instruction, instruction_index)*/;
    }
    else if(current_instruction >= 6 && current_instruction <= 13){
        return   encode_single_operand_instruction(tokens, instruction_array, current_instruction, instruction_index, symbol_head, line_number);
    }
    else if (current_instruction >= 14){
       return encode_zero_operand_instruction(tokens, instruction_array, current_instruction, instruction_index);
    }

    /*else*/ return instruction_index;
}

int encode_string(const char line[], int index, struct bitfield * data_array[], int data_index) {
    while (line[index] != '"') {
        index++;
    }

    index++;
    while (line[index] != '"') {
        data_array[data_index] = char_to_bitfield(line[index]);
        index++;
        data_index++;
    }
    data_array[data_index] = char_to_bitfield('\0');
    data_index++;
    return data_index;
}

int encode_data(const char line[], int index, struct bitfield * data_array[], int data_index, int line_number) {
    int modifier;
    int num;
    while (line[index] != '\n') {
        num = 0;
        while (isspace(line[index])) {
            index++;
        }

        if (line[index] == '-') {
            modifier = -1;
            index++;
        } else if (line[index] == '+') {
            modifier = 1;
            index++;
        } else {
            modifier = 1;
        }

        while (line[index] != '\n' && line[index] != ',') {
            if (isdigit(line[index])) {
                num = num * 10 + (line[index] - '0');
            }
            index++;
        }

        if (line[index] == ',' || line[index] == '\n') {
            if (is_valid_num(num)) {
                if (modifier == -1) {
                    num = twos_complement(num);
                }
                data_array[data_index] = num_to_bitfield(num);
                data_index++;
            } else {
                printf("Error: invalid number in .data instruction at line %d\n", line_number);
                error_free = 0;
                return data_index;
            }
            if (line[index] == '\n')
                return data_index;
            index++;
        }
    }

    return data_index;
}