
#include "second_pass_through.h"
#include "symbol_table.h"
#include "first_pass_through.h"
#include "processing_helpers.h"
#include "encoding_functions.h"

struct bitfield {
    unsigned int bits: 12;
};


/* function to create a bitfield */
struct bitfield *create_bitfield(void){
    struct bitfield *new_bitfield;
    new_bitfield = malloc(sizeof(struct bitfield));
    new_bitfield->bits = 0;
    return new_bitfield;
}

struct bitfield *char_to_bitfield(char c) {
    struct bitfield *new_bitfield;
    int i;
    new_bitfield = malloc(sizeof(struct bitfield));
    new_bitfield->bits = 0;

    for (i = 0; i < 12; i++) {
        new_bitfield->bits |= ((c >> i) & 1) << i;
    }

    return new_bitfield;
}

void print_binary(unsigned int value) {
    int i;

    for (i = 11; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void second_pass_through(char* argv, struct symbol_table* symbol_head){
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

    input_file = open_file(argv, ".am");
    line_number = 0;
    index = 0;
    instruction_index = 0;
    data_index = 0;
    symbol_name = NULL;


    /* Go through every line in the file */
    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)){
        line_number++;
        index = 0;

        symbol_name = extract_symbol(line);
        if (symbol_name != NULL){
            while(isspace(line[index])){
                index++;
            }
            while(!isspace(line[index])){
                index++;
            }
            while(isspace(line[index])){
                index++;
            }
        }

        entry_or_extern = classify_extern_or_entry(line, index);
        data_or_string = classify_data_or_string(line, index);

        if (entry_or_extern != 0){
            continue;
        }

        if (data_or_string != 0){
            if (data_or_string == 1){
                data_index = encode_string(line, index, data_array, data_index);
            }
            else if (data_or_string == 2){
                /*instruction_index = encode_data*/
            }
        }
        else{
            /*instruction_index = encode_instruction*/
        }
        free (symbol_name);
    }


    fclose(input_file);

}
