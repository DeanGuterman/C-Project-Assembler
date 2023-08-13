
#include "encoding_functions.h"
#include "second_pass_through.h"





int encode_string(const char line[], int index, struct bitfield *data_array[], int data_index){
    while (line[index] != '"'){
        index++;
    }

    index++;
    while(line[index] != '"'){
        data_array[data_index] = char_to_bitfield(line[index]);
        index++;
        data_index++;
    }
    data_array[data_index] = char_to_bitfield('\0');
    data_index++;
    return data_index;
}
