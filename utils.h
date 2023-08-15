#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define TEMP_SYMBOL_NAME "mov"
#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_LENGTH 31
#define MAX_MEMORY_SIZE 1024
#define RESERVED_NAMES {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "data", "string","entry", "extern", "r1", "r2", "r3", "r4","r5", "r6", "r7", "r0"}
#define INSTRUCTION_NAMES {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"}
#define REGISTER_NAMES {"@r0", "@r1", "@r2", "@r3", "@r4","@r5", "@r6", "@r7"}

struct symbol_table;
extern int error_free;
extern int contains_extern;
extern int contains_entry;
extern char* reserved_names[];
extern char* instruction_names[];
extern char* register_names[];

/* Free symbol table */
void free_tables(struct symbol_table* symbol_head);

int get_instruction_decimal_code(char* instruction_name);

/* Open file for reading */
FILE* open_file(char* file_name, char* file_type);

/* Create output file for writing */
FILE* create_output_file(char* file_name, char* file_type);

#endif
