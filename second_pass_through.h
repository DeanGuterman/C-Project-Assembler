

#ifndef SECOND_PASS_THROUGH_H
#define SECOND_PASS_THROUGH_H
#include "symbol_table.h"
#include "utils.h"
#include "first_pass_through.h"
#include "processing_helpers.h"
#include "encoding_functions.h"
#include "create_output_files.h"

struct bitfield;

struct bitfield *create_bitfield(void);
unsigned int get_bitfield_value(struct bitfield *bitfield);
struct bitfield *char_to_bitfield(char c);
int twos_complement(int num);
struct bitfield *num_to_bitfield(int num);
void print_binary(unsigned int value);
void free_bitfield_array(struct bitfield *array[]);
void second_pass_through(char* argv, struct symbol_table* symbol_head);

#endif
