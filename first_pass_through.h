
#ifndef FIRST_PASS_THROUGH_H
#define FIRST_PASS_THROUGH_H


#include "utils.h"

/* Extracts a symbol from a line */
char* extract_symbol(const char line[]);







/* Handle a line with a symbol declaration */
void handle_symbol(struct symbol_table *symbol_head, char line[], int line_number, char symbol_name[MAX_SYMBOL_LENGTH + 1], int *temp_ic, int *temp_dc);

/* Handle a line without a symbol declaration */
void handle_non_symbol(struct symbol_table *symbol_head, char line[], int line_number, int *temp_ic, int *temp_dc);

/* First pass through the input file */
void first_pass_through(char* argv, struct symbol_table* symbol_head, int *IC, int *DC);

#endif
