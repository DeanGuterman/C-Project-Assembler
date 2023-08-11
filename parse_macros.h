
#ifndef PARSE_MACROS_H
#define PARSE_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

struct macro_table;

/* Check if line length exceeds the maximum allowed */
int check_line_length(char line[]);

/* Handle the start of a new macro */
struct macro_table* handle_macro_start(char* line, struct macro_table* macro_tail);

/* Check if a macro name is legal */
int check_legal_macro_name(char name[], int line_number);

/* Check if a macro name already exists */
int check_if_name_exists(char name[], struct macro_table* macro_tail, int line_number);

/* Append the given line to the content of the current macro */
void append_to_macro(struct macro_table* current_macro, char* line);

/* Handle a macro call in the given line */
int handle_macro_call(char* line, struct macro_table* current_macro, FILE* output_file, int line_number);

/* Parse macros from .as file to .am file */
void parse_macros(char* argv);

#endif
