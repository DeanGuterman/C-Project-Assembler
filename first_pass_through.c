#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbol_table.h"
#include "utils.h"

extern int contains_extern;
extern int error_free;

char* extract_symbol(const char line[]) {
    int index;
    int symbol_index;
    char* symbol_name;
    int i;
    index = 0;
    symbol_index = 0;

    /* Skip leading whitespace */
    while (isspace(line[index]))
        index++;

    /* Calculate the length of the symbol name */
    while (!isspace(line[index]) && line[index] != '\n' && line[index] != ':') {
        symbol_index++;
        index++;
    }

    /* Allocate memory for the symbol name */
    symbol_name = (char*)malloc((symbol_index + 1) * sizeof(char));
    if (symbol_name == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    /* Copy the symbol name into the allocated memory */
    index -= symbol_index;
    for (i = 0; i < symbol_index; i++) {
        symbol_name[i] = line[index];
        index++;
    }
    symbol_name[symbol_index] = '\0'; /* Null-terminate the symbol name */

    /* Check if it's a valid symbol declaration */
    if (line[index] != ':') {
        free(symbol_name); /* Free the memory before returning NULL */
        return NULL; /* No symbol declaration found */
    }

    return symbol_name;
}

/* Extracts the number of chars in a .string */
int handle_string(char line[], int index, int line_number){
    int char_counter;
    char_counter = 1;

    /* Skip leading whitespace characters */
    while(isspace(line[index])){
        index++;
    }

    /* Check if the .string prompt starts with a double quote */
    if (line[index] != '\"'){
        printf("Error: missing starting quotation marks in .string at line %d\n",line_number);
        return 0;
    }

    /* If the .string prompt starts with a double quote, proceed */
    if (line[index] == 34){
        index++;
        while (line[index] != '\"'){
            /* Check it's a printable character */
            if (line[index] < 32 || line[index] > 126){
                printf("Error: non-printable character in .string at line %d\n", line_number);
                return 0;
            }
            /* Check for missing ending quotation marks within the .string */
            if (line[index] == '\n'){
                printf("Error: missing ending quotation marks in .string at line %d\n",line_number);
                return 0;
            }
            char_counter++;
            index++;
        }
    }

    /* Return the total number of characters in the .string content, excluding quotes */
    return char_counter;
}

/* Extracts the number of integers in a .data */
int handle_data(char line[], int index, int line_number){
    int data_counter;
    int comma_flag;
    int plus_minus_flag;
    data_counter = 1;
    comma_flag = 1;
    plus_minus_flag = 0;

    /* Count the number of integers (data items) in the .data prompt */
    while(line[index] != '\0'){
        if(line[index] == 44){
            /* Check for unnecessary commas */
            if (comma_flag == 1){
                printf("Error: unnecessary commas in .data at line %d\n", line_number);
                return 0;
            }
            else {
                comma_flag = 1;
                plus_minus_flag = 0;
                data_counter++;
            }
        }
        else if (line[index] == 43 || line[index] == 45){
            /* Check for multiple consecutive plus or minus signs */
            if (plus_minus_flag == 1){
                printf("Error: multiple consecutive plus or minus signs in .data at line %d\n", line_number);
                return 0;
            }
            else {
                plus_minus_flag = 1;
            }
        }
        else if (isdigit(line[index]) ){
            comma_flag = 0;
        }
        index++;
    }

    /* Return the total number of integers (data items) found in the .data prompt */
    return data_counter;
}

/* Check if a given line contains a .data or .string prompt, and handle them */
int handle_data_or_string(char line[], int index, int line_number){
    int prompt_index;
    char prompt[MAX_LINE_LENGTH + 1];
    int handle_string_value;
    int handle_data_value;

    prompt_index = 0;
    /* If the line starts with a newline, return 0 indicating no prompt */
    if (line[index] == '\n'){
        return 0;
    }

    memset(prompt, '\0', sizeof(prompt));

    /* Skip leading whitespace characters */
    while(isspace(line[index])){
        index++;
    }


    /* Extract the prompt */
    while(!isspace(line[index]) && line[index] != '\n'){
        prompt[prompt_index] = line[index];
        index++;
        prompt_index++;
    }

    /* Compare the prompt with ".string" and ".data" */
    if (strcmp(".string", prompt) == 0){
        handle_string_value = handle_string(line, index, line_number);
        if (handle_string_value == 0){
            return -1;
        }
        return handle_string_value;
    }
    if (strcmp(".data", prompt) == 0){
        handle_data_value = handle_data(line, index, line_number);
        if (handle_data_value == 0){
            return -1;
        }
        return handle_data_value;
    }

    /* Return 0 if the prompt is not recognized */
    return 0;
}

/* extract a .extern symbol name and handle it */
void handle_external_symbol(char line[], struct symbol_table* head,int current_ic, int line_number, int index){
    char symbol_name[MAX_SYMBOL_LENGTH + 1];
    int symbol_index;

    symbol_index = 0;
    memset(symbol_name, '\0', sizeof(symbol_name));

    /* Skip leading .extern call and whitespace characters */
    while (isspace(line[index])){
        index++;
    }
    while(!isspace(line[index]) && line[index] != '\n'){
        index++;
    }
    while(isspace(line[index])){
        index++;
    }
    /* Extract the symbol name */
    while(!isspace(line[index]) && line[index] != '\n'){
        symbol_name[symbol_index] = line[index];
        index++;
        symbol_index++;
    }
    insert_symbol(head, symbol_name,current_ic ,line_number);
    contains_extern = 1;
}

/* Check if it's a .entry or .extern prompt, and return a corresponding int value */
int handle_entry_or_extern(char line[], int index){
    int prompt_index;
    char prompt[MAX_LINE_LENGTH + 1];

    prompt_index = 0;
    memset(prompt, '\0', sizeof(prompt));

    /* Skip leading whitespace characters */
    while(isspace(line[index])){
        index++;
    }

    /* Extract the prompt */
    while(!isspace(line[index]) && line[index] != '\n'){
        prompt[prompt_index] = line[index];
        index++;
        prompt_index++;
    }

    /* Compare the prompt with ".entry" and ".extern" */
    if (strcmp(".entry", prompt) == 0){
        return 1;
    }
    if (strcmp(".extern", prompt) == 0){
        return 2;
    }

    /* Return 0 if the prompt is not recognized */
    return 0;
}

void first_pass_through(char* argv, struct symbol_table* symbol_head) {
    char line[MAX_LINE_LENGTH + 1];
    FILE *input_file;
    int temp_dc, temp_ic;
    char *symbol_name;
    struct symbol_table *new_symbol;
    int line_number;
    int index;
    int data_or_string_value;
    int entry_or_extern_value;

    symbol_name = NULL;
    temp_dc = 0;
    temp_ic = 100;
    line_number = 0;
    input_file = open_file(argv, ".am");

    /* Go through every line in the file */
    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)) {
        line_number++;
        index = 0;
        /* Check if it's an .entry or .extern prompt */
        entry_or_extern_value = handle_entry_or_extern(line, index);
        /* Check if it's a .data or .string prompt */
        data_or_string_value = handle_data_or_string(line, index, line_number);
        /* Check if it's a symbol declaration */
        symbol_name = extract_symbol(line);

        if (symbol_name != NULL) {
            new_symbol = insert_symbol(symbol_head, symbol_name, temp_ic, line_number);
            if (symbol_head == NULL){
                symbol_head = new_symbol;
            }
            /* Skip the symbol name */
            while(isspace(line[index])){
                index++;
            }
            while(!isspace(line[index])){
                index++;
            }

            /* Check if the label is a .data or .string prompt */
            data_or_string_value = handle_data_or_string(line, index, line_number);

            /* Check if the label is a .entry or .extern prompt */
            entry_or_extern_value = handle_entry_or_extern(line, index);

            if (data_or_string_value == -1){ /* If there was an error in the .data or .string prompt */
                error_free = 0;
                continue;
            }
            else if (data_or_string_value > 0){ /* If it's a .data or .string prompt */
                temp_dc += data_or_string_value;
                temp_ic += data_or_string_value;
            }
            else if(entry_or_extern_value == 1){ /* If it's an .entry prompt */
                /*
                 * CODE THAT DOES SOMETHING IF ITS AN ENTRY
                 */
            }
            else if(entry_or_extern_value == 2){ /* If it's an .extern prompt */
                handle_external_symbol(line, symbol_head,temp_ic, line_number, index);
                symbol_head = delete_symbol(symbol_head, symbol_name); /* Since we have re-added the symbol as an external symbol, we can delete the original from the symbol table */
                /*new_symbol->is_external = 1; DO THIS SOMEHOW IMPORTANTO!!!!!!!!!!!!!!!!!*/
            }
                /*
                 * MIGHT HAVE TO HANDLE LABELS THAT ARE .entry or .extern HERE, UNSURE BUT THIS IS IMPORTANT!!!!!
                 */
            else if(data_or_string_value == 0){ /* If it's not a .data or .string prompt */
                /*
                 * SOME CODE GOES HERE, SOMETHING ABOUT CHECKING OP CODE AND STUFF
                 */
            }
            free(symbol_name);
        }

        else if (data_or_string_value != 0) {
            if (data_or_string_value == -1) { /* If there was an error in the .data or .string prompt */
                error_free = 0;
                continue;
            } else if (data_or_string_value > 0) { /* If it's a .data or .string prompt */
                temp_dc += data_or_string_value;
                temp_ic += data_or_string_value;
            }
        }
        else if (entry_or_extern_value == 1) { /* If it's a .entry prompt */
                /*
                 * CODE THAT DOES SOMETHING IF ITS AN ENTRY
                 */
        } else if (entry_or_extern_value == 2) { /* If it's a .extern prompt */
                handle_external_symbol(line, symbol_head,temp_ic, line_number, index);
        }
        else{ /* If it's an instruction */
            /*
             * SOME CODE GOES HERE, SOMETHING ABOUT CHECKING OP CODE AND STUFF
             */
        }

    }
    fclose(input_file);
}
