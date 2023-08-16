#include "symbol_table.h"
#include "utils.h"
#include "second_pass_through.h"
#include "first_pass_through.h"
#include "processing_helpers.h"
#include "encoding_functions.h"
#include "create_output_files.h"

/* Define a structure for a 12-bit bitfield */
struct bitfield {
    unsigned int bits: 12;
};

/* Function to get the value stored in a bitfield */
unsigned int get_bitfield_value(struct bitfield *bitfield) {
    return bitfield->bits;
}

/* Function to convert a character to a bitfield */
struct bitfield *char_to_bitfield(char c) {
    /* Allocate memory for a new bitfield */
    struct bitfield *new_bitfield;
    int i;
    new_bitfield = malloc(sizeof(struct bitfield));
    new_bitfield->bits = 0;

    /* Convert the character to a bitfield representation */
    for (i = 0; i < 12; i++) {
        new_bitfield->bits |= ((c >> i) & 1) << i;
    }

    return new_bitfield;
}

/* Function to calculate the two's complement of a number */
int twos_complement(int num) {
    if (num >= 0) {
        return num;
    } else {
        return ~(-num) + 1;
    }
}

/* Function to convert a number to a bitfield */
struct bitfield *num_to_bitfield(unsigned int num) {
    struct bitfield *new_bitfield;
    int i;
    new_bitfield = malloc(sizeof(struct bitfield));
    new_bitfield->bits = 0;
    for (i = 0; i < 12; i++) {
        new_bitfield->bits |= ((num >> i) & 1) << i;
    }
    return new_bitfield;
}

/* Function to free memory used by an array of bitfields */
void free_bitfield_array(struct bitfield *array[]) {
    int i;
    for (i = 0; i < 1024; i++) {
        if (array[i] != NULL) {
            free(array[i]);
        }
    }
}

/* Function to process the second pass through the input file */
void second_pass_through(char* argv, struct symbol_table* symbol_head) {
    FILE *input_file;
    char line[MAX_LINE_LENGTH + 1];
    int line_number;
    int index;
    int instruction_index;
    int data_index;
    int entry_or_extern;
    int data_or_string;
    char *symbol_name;
    struct bitfield *instruction_array[1024];
    struct bitfield *data_array[1024];

    /* Open the input file */
    input_file = open_file(argv, ".am");
    line_number = 0;
    instruction_index = 0;
    data_index = 0;
    symbol_name = NULL;
    memset(data_array, 0, sizeof(data_array));
    memset(instruction_array, 0, sizeof(instruction_array));

    /* Read each line of the input file */
    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)) {
        line_number++;
        index = 0;

        /* Extract symbol name from the line */
        symbol_name = extract_symbol(line);
        if (symbol_name != NULL) {
            while (isspace(line[index])) {
                index++;
            }
            while (!isspace(line[index])) {
                index++;
                if (line[index] == ':') {
                    index++;
                    break;
                }
            }
            while (isspace(line[index])) {
                index++;
            }
            free(symbol_name);
        }

        /* Classify if it's an entry or extern statement */
        entry_or_extern = classify_extern_or_entry(line, index);
        data_or_string = classify_data_or_string(line, index);

        /* Handle entry/extern or data/string statements */
        if (entry_or_extern != 0) {
            continue;
        }

        if (data_or_string != 0) {
            if (data_or_string == 1) {
                data_index = encode_string(line, index, data_array, data_index);
            } else if (data_or_string == 2) {
                data_index = encode_data(line, index, data_array, data_index, line_number);
            }
        } else {
            instruction_index = encode_instruction(line, index, instruction_array, instruction_index, line_number, symbol_head);
        }
    }
    /* If error-free, create output files */
    if (error_free) {
        create_output_files(argv, symbol_head, instruction_array, data_array, instruction_index, data_index);
    }
    else {
        printf("Cannot create output files due to errors in the input file\n\n");
    }

    /* Free memory and close the input file */
    free_bitfield_array(instruction_array);
    free_bitfield_array(data_array);
    fclose(input_file);
}
