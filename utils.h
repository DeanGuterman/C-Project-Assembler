#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "second_pass_through.h"

#define TEMP_SYMBOL_NAME "mov"
#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_LENGTH 31
#define MAX_MEMORY_SIZE 1024
#define RESERVED_NAMES {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "data", "string","entry", "extern", "r1", "r2", "r3", "r4","r5", "r6", "r7", "r0"}
#define INSTRUCTION_NAMES {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"}
#define REGISTER_NAMES {"@r0", "@r1", "@r2", "@r3", "@r4","@r5", "@r6", "@r7"}

struct symbol_table;

/* Free symbol table */
void free_tables(struct symbol_table* symbol_head);


/* Open file for reading */
FILE* open_file(char* file_name, char* file_type);

/* Create output file for writing */
FILE* create_output_file(char* file_name, char* file_type);

#endif
