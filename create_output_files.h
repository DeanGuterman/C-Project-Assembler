#ifndef CREATE_OUTPUT_FILES_H
#define CREATE_OUTPUT_FILES_H

#include "symbol_table.h"
#include "second_pass_through.h"

struct bitfield;

/* Add an external symbol to the list */
void add_to_extern_list(const char extern_name[], int instruction_index);

/* Encode a bitfield using Base64 encoding */
char* encode_base64(struct bitfield *bf);

/* Create the object file */
void create_obj_file(char argv[], struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit);

/* Create the entry file */
void create_ent_file(char argv[], struct symbol_table *symbol_head);

/* Create the external file */
void create_ext_file(char argv[]);

/* Create all output files */
void create_output_files(char argv[], struct symbol_table *symbol_head, struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit);

#endif
