
#include "second_pass_through.h"
#include "symbol_table.h"
#include "first_pass_through.h"
#include "processing_helpers.h"
#include "encoding_functions.h"

typedef struct bitfield {
    unsigned int bit_0 : 1;
    unsigned int bit_1 : 1;
    unsigned int bit_2 : 1;
    unsigned int bit_3 : 1;
    unsigned int bit_4 : 1;
    unsigned int bit_5 : 1;
    unsigned int bit_6 : 1;
    unsigned int bit_7 : 1;
    unsigned int bit_8 : 1;
    unsigned int bit_9 : 1;
    unsigned int bit_10 : 1;
    unsigned int bit_11 : 1;
} bitfield;

void print_bitfield(struct bitfield *bitfield){
    printf("%d", bitfield->bit_0);
    printf("%d", bitfield->bit_1);
    printf("%d", bitfield->bit_2);
    printf("%d", bitfield->bit_3);
    printf("%d", bitfield->bit_4);
    printf("%d", bitfield->bit_5);
    printf("%d", bitfield->bit_6);
    printf("%d", bitfield->bit_7);
    printf("%d", bitfield->bit_8);
    printf("%d", bitfield->bit_9);
    printf("%d", bitfield->bit_10);
    printf("%d", bitfield->bit_11);
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
