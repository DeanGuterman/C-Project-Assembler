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
    strcpy(file_name, argv);
    strcat(file_name, ".as");
    printf("Parsing macros from file: %s\n", file_name);
    FILE * input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        printf("Error opening file: %s\n", argv);
        return;
    }

    /* Create the output file name */
    char output_argv[100];
    strcpy(output_argv, argv);
    char * dot = strrchr(output_argv, '.');
    if (dot != NULL) {
        * dot = '\0';
    }
    strcat(output_argv, ".am");

    FILE * output_file = fopen(output_argv, "w");
    if (output_file == NULL) {
        printf("Error creating output file: %s\n", output_argv);
        fclose(input_file);
        return;
    }

    char line[200];
    Macro * macros = NULL;
    Macro * current_macro = NULL;
    int inside_macro = 0;

    /* Read the input file line by line */
    while (fgets(line, sizeof(line), input_file)) {
        /* Trim trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        /* Ignore empty lines */
        if (strlen(line) == 0) {
            continue;
        }

        /* Trim leading whitespace */
        char * trimmed_line = line;
        while ( * trimmed_line == ' ' || * trimmed_line == '\t') {
            ++trimmed_line;
        }
        /* GOOD */

        /* Check if the line starts with "mcro" */
        if (strncmp(trimmed_line, "mcro", 4) == 0) {
            /* Create a new macro and add it to the linked list */
            Macro * new_macro = (Macro * ) malloc(sizeof(Macro));
            strncpy(new_macro -> name, trimmed_line + 5, sizeof(new_macro -> name) - 1);
            new_macro -> name[sizeof(new_macro -> name) - 1] = '\0';

            /* Remove trailing whitespace from macro name */
            int len = strlen(new_macro -> name);
            int i = len - 1;
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
            if (current_macro -> content == NULL) {
                current_macro -> content = strdup(trimmed_line); /* Allocate memory and copy the line */
            } else {
                size_t content_len = strlen(current_macro -> content);
                size_t line_len = strlen(trimmed_line);
                char * new_content = (char * ) malloc(content_len + line_len + 2); /* +2 for '\n' and '\0' */
                strcpy(new_content, current_macro -> content);
                strcat(new_content, "\n");
                strcat(new_content, trimmed_line);
                free(current_macro -> content);
                current_macro -> content = new_content;
            }
        } else {
            /* Check if the line contains a macro call */
            int macro_call_found = 0;
            char * line_copy[strlen(trimmed_line)];
            strcpy(line_copy, trimmed_line);
            char * token = strtok(line_copy, " ");
            while (token != NULL) {
                /* Check if the token matches a macro name */
                Macro * current = macros;
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
        }
    }

    /* Cleanup: free the macro linked list and associated content */
    Macro * temp;
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