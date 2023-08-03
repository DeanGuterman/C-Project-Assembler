#include <stdio.h>

#include <string.h>

#include <stdlib.h>

/* Structure to hold macro information */
typedef struct Macro {
    char name[50];
    char * content;
    struct Macro * next;
}
        Macro;

/* Function to parse macros from .as file to .am file */
void parse_macros(char * argv) {
    char file_name[100];
    FILE * input_file;
    char output_argv[100];
    char * dot;
    FILE * output_file;
    char line[200];
    Macro * macros;
    Macro * current_macro;
    int inside_macro;
    Macro * temp;
    int len;
    int i;
    strcpy(file_name, argv);
    strcat(file_name, ".as");
    printf("Parsing macros from file: %s\n", file_name);
    input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        printf("Error opening file: %s\n", argv);
        return;
    }

    /* Create the output file name */
    strcpy(output_argv, argv);
    dot = strrchr(output_argv, '.');
    if (dot != NULL) {
        * dot = '\0';
    }
    strcat(output_argv, ".am");

    output_file = fopen(output_argv, "w");
    if (output_file == NULL) {
        printf("Error creating output file: %s\n", output_argv);
        fclose(input_file);
        return;
    }

    macros = NULL;
    current_macro = NULL;
    inside_macro = 0;

    /* Read the input file line by line */
    while (fgets(line, sizeof(line), input_file)) {
        char * trimmed_line;
        /* Trim trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        /* Ignore empty lines */
        if (strlen(line) == 0) {
            continue;
        }

        /* Trim leading whitespace */
        trimmed_line = line;
        while ( * trimmed_line == ' ' || * trimmed_line == '\t') {
            ++trimmed_line;
        }
        /* Ignore lines that start with ';' */
        if ( * trimmed_line == ';') {
            continue;
        }

        /* Check if the line starts with "mcro" */
        if (strncmp(trimmed_line, "mcro", 4) == 0) {
            /* Create a new macro and add it to the linked list */
            Macro * new_macro;
            new_macro = (Macro * ) malloc(sizeof(Macro));
            strncpy(new_macro -> name, trimmed_line + 5, sizeof(new_macro -> name) - 1);
            new_macro -> name[sizeof(new_macro -> name) - 1] = '\0';

            /* Remove trailing whitespace from macro name */
            len = strlen(new_macro -> name);
            i = len - 1;
            while (i >= 0 && (new_macro -> name[i] == ' ' || new_macro -> name[i] == '\t')) {
                new_macro -> name[i] = '\0';
                i--;
            }
            new_macro -> content = NULL; /* Initialize content pointer */
            new_macro -> next = NULL;

            if (macros == NULL) {
                macros = new_macro;
            } else {
                new_macro -> next = macros;
                macros = new_macro;
            }

            current_macro = new_macro;
            inside_macro = 1;
        } else if (strncmp(trimmed_line, "endmcro", 7) == 0) {
            /* Reached the end of a macro definition */
            inside_macro = 0;
        } else if (inside_macro) {
            /* Append the line to the current macro's content */
            if (current_macro->content == NULL) {
                size_t trimmed_len;
                trimmed_len = 0;
                while (trimmed_line[trimmed_len] != '\0') {
                    trimmed_len++;
                }

                current_macro->content = (char*)malloc(trimmed_len + 1);
                if (current_macro->content != NULL) {
                    size_t i;
                    for (i = 0; i <= trimmed_len; i++) {
                        current_macro->content[i] = trimmed_line[i];
                    }
                }
            } else {
                size_t content_len;
                size_t line_len;
                char * new_content;
                content_len = strlen(current_macro -> content);
                line_len = strlen(trimmed_line);
                new_content = (char * ) malloc(content_len + line_len + 2); /* +2 for '\n' and '\0' */
                strcpy(new_content, current_macro -> content);
                strcat(new_content, "\n");
                strcat(new_content, trimmed_line);
                free(current_macro -> content);
                current_macro -> content = new_content;
            }
        } else {
            /* Check if the line contains a macro call */
            int macro_call_found;
            char * line_copy;
            char * token;
            line_copy = (char*)malloc(strlen(trimmed_line) + 1);
            macro_call_found = 0;
            strcpy(line_copy, trimmed_line);
            token = strtok(line_copy, " ");
            while (token != NULL) {
                /* Check if the token matches a macro name */
                Macro * current;
                current = macros;
                while (current != NULL) {
                    if (strcmp(current -> name, token) == 0) {
                        macro_call_found = 1;
                        fprintf(output_file, "%s\n", current -> content);
                        break;
                    }
                    current = current -> next;
                }
                token = strtok(NULL, " ");
            }
            if (!macro_call_found) {
                /* Write the line to the output file */
                fprintf(output_file, "%s\n", trimmed_line);
            }
            free(line_copy);
        }
    }

    /* Cleanup: free the macro linked list and associated content */
    while (macros != NULL) {
        temp = macros;
        macros = macros -> next;
        free(temp -> content); /* Free allocated content memory */
        free(temp);
    }

    /* Close file handles */
    fclose(input_file);
    fclose(output_file);

    printf("Macros parsed successfully! Output file: %s\n", output_argv);
}