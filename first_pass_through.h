
#ifndef FIRST_PASS_THROUGH_H
#define FIRST_PASS_THROUGH_H

#include "symbol_table.h"
#include "utils.h"


/* Function to extract a symbol from a line */
char* extract_symbol(const char line[]);

/* Function to handle a .string prompt and count characters */
int handle_string(char line[], int index, int line_number);

/* Function to handle a .data prompt and count integers */
int handle_data(char line[], int index, int line_number);

/* Function to handle .data or .string prompts and return count */
int handle_data_or_string(char line[], int index, int line_number);

/* Function to handle an .extern symbol and add it to the symbol table */
void handle_external_symbol(char line[], struct symbol_table* head, int current_ic, int line_number);

/* Function to determine if a line contains an .entry or .extern prompt */
int handle_entry_or_extern(char line[]);

/* First pass through the input file */
void first_pass_through(char* argv, struct symbol_table* symbol_head);

#endif
