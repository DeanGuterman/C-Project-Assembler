#ifndef INSTRUCTION_HANDLING_H
#define INSTRUCTION_HANDLING_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"

extern int error_free;
extern const char* instruction_names[];
extern const char* register_names[];

/* Find the index of an instruction in the instruction names array */
int find_instruction_index(char* token_instruction, int line_number, int check_errors);

/* Delete whitespaces and newline from a token */
void delete_whitespaces(char* token);

/* Check if a token represents a valid register */
int is_valid_register(char* token);

/* Check if a token represents a valid symbol */
int is_valid_symbol(char* token, struct symbol_table* symbol_head);

/* Check if a token represents a valid number */
int is_valid_number(char* token);

/* Check two-operand instructions */
int check_two_operand(int line_number, int instruction_index, int num_of_tokens, char* tokens[], struct symbol_table* symbol_head, int check_errors);

/* Check one-operand instructions */
int check_one_operand(int line_number, int instruction_index, int num_of_tokens, char* tokens[], struct symbol_table* symbol_head, int check_errors);

/* Check zero-operand instructions */
int check_zero_operand(int line_number, int instruction_index, int num_of_tokens, int check_errors);

/* Get the number of operands in an instruction line */
int get_instruction_line_amount(char line[], int line_number, int index, struct symbol_table* symbol_head, int check_errors);

#endif
