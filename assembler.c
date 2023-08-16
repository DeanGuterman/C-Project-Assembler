
#include <stdio.h>
#include "symbol_table.h"
#include "utils.h"
#include "parse_macros.h"
#include "first_pass_through.h"
#include "error_detection_pass.h"
#include "second_pass_through.h"
#include "assembler.h"



int contains_extern;
int contains_entry;
int error_free;

int main(int argc, char* argv[]){
    int i;

    if(argc < 2){
        printf("Error: no files were given to the assembler\n");
        return 0;
    }

    for (i = 1; i < argc; i++){
        struct symbol_table* symbol_head;
        int IC, DC;


        IC = 0;
        DC = 0;
        contains_extern = 0;
        contains_entry = 0;
        error_free = 1;

        symbol_head = insert_symbol(NULL, TEMP_SYMBOL_NAME, -1, -1);
        parse_macros(argv[i]);
        if (error_free == 1) {
            first_pass_through(argv[i], symbol_head, &IC, &DC);
        }
        error_detection_pass(argv[i], symbol_head, IC, DC);
        second_pass_through(argv[i], symbol_head);
        free_tables(symbol_head);
    }
    return 0;
}

