#include "symbol_table.h"
#include "utils.h"
#include "parse_macros.h"
#include "first_pass_through.h"
#include "error_detection_pass.h"
#include "second_pass_through.h"
#include "assembler.h"
#include <stdio.h>
/* system size is 1024, so ic+dc <= 924 as 100 first ones are saved */

/*
 * CURRENT TO DO
 * FOLLOW THEIR ALGORITHM
 * IF THE FOUND SYMBOL IS CODE THE VALUE IS IC
 * ADD FUNCTION TO CHECK FOR INSTRUCTION NAME (IF IT EXISTS AND STUFF)
 * ADD FUNCTION TO CHECK FOR INSTRUCTION VARIABLES LEGALITY
 * MAYBE ADD FUNCTION TO SEPERATE THE INSTRUCTION LINE TO COMMAND AND VARIABLES?
 * FIRST ONE CAN BE ALWAYS DESTINATION, AND IF A SECOND COMES IT BECOMES DESTINATION AND FIRST BECOME SOURCE
 * ADD THE 2ND PASS-THROUGH
 * CURRENT MISSIONS IS TO FINISH WITH NUMBER REALIZING AND FILTERING THROUGH INSTRUCITON HANDLING
 * ONLY SAVE DATA OF NUMBERS LOWER THAN 512? OK MAYBE SAVE EVERYONE AT 12 SIVIOT AND ONLY CODE IF BETWEEN -512 AND 512???
 * FIRST PASS CANNOT MAKE SURE SYMBOLS IN INSTRUCTIONS EXIST, DO IT IN SECOND PASS
 *
 *
 * CHECK IF NUMBERS ARE VALID ONLY AT SECOND PASS
 * MIUN MIYADI IS 10BIT, .DATA IS 12BIT
 * JUST START DOING 2ND PASS
 *
 * CURRENT TO DO IS JUST TO DO WORDS WITH 2 OPERANDS AND THEN PRINT EVERYTHING (ALSO SOMETHING ABOUT CHECKING IF ENTRYS WERE USED)
 */



int main(int argc, char* argv[]){
    int i;

    for (i = 1; i < argc; i++){
        struct symbol_table* symbol_head;
        extern int contains_extern;
        int IC, DC;


        IC = 0;
        DC = 0;
        contains_extern = 0;
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

