
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "symbol_table.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Function declarations */
int parse_macros(char* argv);
int first_pass_through(char* argv, symbol_table* symbol_head);

#endif
