

#ifndef ASSEMBLER_PROJECT_SECOND_PASS_THROUGH_H
#define ASSEMBLER_PROJECT_SECOND_PASS_THROUGH_H
#include "symbol_table.h"
#include "utils.h"
#include "first_pass_through.h"
#include "processing_helpers.h"
#include "encoding_functions.h"
#include "create_output_files.h"

struct bitfield;

void second_pass_through(char* argv, struct symbol_table* symbol_head);
unsigned int get_bitfield_value(struct bitfield *bitfield);
struct bitfield *create_bitfield(void);
struct bitfield *char_to_bitfield(char c);
struct bitfield *num_to_bitfield(int num);
int twos_complement(int num);

#endif
