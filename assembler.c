#include "symbol_table.h"
#include "utils.h"
#include "parse_macros.h"
#include "first_pass_through.h"
#include <stdio.h>
/* system size is 1024, so ic+dc <= 924 as 100 first ones are saved */



int main(int argc, char* argv[]){
    int i;

    for (i = 1; i < argc; i++){
        struct symbol_table* symbol_head;
        extern int contains_extern;
        extern int error_free;

        contains_extern = 0;
        error_free = 1;
        symbol_head = insert_symbol(NULL, TEMP_SYMBOL_NAME, -1, -1);
        parse_macros(argv[i]);
        if (error_free == 1) {
            first_pass_through(argv[i], symbol_head);
        }

        free_tables(symbol_head);
    }
    return 0;
}

