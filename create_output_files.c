
#include "create_output_files.h"

#include "second_pass_through.h"

#include "symbol_table.h"


const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* encode_base64(struct bitfield *bf) {
    char *encoded = (char*)malloc(3);
    if (encoded == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    encoded[0] = base64_table[(get_bitfield_value(bf) >> 6) & 0x3F];
    encoded[1] = base64_table[get_bitfield_value(bf) & 0x3F];
    encoded[2] = '\0';

    return encoded;
}

void create_obj_file(char argv[], struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit){
    FILE* obj_file;
    int i;
    char *converted_to_base64;

    obj_file = create_output_file(argv, ".ob");
    if (obj_file == NULL) {
        printf("Error creating object file: %s\n", argv);
        return;
    }

    fprintf(obj_file, "%d %d\n", instruction_limit, data_limit);

    for ( i = 0; i < instruction_limit; i++){
        converted_to_base64 = encode_base64(instruction_array[i]);
        fprintf(obj_file, "%s\n", converted_to_base64);
        free(converted_to_base64);
    }
    for(i = 0; i < data_limit; i++){
        converted_to_base64 = encode_base64(data_array[i]);
        fprintf(obj_file, "%s\n", converted_to_base64);
        free(converted_to_base64);
    }

    fclose(obj_file);
}

void create_output_files(char argv[], struct symbol_table *symbol_head, struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit){
    create_obj_file(argv, instruction_array, data_array, instruction_limit, data_limit);
    /*create_ent_file(argv, symbol_head);*/
}

