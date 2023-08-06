#include "symbol_table.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/* maximum length of symbol is 31 without \n */
/* maximum length of line is 80 without \n */

int parse_macros(char* argv);
int first_pass_through(char* argv, symbol_table* symbol_head);


int main(int argc, char* argv[]){
    int i;
    for (i = 1; i < argc; i++){
        symbol_table* symbol_head;
        symbol_head = insert_symbol(NULL, "NULL");
        parse_macros(argv[i]);
        first_pass_through(argv[i], symbol_head);
        free_tables(symbol_head);
    }
    return 0;
}

