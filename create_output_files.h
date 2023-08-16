

#ifndef CREATE_OUTPUT_FILES_H
#define CREATE_OUTPUT_FILES_H
#include "second_pass_through.h"
#include "symbol_table.h"

struct bitfield;

char* encode_base64(struct bitfield *bf);
void add_to_extern_list(const char extern_name[], int instruction_index);
void create_obj_file(char argv[], struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit);
void create_output_files(char argv[], struct symbol_table *symbol_head, struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit);
#endif
