
#include <ctype.h>
#include "encoding_functions.h"
#include "second_pass_through.h"
#include "utils.h"
#include "instruction_handling.h"


int is_valid_num(int num){
    if (num > 2047 || num < -2048){
        return 0;
    }
    return 1;
}

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

int encode_data(const char line[], int index, struct bitfield *data_array[], int data_index, int line_number){
    int modifier;
    int num;
    while (line[index] != '\n') {
        num = 0;
        while (isspace(line[index])) {
            index++;
        }

        if (line[index] == '-') {
            modifier = -1;
            index++;
        }
        else if(line[index] == '+'){
            modifier = 1;
            index++;
        }
        else{
            modifier = 1;
        }


        while (line[index] != '\n' && line[index] != ','){
            if (isdigit(line[index])){
                num = num * 10 + (line[index] - '0');
            }
            index++;
        }

        if (line[index] == ','){
            if (is_valid_num(num)) {
                if (modifier == -1){
                    num = twos_complement(num);
                }
                data_array[data_index] = num_to_bitfield(num);
                data_index++;
            }
            else{
                printf("Error: invalid number in .data instruction at line %d\n", line_number);
                error_free = 0;
                return data_index;
            }
            index++;
        }
    }

    return data_index;
}