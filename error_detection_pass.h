#ifndef ERROR_DETECTION_PASS_H
#define ERROR_DETECTION_PASS_H

#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "processing_helpers.h"
#include "instruction_handling.h"
#include "first_pass_through.h"


/* Validate symbol-related instructions */
void validate_symbol_instruction(char* line, int line_number, struct symbol_table* symbol_head);

/* Validate non-symbol instructions */
void validate_non_symbol_instruction(char* line, int line_number, struct symbol_table* symbol_head);

/* Perform error detection pass */
void error_detection_pass(char* argv, struct symbol_table* symbol_head, int IC, int DC);

#endif