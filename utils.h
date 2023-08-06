#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"


void free_tables(symbol_table* symbol_head);
FILE* open_file(char* file_name, char* file_type);
FILE* create_output_file(char* file_name, char* file_type);

