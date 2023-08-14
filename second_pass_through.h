

#ifndef ASSEMBLER_PROJECT_SECOND_PASS_THROUGH_H
#define ASSEMBLER_PROJECT_SECOND_PASS_THROUGH_H
#include "second_pass_through.h"
#include "symbol_table.h"

struct bitfield;

void second_pass_through(char* argv, struct symbol_table *symbol_head);
struct bitfield *create_bitfield(void);
struct bitfield *char_to_bitfield(char c);
struct bitfield *num_to_bitfield(int num);
int twos_complement(int num);

#endif
