#include "create_output_files.h"
#include "second_pass_through.h"
#include "symbol_table.h"

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char extern_list[MAX_MEMORY_SIZE][MAX_SYMBOL_LENGTH];

void add_to_extern_list(const char extern_name[], int instruction_index) {
    strcpy(extern_list[instruction_index], extern_name);
}

char* encode_base64(struct bitfield *bf) {
    char *encoded = (char*)malloc(3); /* Allocate memory for encoded string */
    if (encoded == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    encoded[0] = base64_table[(get_bitfield_value(bf) >> 6) & 0x3F]; /* Encode the first 6 bits */
    encoded[1] = base64_table[get_bitfield_value(bf) & 0x3F]; /* Encode the last 6 bits */
    encoded[2] = '\0'; /* Null-terminate the string */

    return encoded;
}

void create_obj_file(char argv[], struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit) {
    FILE* obj_file;
    int i;
    char *converted_to_base64;

    obj_file = create_output_file(argv, ".ob"); /* Create object file */
    if (obj_file == NULL) {
        printf("Error creating object file: %s\n", argv);
        return;
    }

    fprintf(obj_file, "%d %d\n", instruction_limit, data_limit); /* Write instruction and data limits */

    for (i = 0; i < instruction_limit; i++) {
        converted_to_base64 = encode_base64(instruction_array[i]);
        fprintf(obj_file, "%s\n", converted_to_base64); /* Write encoded instruction */
        free(converted_to_base64);
    }
    for (i = 0; i < data_limit; i++) {
        converted_to_base64 = encode_base64(data_array[i]);
        fprintf(obj_file, "%s\n", converted_to_base64); /* Write encoded data */
        free(converted_to_base64);
    }

    fclose(obj_file); /* Close object file */
    printf("Assembler finished successfully!\n");
}

void create_ent_file(char argv[], struct symbol_table *symbol_head) {
    FILE* ent_file;
    struct symbol_table *current_symbol;

    current_symbol = symbol_head;

    ent_file = create_output_file(argv, ".ent"); /* Create entry file */
    if (ent_file == NULL) {
        printf("Error creating entry file: %s\n", argv);
        return;
    }

    while (current_symbol != NULL) {
        if (get_symbol_external_or_entry(current_symbol) == 2) {
            fprintf(ent_file, "%s %d\n", get_symbol(current_symbol), get_symbol_value(current_symbol)); /* Write symbol entry */
        }
        current_symbol = get_next_symbol(current_symbol);
    }

    free(current_symbol); /* Free symbol memory */
    fclose(ent_file); /* Close entry file */
}

void create_ext_file(char argv[]) {
    FILE* ext_file;
    int i;
    int create_file;

    create_file = 0;

    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (extern_list[i][0] != '\0') {
            create_file = 1;
        }
    }
    if (create_file == 1) {
        ext_file = create_output_file(argv, ".ext"); /* Create extern file */
        if (ext_file == NULL) {
            printf("Error creating extern file: %s\n", argv);
            return;
        }
        for (i = 0; i < MAX_MEMORY_SIZE; i++) {
            if (extern_list[i] != NULL) {
                if (extern_list[i][0] != '\0') {
                    fprintf(ext_file, "%s %d\n", extern_list[i], i); /* Write external symbol */
                }
            }
        }
        fclose(ext_file); /* Close extern file */
    }
}

void create_output_files(char argv[], struct symbol_table *symbol_head, struct bitfield *instruction_array[], struct bitfield *data_array[], int instruction_limit, int data_limit) {
    create_obj_file(argv, instruction_array, data_array, instruction_limit, data_limit);
    if (contains_entry)
        create_ent_file(argv, symbol_head);
    if (contains_extern)
        create_ext_file(argv);
}
