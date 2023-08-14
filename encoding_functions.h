

#ifndef ASSEMBLER_PROJECT_ENCODING_FUNCTIONS_H
#define ASSEMBLER_PROJECT_ENCODING_FUNCTIONS_H

#include "second_pass_through.h"

struct bitfield;

int encode_string(const char line[], int index, struct bitfield *data_array[], int data_index);
int encode_data(const char line[], int index, struct bitfield *data_array[], int data_index, int line_number);
int encode_instruction(const char line[], int index, struct bitfield *instruction_array[], int instruction_index, int line_number);

#endif
