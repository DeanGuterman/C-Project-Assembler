#include "symbol_table.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/* system size is 1024, so ic+dc <= 924 as 100 first ones are saved */


int parse_macros(char* argv);
int first_pass_through(char* argv, symbol_table* symbol_head);


int main(int argc, char* argv[]){
    int i;

    for (i = 1; i < argc; i++){
        int error_free;
        symbol_table* symbol_head;

        symbol_head = insert_symbol(NULL, TEMP_SYMBOL_NAME, -1);

        error_free = parse_macros(argv[i]);
        if (error_free == 1) {
            error_free = first_pass_through(argv[i], symbol_head);
        }

        free_tables(symbol_head);
    }
    return 0;
}

