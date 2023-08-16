#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "parse_macros.h"


/* Structure to hold macro information */
typedef struct macro_table {
    char name[MAX_LINE_LENGTH];
    char* content;
    struct macro_table* next;
} macro_table;

int check_line_length(const char line[]){
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (i >= MAX_LINE_LENGTH - 1) {
            return 0;
        }
    }
    return 1;
}

/* Handle the start of a new macro */
macro_table* handle_macro_start(char* line, macro_table* macro_tail) {
    /* Create a new macro and add it to the linked list */
    macro_table* new_macro;
    size_t len;
    size_t i;
    new_macro = (macro_table*)malloc(sizeof(macro_table));
    strncpy(new_macro->name, line + 5, sizeof(new_macro->name) - 1);
    new_macro->name[sizeof(new_macro->name) - 1] = '\0';

    /* Remove trailing whitespace from macro name */
    len = strlen(new_macro->name);
    i = len - 1;
    while (i  - 1 >= -1 && isspace(new_macro->name[i])) {
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

int check_legal_macro_name(char name[], int line_number){
    int i;
    for ( i = 0; i < 28; i++) {
        if (strcmp(name, reserved_names[i]) == 0) {
            printf("Error in line %d illegal macro name: %s is a reserved name\n", line_number, name);
            return 0;
        }
    }
    return 1;
}

int check_if_name_exists(char name[], macro_table* macro_tail, int line_number){
    macro_table* current_macro;
    current_macro = macro_tail;
    while (current_macro != NULL) {
        if (strcmp(name, current_macro->name) == 0) {
            printf("Error in line %d macro name already exists: %s\n", line_number, name);
            return 0;
        }
        current_macro = current_macro->next;
    }
    return 1;
}

/* Append the given line to the content of the current macro */
void append_to_macro(macro_table* current_macro, char* line) {
    if (current_macro->content == NULL) { /* If the macro's content is empty, allocate memory for the new content */
        size_t trimmed_len = strlen(line);
        current_macro->content = (char*)malloc(trimmed_len + 1);
        if (current_macro->content != NULL) {
            strcpy(current_macro->content, line);
        }
    } else { /* If the macro already has content, update and format the content */
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
int handle_macro_call(char* line, macro_table* current_macro, FILE* output_file, int line_number) {
    int macro_call_found;
    char* line_copy;
    char* token;

    macro_call_found = 0;
    line_copy = (char*)malloc(strlen(line) + 1);
    strcpy(line_copy, line);
    token = strtok(line_copy, " ");

    while (token != NULL) {
        /* Check if the token matches a macro name */
        macro_table* current = current_macro;
        while (current != NULL) {
            if (strcmp(current->name, token) == 0) {
                macro_call_found = 1;
                fprintf(output_file, "%s\n", current->content);
                break;
            }
            current = current->next;
        }

        token = strtok(NULL, " ");

        if (macro_call_found && token != NULL) {
            fprintf(stderr, "Error: macro_table call in line %d is not the sole line statement\n", line_number);
            error_free = 0;
            break;
        }
    }

    free(line_copy);
    return macro_call_found;
}


/* Parse macros from .as file to .am file */
void parse_macros(char* argv) {
    FILE* input_file;
    FILE* output_file;
    char line[MAX_LINE_LENGTH + 1];
    char* trimmed_line;
    macro_table* current_macro;
    int inside_macro;
    macro_table* macro_tail;
    macro_table* temp;
    int line_number;


    current_macro = NULL;
    inside_macro = 0;
    macro_tail = NULL;
    input_file = open_file(argv, ".as");
    line_number = 0;

    if (input_file == NULL) {
        printf("Error opening file: %s\n", argv);
        error_free = 0;
        return;
    }

    output_file = create_output_file(argv, ".am");
    if (output_file == NULL) {
        printf("Error creating .am file: %s\n", argv);
        fclose(input_file);
        error_free = 0;
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH + 1, input_file) != NULL) {
        line_number++;

        /* Check if line length exceeds maximum line length */
        if (check_line_length(line) == 0){
            printf("Error: line %d length exceeds maximum line length of %d characters\n",line_number, MAX_LINE_LENGTH - 1);
            error_free = 0;
            continue;
        }
        /* Trim trailing newline character */
        line[strcspn(line, "\r\n")] = '\0';

        /* Trim leading whitespace */
        trimmed_line = line;
        while (isspace(*trimmed_line)) {
            trimmed_line++;
        }

        if (strlen(line) == 0 || line[0] == ';') {
            continue; /* Ignore empty lines and comments */
        }

        /* Check if it's a macro declaration */
        if (strncmp(trimmed_line, "mcro", 4) == 0) {
            current_macro = handle_macro_start(trimmed_line, macro_tail);
            if (check_legal_macro_name(current_macro->name, line_number) == 0) {
                error_free = 0;
                continue;
            }
            if (check_if_name_exists(current_macro->name, macro_tail, line_number) == 0) {
                error_free = 0;
                continue;
            }
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
            if (handle_macro_call(trimmed_line, current_macro, output_file, line_number) == 0) {
                /* Write the line to the output file */
                fprintf(output_file, "%s\n", trimmed_line);
            }
        }
    }

    /* Clean up: free the macro linked list and associated content */
    while (current_macro != NULL) {
        temp = current_macro;
        current_macro = current_macro->next;
        free(temp->content); /* Free allocated content memory */
        free(temp);
    }

    /* Close file handles */
    fclose(input_file);
    fclose(output_file);

    if (!error_free) {
        printf("Errors occurred while parsing macros\n");
    }
}
