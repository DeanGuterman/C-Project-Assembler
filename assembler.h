
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "symbol_table.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "second_pass_through.h"


/* Function declarations */
void parse_macros(char* argv);
void first_pass_through(char* argv, struct symbol_table* symbol_head, int *IC, int *DC);

#endif
