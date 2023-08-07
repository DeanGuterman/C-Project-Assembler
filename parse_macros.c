#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"


/* Structure to hold macro information */
typedef struct Macro {
    char name[MAX_LINE_LENGTH];
    char* content;
    struct Macro* next;
} Macro;

int check_line_length(char line[]){
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (i >= MAX_LINE_LENGTH - 1) {
            return 0;
        }
    }
    printf("i = %d\n", i);
    return 1;
}

/* Handle the start of a new macro */
Macro* handle_macro_start(char* line, Macro* macro_tail) {
    /* Create a new macro and add it to the linked list */
    Macro* new_macro;
    int len;
    int i;
    new_macro = (Macro*)malloc(sizeof(Macro));
    strncpy(new_macro->name, line + 5, sizeof(new_macro->name) - 1);
    new_macro->name[sizeof(new_macro->name) - 1] = '\0';

    /* Remove trailing whitespace from macro name */
    len = strlen(new_macro->name);
    i = len - 1;
    while (i >= 0 && (new_macro->name[i] == ' ' || new_macro->name[i] == '\t')) {
        new_macro->name[i] = '\0';
        i--;
    }
    new_macro->content = NULL; /* Initialize content pointer */
    new_macro->next = NULL;

    if (macro_tail != NULL) {
        new_macro->next = macro_tail;
    }

    return new_macro;
}

/* Append the given line to the content of the current macro */
void append_to_macro(Macro* current_macro, char* line) {
    if (current_macro->content == NULL) {
        size_t trimmed_len = strlen(line);
        current_macro->content = (char*)malloc(trimmed_len + 1);
        if (current_macro->content != NULL) {
            strcpy(current_macro->content, line);
        }
    } else {
        size_t content_len = strlen(current_macro->content);
        size_t line_len = strlen(line);
        char* new_content = (char*)malloc(content_len + line_len + 2); /* +2 for '\n' and '\0' */
        strcpy(new_content, current_macro->content);
        strcat(new_content, "\n");
        strcat(new_content, line);
        free(current_macro->content);
        current_macro->content = new_content;
    }
}

/* Handle a macro call in the given line */
int handle_macro_call(char* line, Macro* macro_tail, FILE* output_file) {
    int macro_call_found;
    char* line_copy;
    char* token;
    line_copy = (char*)malloc(strlen(line) + 1);
    macro_call_found = 0;
    strcpy(line_copy, line);
    token = strtok(line_copy, " ");
    while (token != NULL) {
        /* Check if the token matches a macro name */
        Macro* current;
        current = macro_tail;
        while (current != NULL) {
            if (strcmp(current->name, token) == 0) {
                macro_call_found = 1;
                fprintf(output_file, "%s\n", current->content);
                break;
            }
            current = current->next;
        }
        token = strtok(NULL, " ");
    }
    free(line_copy);
    return macro_call_found;
}

/* Parse macros from .as file to .am file */
int parse_macros(char* argv) {
    FILE* input_file;
    FILE* output_file;
    char line[MAX_LINE_LENGTH + 1];
    char* trimmed_line;
    Macro* current_macro;
    int inside_macro;
    Macro* macro_tail;
    Macro* temp;
    int line_number;

    current_macro = NULL;
    inside_macro = 0;
    macro_tail = NULL;
    input_file = open_file(argv, ".as");
    line_number = 0;

    printf("Parsing macros from file: %s.as\n", argv);
    if (input_file == NULL) {
        printf("Error opening file: %s\n", argv);
        return;
    }

    output_file = create_output_file(argv, ".am");
    if (output_file == NULL) {
        printf("Error creating output file: %s\n", argv);
        fclose(input_file);
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH + 1, input_file)) {
        line_number++;

        /* Check if line length exceeds maximum line length */
        if (check_line_length(line) == 0){
            printf("Error: line %d length exceeds maximum line length of %d characters\n",line_number, MAX_LINE_LENGTH - 1);
            return 0;
        }
        /* Trim trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        /* Trim leading whitespace */
        trimmed_line = line;
        while (*trimmed_line == ' ' || *trimmed_line == '\t') {
            ++trimmed_line;
        }

        if (strlen(line) == 0 || line[0] == ';') {
            continue; /* Ignore empty lines and comments */
        }

        /* Check if it's a macro declaration */
        if (strncmp(trimmed_line, "mcro", 4) == 0) {
            current_macro = handle_macro_start(trimmed_line, macro_tail);
            if (macro_tail == NULL) {
                macro_tail = current_macro;
            }
            inside_macro = 1;
        }

        else if (strncmp(trimmed_line, "endmcro", 7) == 0) {
            /* Reached the end of a macro definition */
            inside_macro = 0;
        }

        else if (inside_macro) {
            append_to_macro(current_macro, trimmed_line);
        }

        else {
            if (handle_macro_call(trimmed_line, macro_tail, output_file) == 0) {
                /* Write the line to the output file */
                fprintf(output_file, "%s\n", trimmed_line);
            }
        }
    }

    /* Clean up: free the macro linked list and associated content */
    while (current_macro != NULL) {
        temp = current_macro;
        current_macro = current_macro->next;
        printf("Freeing macro: %s\n", temp->name);
        free(temp->content); /* Free allocated content memory */
        free(temp);
    }

    /* Close file handles */
    fclose(input_file);
    fclose(output_file);

    printf("Macros parsed successfully! Output file: %s.am\n", argv);
    return 1;
}
