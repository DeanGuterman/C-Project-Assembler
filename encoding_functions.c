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

int encode_double_operand_instruction(char * tokens[], struct bitfield * instruction_array[], int current_instruction, int instruction_index, struct symbol_table * symbol_head, int line_number) {
    struct bitfield *instruction_opcode, *source_ARE, *destination_ARE, *source_method, *destination_method, *source_operand_address, *destination_operand_address;
    struct symbol_table *source_symbol, *destination_symbol;
    int source_operand_twos_complement, destination_operand_twos_complement, print_third_line;

    /* Shift the current_instruction to the left by 5 bits */
    current_instruction <<= 5;
    instruction_opcode = num_to_bitfield(current_instruction);
    print_third_line = 1;
    destination_ARE = NULL;

    source_symbol = search_symbol(symbol_head, tokens[1]);
    destination_symbol = search_symbol(symbol_head, tokens[2]);

    /* Check if the operand starts with '@' */
    if (strncmp(tokens[1], "@", 1) == 0) {
        /* Handle direct addressing */
        source_method = num_to_bitfield(5 << 9);
        source_operand_address = num_to_bitfield((tokens[1][2] - '0') << 7);
        source_ARE = num_to_bitfield(0);
    } else if (source_symbol != NULL) {
        /* Handle symbol addressing */
        source_method = num_to_bitfield(12);
        source_operand_address = num_to_bitfield(get_symbol_value(source_symbol) << 2);

        /* Check if the symbol is external or entry */
        if (get_symbol_external_or_entry(source_symbol) == 1) {
            source_ARE = num_to_bitfield(1);
        } else {
            source_ARE = num_to_bitfield(2);
        }
    } else {
        /* Handle numeric operand */
        if (is_valid_operand_num(tokens[1]) == 1) {
            source_method = num_to_bitfield(4);
            source_operand_twos_complement = twos_complement(atoi(tokens[1]));

            /* Check if the operand is negative */
            if (atoi(tokens[1]) < 0) {
                source_operand_twos_complement |= (1 << 8);
            }
            source_operand_address = num_to_bitfield(source_operand_twos_complement << 2);
            source_ARE = num_to_bitfield(0);
        } else {
            /* Invalid operand, print error message and set error flag */
            printf("Error: invalid operand at line %d\n", line_number);
            error_free = 0;
            return instruction_index;
        }
    }


    if (strncmp(tokens[2], "@", 1) == 0) {
        destination_method = num_to_bitfield(20);
        if (strncmp(tokens[1], "@", 1) == 0) {
            printf("current instruction: %d\n", current_instruction);
            print_third_line = 0;
            destination_operand_address = num_to_bitfield((tokens[2][2] - '0' )<< 2);
        } else {
            destination_operand_address = num_to_bitfield((tokens[2][2] - '0' ) << 7);
            destination_ARE = num_to_bitfield(0);
        }
    } else if(destination_symbol != NULL){
        /* Handle symbol addressing */
        destination_method = num_to_bitfield(3 << 2);
        destination_operand_address = num_to_bitfield(get_symbol_value(destination_symbol) << 2);

        /* Check if the symbol is external or entry */
        if (get_symbol_external_or_entry(destination_symbol) == 1) {
            destination_ARE = num_to_bitfield(1);
        } else {
            destination_ARE = num_to_bitfield(2);
        }
    } else {
        /* Handle numeric operand */
        if (is_valid_operand_num(tokens[2]) == 1) {
            destination_method = num_to_bitfield(4);
            destination_operand_twos_complement = twos_complement(atoi(tokens[2]));

            /* Check if the operand is negative */
            if (atoi(tokens[2]) < 0) {
                destination_operand_twos_complement |= (1 << 8);
            }
            destination_operand_address = num_to_bitfield(destination_operand_twos_complement << 2);
            destination_ARE = num_to_bitfield(0);
        } else {
            /* Invalid operand, print error message and set error flag */
            printf("Error: invalid operand at line %d\n", line_number);
            error_free = 0;
            return instruction_index;
        }
    }

    instruction_array[instruction_index++] = num_to_bitfield(
            get_bitfield_value(instruction_opcode) | get_bitfield_value(source_method) | get_bitfield_value(destination_method));

    if (print_third_line == 1) {
        instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(source_operand_address) | get_bitfield_value(source_ARE));
        instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(destination_operand_address) | get_bitfield_value(destination_ARE));
    } else {
        instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(source_operand_address) | get_bitfield_value(destination_operand_address) | get_bitfield_value(source_ARE));
    }

    free(instruction_opcode);
    free(source_method);
    free(source_operand_address);
    free(source_ARE);
    free(destination_method);
    free(destination_operand_address);
    free(destination_ARE);
    return instruction_index;
}



int encode_single_operand_instruction(char* tokens[], struct bitfield *instruction_array[], int current_instruction, int instruction_index, struct symbol_table *symbol_head, int line_number) {
    struct bitfield *instruction_opcode, *ARE, *destination_method, *operand_address;
    struct symbol_table *current_symbol;
    int operand_twos_complement;

    /* Shift the current_instruction to the left by 5 bits */
    current_instruction <<= 5;
    instruction_opcode = num_to_bitfield(current_instruction);

    /* Search for the current symbol in the symbol table */
    current_symbol = search_symbol(symbol_head, tokens[1]);

    /* Check if the operand starts with '@' */
    if (strncmp(tokens[1], "@", 1) == 0) {
        /* Handle direct addressing */
        destination_method = num_to_bitfield(20);
        operand_address = num_to_bitfield(tokens[1][2] << 7);
        ARE = num_to_bitfield(0);
    } else if (current_symbol != NULL) {
        /* Handle symbol addressing */
        destination_method = num_to_bitfield(12);
        operand_address = num_to_bitfield(get_symbol_value(current_symbol) << 2);

        /* Check if the symbol is external or entry */
        if (get_symbol_external_or_entry(current_symbol) == 1) {
            ARE = num_to_bitfield(1);
        } else {
            ARE = num_to_bitfield(2);
        }
    } else {
        /* Handle numeric operand */
        if (is_valid_operand_num(tokens[1]) == 1) {
            destination_method = num_to_bitfield(4);
            operand_twos_complement = twos_complement(atoi(tokens[1]));

            /* Check if the operand is negative */
            if (atoi(tokens[1]) < 0) {
                operand_twos_complement |= (1 << 8);
            }
            operand_address = num_to_bitfield(operand_twos_complement << 2);
            ARE = num_to_bitfield(0);
        } else {
            /* Invalid operand, print error message and set the error flag */
            printf("Error: invalid operand at line %d\n", line_number);
            error_free = 0;
            return instruction_index;
        }
    }

    /* Store instruction components in instruction_array */
    instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(instruction_opcode) | get_bitfield_value(destination_method));
    instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(operand_address) | get_bitfield_value(ARE));

    /* Free dynamically allocated memory */
    free(instruction_opcode);
    free(destination_method);
    free(operand_address);
    free(ARE);

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
        return encode_double_operand_instruction(tokens, instruction_array, current_instruction, instruction_index, symbol_head, line_number);
    }
    else if(current_instruction >= 6 && current_instruction <= 13){
        return   encode_single_operand_instruction(tokens, instruction_array, current_instruction, instruction_index, symbol_head, line_number);
    }
    else if (current_instruction >= 14){
       return encode_zero_operand_instruction(tokens, instruction_array, current_instruction, instruction_index);
    }

    return instruction_index;
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
                    num *= -1;
                    num = twos_complement(num);
                    num |= (1 << 11);
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