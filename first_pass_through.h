
#ifndef FIRST_PASS_THROUGH_H
#define FIRST_PASS_THROUGH_H

#include "symbol_table.h"
#include "utils.h"


char* extract_symbol(const char line[]);
int handle_string(char line[], int index, int line_number);
int handle_data(char line[], int index, int line_number);
int handle_data_or_string(char line[], int index, int line_number);
int handle_entry_or_extern(char line[]);
void first_pass_through(char* argv, symbol_table* symbol_head);

#endif
