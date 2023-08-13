
#include "second_pass_through.h"
#include "symbol_table.h"
#include "first_pass_through.h"
#include "processing_helpers.h"

struct bitfield {
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
};

void second_pass_through(char* argv, struct symbol_table* symbol_head, struct bitfield *IC_array[], struct bitfield *DC_array[]){
    FILE *input_file;
    char line[MAX_LINE_LENGTH + 1];
    int line_number;
    int index;
    int IC_index;
    int DC_index;
    int entry_or_extern;
    int data_or_string;

    input_file = open_file(argv, ".am");
    line_number = 0;
    index = 0;
    IC_index = 0;
    DC_index = 0;

    /* Go through every line in the file */
    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)){
        line_number++;
        index = 0;
        if (extract_symbol(line) != NULL){
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
        printf("line: %s entry_or_extern: %d data_or_string: %d\n", line, entry_or_extern, data_or_string);

        if (entry_or_extern != 0){
            continue;
        }

        if (data_or_string != 0){
            if (data_or_string == 1){

            }
            else if (data_or_string == 2){

            }
        }
    }


    fclose(input_file);
}
