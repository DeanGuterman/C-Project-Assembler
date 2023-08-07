

#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"

#define TEMP_SYMBOL_NAME "@r0"
#define MAX_LINE_LENGTH 80
#define MAX_SYMBOL_LENGTH 31

struct symbol_table;

void free_tables(struct symbol_table* symbol_head);
FILE* open_file(char* file_name, char* file_type);
FILE* create_output_file(char* file_name, char* file_type);

