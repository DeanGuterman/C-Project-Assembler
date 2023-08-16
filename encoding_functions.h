

#ifndef ASSEMBLER_PROJECT_ENCODING_FUNCTIONS_H
#define ASSEMBLER_PROJECT_ENCODING_FUNCTIONS_H

#include <ctype.h>

#include "second_pass_through.h"

#include "utils.h"

#include "instruction_handling.h"

#include "create_output_files.h"

struct bitfield;

/* Check if a number is within the valid range */
int is_valid_num(int num);

/* Check if a numeric operand is within the valid range */
int is_valid_operand_num(char num_string[]);

/* Encode a double-operand instruction */
int encode_double_operand_instruction(char* tokens[], struct bitfield* instruction_array[], int current_instruction, int instruction_index, struct symbol_table* symbol_head, int line_number);

/* Encode a single-operand instruction */
int encode_single_operand_instruction(char* tokens[], struct bitfield* instruction_array[], int current_instruction, int instruction_index, struct symbol_table* symbol_head, int line_number);

/* Encode a zero-operand instruction */
int encode_zero_operand_instruction(struct bitfield* instruction_array[], int current_instruction, int instruction_index);

/* Encode an instruction based on the line content */
int encode_instruction(const char line[], int index, struct bitfield* instruction_array[], int instruction_index, int line_number, struct symbol_table* symbol_head);

/* Encode a string data */
int encode_string(const char line[], int index, struct bitfield* data_array[], int data_index);

/* Encode data in a .data instruction */
int encode_data(const char line[], int index, struct bitfield* data_array[], int data_index, int line_number);

#endif
