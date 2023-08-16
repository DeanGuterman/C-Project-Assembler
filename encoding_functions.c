#include <ctype.h>

#include "encoding_functions.h"

#include "second_pass_through.h"

#include "utils.h"

#include "instruction_handling.h"

#include "create_output_files.h"


/* Check if a given integer is within a valid range */
int is_valid_num(int num) {
    if (num > 2047 || num < -2048) {
        /* Return 0 to indicate that the number is not valid */
        return 0;
    }

    /* Return 1 to indicate that the number is valid */
    return 1;
}

/* Check if a given string can be converted to a valid operand number within the specified range */
int is_valid_operand_num(char num_string[]) {
    int num;

    /* Convert the string to an integer */
    num = atoi(num_string);

    /* Check if the integer is within the valid range */
    if (num > 511 || num < -512) {
        /* Return 0 to indicate that the operand is not valid */
        return 0;
    }

    /* Return 1 to indicate that the operand is valid */
    return 1;
}


/* Encode a double-operand instruction */
int encode_double_operand_instruction(char * tokens[], struct bitfield * instruction_array[], int current_instruction, int instruction_index, struct symbol_table * symbol_head, int line_number) {
    struct bitfield *instruction_opcode, *source_ARE, *destination_ARE, *source_method, *destination_method, *source_operand_address, *destination_operand_address;
    struct symbol_table *source_symbol, *destination_symbol;
    int source_operand_twos_complement, destination_operand_twos_complement, print_third_line;

    /* Shift the current_instruction to the left by 5 bits */
    instruction_opcode = num_to_bitfield(current_instruction << 5);
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
        source_method = num_to_bitfield(3 << 9);
        source_operand_address = num_to_bitfield(get_symbol_value(source_symbol) << 2);

        /* Check if the symbol is external or entry */
        if (get_symbol_external_or_entry(source_symbol) == 1) {
            source_ARE = num_to_bitfield(1);
            source_operand_address = num_to_bitfield(0);
            add_to_extern_list(tokens[1], instruction_index + 101);
        } else {
            source_ARE = num_to_bitfield(2);
        }
    } else {
        /* Handle numeric operand */
        if (is_valid_operand_num(tokens[1]) == 1) {
            source_method = num_to_bitfield(1 << 9);
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
        destination_method = num_to_bitfield(5 << 2);
        if (strncmp(tokens[1], "@", 1) == 0) {
            print_third_line = 0;
            destination_operand_address = num_to_bitfield((tokens[2][2] - '0' )<< 2);
        } else {
            destination_operand_address = num_to_bitfield((tokens[2][2] - '0' ) << 2);
            destination_ARE = num_to_bitfield(0);
        }
    } else if(destination_symbol != NULL){
        /* Handle symbol addressing */
        destination_method = num_to_bitfield(3 << 2);
        destination_operand_address = num_to_bitfield(get_symbol_value(destination_symbol) << 2);

        /* Check if the symbol is external or entry */
        if (get_symbol_external_or_entry(destination_symbol) == 1) {
            destination_ARE = num_to_bitfield(1);
            destination_operand_address = num_to_bitfield(0);
            add_to_extern_list(tokens[1], instruction_index + 102);
        } else {
            destination_ARE = num_to_bitfield(2);
        }
    } else {
        /* Handle numeric operand */
        if (is_valid_operand_num(tokens[2]) == 1) {
            destination_method = num_to_bitfield(1 << 2);
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

    /* Set the encoded instruction and operands into the instruction array */
    instruction_array[instruction_index++] = num_to_bitfield(
            get_bitfield_value(instruction_opcode) | get_bitfield_value(source_method) | get_bitfield_value(destination_method));

    if (print_third_line == 1) {
        instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(source_operand_address) | get_bitfield_value(source_ARE));
        instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(destination_operand_address) | get_bitfield_value(destination_ARE));
    } else {
        instruction_array[instruction_index++] = num_to_bitfield(get_bitfield_value(source_operand_address) | get_bitfield_value(destination_operand_address) | get_bitfield_value(source_ARE));
    }

    /* Free allocated memory */
    free(instruction_opcode);
    free(source_method);
    free(source_operand_address);
    free(source_ARE);
    free(destination_method);
    free(destination_operand_address);
    free(destination_ARE);

    /* Return the updated instruction index */
    return instruction_index;
}


/* Encode a single-operand instruction */
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
        destination_method = num_to_bitfield(5 << 2);
        operand_address = num_to_bitfield((tokens[1][2] - '0') << 2);
        ARE = num_to_bitfield(0);
    } else if (current_symbol != NULL) {
        /* Handle symbol addressing */
        destination_method = num_to_bitfield(12);
        operand_address = num_to_bitfield(get_symbol_value(current_symbol) << 2);

        /* Check if the symbol is external or entry */
        if (get_symbol_external_or_entry(current_symbol) == 1) {
            ARE = num_to_bitfield(1);
            free(operand_address);
            operand_address = num_to_bitfield(0);
            add_to_extern_list(tokens[1], instruction_index + 101);
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


/* Encode a zero-operand instruction */
int encode_zero_operand_instruction(struct bitfield *instruction_array[], int current_instruction, int instruction_index){
    struct bitfield *instruction_opcode;

    /* Calculate and set the instruction opcode by left-shifting the current instruction number by 5 bits */
    instruction_opcode = num_to_bitfield(current_instruction << 5);

    /* Store the instruction opcode in the instruction array at the specified index */
    instruction_array[instruction_index] = instruction_opcode;
    instruction_index++;

    /* Return the updated instruction index */
    return instruction_index;
}

/* Encode a given instruction based on the input line and parameters */
int encode_instruction(const char line[], int index, struct bitfield *instruction_array[], int instruction_index, int line_number, struct symbol_table *symbol_head) {
    char line_copy[MAX_LINE_LENGTH];
    char* token;
    char* tokens[3];
    int num_of_tokens;
    int current_instruction;
    int i;

    /* Create a copy of the line starting from the specified index */
    strcpy(line_copy, line + index);

    /* Tokenize the copied line using whitespace and newline characters as delimiters */
    token = strtok(line_copy, " \t\n");
    num_of_tokens = 0;

    /* Store tokens in an array */
    while (token != NULL) {
        tokens[num_of_tokens] = token;
        num_of_tokens++;
        token = strtok(NULL, ",");
    }

    /* Remove whitespaces from each token */
    for (i = 0; i < num_of_tokens; i++) {
        delete_whitespaces(tokens[i]);
    }

    /* Find the index of the current instruction in the instruction set */
    current_instruction = find_instruction_index(tokens[0], line_number, 0);

    if (current_instruction >= 0 && current_instruction <= 4) {
        if (current_instruction == 4)
            current_instruction = 6;
        /* Encode a double operand instruction */
        return encode_double_operand_instruction(tokens, instruction_array, current_instruction, instruction_index, symbol_head, line_number);
    }
    else if (current_instruction >= 5 && current_instruction <= 13) {
        if (current_instruction == 6)
            current_instruction = 4;
        /* Encode a single operand instruction */
        return encode_single_operand_instruction(tokens, instruction_array, current_instruction, instruction_index, symbol_head, line_number);
    }
    else if (current_instruction >= 14) {
        /* Encode a zero operand instruction */
        return encode_zero_operand_instruction(instruction_array, current_instruction, instruction_index);
    }

    /* Return the instruction index */
    return instruction_index;
}


/* Encode a string literal into the data array */
int encode_string(const char line[], int index, struct bitfield * data_array[], int data_index) {
    /* Skip characters until the opening double-quote is encountered */
    while (line[index] != '"') {
        index++;
    }

    index++; /* Skip the opening double-quote */

    /* Process characters within the string */
    while (line[index] != '"') {
        /* Convert the current character to a bitfield and store it in the data array */
        data_array[data_index] = char_to_bitfield(line[index]);
        index++;
        data_index++;
    }

    /* Append a null terminator to represent the end of the string */
    data_array[data_index] = char_to_bitfield('\0');
    data_index++;

    /* Return the updated data index */
    return data_index;
}


/* Encode data values from a given line into the data array */
int encode_data(const char line[], int index, struct bitfield * data_array[], int data_index, int line_number) {
    int modifier;
    int num;

    /* Loop until the end of the line is reached */
    while (line[index] != '\n') {
        num = 0;

        /* Skip whitespace characters */
        while (isspace(line[index])) {
            index++;
        }

        /* Determine if a modifier is present */
        if (line[index] == '-') {
            modifier = -1;
            index++;
        } else if (line[index] == '+') {
            modifier = 1;
            index++;
        } else if (!isdigit(line[index])) {
            index++;
            continue;  /* Skip characters that are not digits or modifiers */
        } else {
            modifier = 1;  /* Default modifier if not specified */
        }

        /* Parse the numeric value */
        while (line[index] != '\n' && line[index] != ',') {
            if (isdigit(line[index])) {
                num = num * 10 + (line[index] - '0');
            }
            index++;
        }

        /* Process the parsed value */
        if (line[index] == ',' || line[index] == '\n') {
            if (is_valid_num(num)) {
                /* Apply modifier and two's complement if necessary */
                if (modifier == -1) {
                    num *= -1;
                    num = twos_complement(num);
                    num |= (1 << 11);
                }
                /* Store the encoded numeric value in the data array */
                data_array[data_index] = num_to_bitfield(num);
                data_index++;
            } else {
                /* Print an error message for invalid numbers */
                printf("Error: invalid number in .data instruction at line %d\n", line_number);
                error_free = 0;
                return data_index;
            }

            /* Check if the end of the line has been reached */
            if (line[index] == '\n')
                return data_index;

            index++;  /* Move past the comma separator */
        }
    }

    /* Return the updated data index */
    return data_index;
}

