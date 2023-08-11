
#ifndef FIRST_PASS_THROUGH_H
#define FIRST_PASS_THROUGH_H

#include "symbol_table.h"
#include "utils.h"

/* Extracts a symbol from a line */
char* extract_symbol(const char line[]);

/* Extracts the number of characters in a .string */
int handle_string(char line[], int index, int line_number);

/* Extracts the number of integers in a .data */
int handle_data(char line[], int index, int line_number);

/* Check if a line contains a .data or .string prompt, and handle them */
int handle_data_or_string(char line[], int index, int line_number);

/* Extracts an .extern symbol name and handles it */
void handle_extern_or_entry_symbol(char line[], struct symbol_table* head, int index, int extern_or_entry, int line_number);

/* Classify if a line is a .entry or .extern prompt */
int classify_extern_or_entry(char line[], int index);

/* Handle a line with a symbol declaration */
void handle_symbol(struct symbol_table *symbol_head, char line[], int index, int line_number, char symbol_name[MAX_SYMBOL_LENGTH + 1], int temp_ic, int temp_dc);

/* Handle a line without a symbol declaration */
void handle_non_symbol(struct symbol_table *symbol_head, char line[], int index, int line_number, int temp_ic, int temp_dc);

/* First pass through the input file */
void first_pass_through(char* argv, struct symbol_table* symbol_head);

#endif
