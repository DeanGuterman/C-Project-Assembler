
#ifndef PARSE_MACROS_H
#define PARSE_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

typedef struct Macro {
    char name[MAX_LINE_LENGTH];
    char* content;
    struct Macro* next;
} Macro;

extern int error_free;

int check_line_length(char line[]);
int check_legal_macro_name(char name[], int line_number);
Macro* handle_macro_start(char* line, Macro* macro_tail);
void append_to_macro(Macro* current_macro, char* line);
int handle_macro_call(char* line, Macro* macro_tail, FILE* output_file);
void parse_macros(char* argv);

#endif
