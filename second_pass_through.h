

#ifndef SECOND_PASS_THROUGH_H
#define SECOND_PASS_THROUGH_H
#include "symbol_table.h"
#include "utils.h"
#include "first_pass_through.h"
#include "processing_helpers.h"
#include "encoding_functions.h"
#include "create_output_files.h"

/* Structure for a 12-bit bitfield */
struct bitfield;

/* Get the value stored in a bitfield */
unsigned int get_bitfield_value(struct bitfield *bitfield);

/* Convert a character to a bitfield */
struct bitfield *char_to_bitfield(char c);

/* Calculate the two's complement of a number */
int twos_complement(int num);

/* Convert a number to a bitfield */
struct bitfield *num_to_bitfield(int num);

/* Free memory used by an array of bitfields */
void free_bitfield_array(struct bitfield *array[]);

/* Process the second pass through the input file */
void second_pass_through(char* argv, struct symbol_table* symbol_head);

#endif
