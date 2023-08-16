#include <stdio.h>
#include "symbol_table.h"
#include "utils.h"
#include "parse_macros.h"
#include "first_pass_through.h"
#include "error_detection_pass.h"
#include "second_pass_through.h"

/*
 * Coded by Dean Guterman for the Open University
 *
 */



/* Global variables */
int contains_extern;
int contains_entry;
int error_free;

/* Main function */
int main(int argc, char* argv[]) {
    int i;

    /* Check if input files are provided */
    if (argc < 2) {
        printf("Error: no files were given to the assembler\n");
        return 0;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) {
        struct symbol_table* symbol_head;
        int IC, DC;

        printf("Processing file %s\n", argv[i]);

        /* Initialize variables for this file */
        IC = 0;
        DC = 0;
        contains_extern = 0;
        contains_entry = 0;
        error_free = 1;

        /* Create the symbol table with a temporary symbol */
        symbol_head = insert_symbol(NULL, TEMP_SYMBOL_NAME, -1, -1);

        /* Parse macros from the input file */
        parse_macros(argv[i]);

        /* If no errors in macro parsing, proceed with the first pass */
        if (error_free == 1) {
            first_pass_through(argv[i], symbol_head, &IC, &DC);

            /* Perform error detection pass */
            error_detection_pass(argv[i], symbol_head, IC, DC);

            /* Perform second pass */
            second_pass_through(argv[i], symbol_head);
        }

        /* Free memory used by the symbol table */
        free_tables(symbol_head);
    }
    return 0;
}
