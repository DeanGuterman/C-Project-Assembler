

#ifndef ASSEMBLER_PROJECT_CREATE_OBJ_FILES_H
#define ASSEMBLER_PROJECT_CREATE_OBJ_FILES_H
#include "second_pass_through.h"
#include "symbol_table.h"

char* encode_base64(struct bitfield *bf);
void create_obj_file(char argv[], struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit);
void create_output_files(char argv[], struct symbol_table *symbol_head, struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit);
#endif
