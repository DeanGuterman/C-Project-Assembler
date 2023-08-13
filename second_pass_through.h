

#ifndef ASSEMBLER_PROJECT_SECOND_PASS_THROUGH_H
#define ASSEMBLER_PROJECT_SECOND_PASS_THROUGH_H
#include "second_pass_through.h"
#include "symbol_table.h"

struct bitfield;

void second_pass_through(char* argv, struct symbol_table* symbol_head);

#endif
