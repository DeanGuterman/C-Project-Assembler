
#ifndef ASSEMBLER_PROJECT_PROCESSING_HELPERS_H
#define ASSEMBLER_PROJECT_PROCESSING_HELPERS_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

/* Extracts the number of chars in a .string */
int handle_string(char line[], int index, int line_number, int check_errors);

/* Extracts the number of integers in a .data */
int handle_data(char line[], int index, int line_number, int check_errors);

/* Check if a given line contains a .data or .string prompt, and handle them */
int handle_data_or_string(char line[], int index, int line_number, int check_errors);

/* Extracts a .extern symbol name and handles it */
void handle_extern_or_entry_symbol(char line[], struct symbol_table* head, int index, int extern_or_entry, int line_number);

/* Check if it's a .entry or .extern prompt, and return a corresponding int value */
int classify_extern_or_entry(char line[], int index);

#endif
