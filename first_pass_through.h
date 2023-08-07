
#ifndef FIRST_PASS_THROUGH_H
#define FIRST_PASS_THROUGH_H

#include "symbol_table.h"
#include "utils.h"


char* extract_symbol(const char line[]);
int first_pass_through(char* argv, symbol_table* symbol_head);

#endif
