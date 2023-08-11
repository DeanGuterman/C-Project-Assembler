#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

extern int contains_extern;
extern int contains_entry;

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
void handle_extern_or_entry_symbol(char line[], struct symbol_table* head, int index, int extern_or_entry, int line_number){
    char symbol_name[MAX_SYMBOL_LENGTH + 1];
    int symbol_index;
    struct symbol_table* symbol;

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
    if (extern_or_entry == 1) {
        contains_extern = 1;
        if (search_symbol(head, symbol_name) == NULL) {
            set_symbol_external_or_entry(insert_symbol(head, symbol_name, 0, line_number), 1);
        }
        else set_symbol_external_or_entry(search_symbol(head, symbol_name), 1);
    }
    else if (extern_or_entry == 2){
        contains_entry = 1;
        if (search_symbol(head, symbol_name) == NULL) {
            symbol = insert_symbol(head, symbol_name, 0, line_number);
            set_symbol_external_or_entry(symbol, 2);
            set_symbol_pre_defined_entry(symbol, 1);
        }
        else {
            set_symbol_external_or_entry(search_symbol(head, symbol_name), 2);
        }
    }
}

/* Check if it's a .entry or .extern prompt, and return a corresponding int value */
int classify_extern_or_entry(char line[], int index){
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
        return 2;
    }
    if (strcmp(".extern", prompt) == 0){
        return 1;
    }

    /* Return 0 if the prompt is not recognized */
    return 0;
}