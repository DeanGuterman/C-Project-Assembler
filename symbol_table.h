#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "utils.h"

/* Forward declaration of the struct symbol_table */
struct symbol_table;

/* Function to return the next symbol in the symbol table */
struct symbol_table* get_next_symbol(struct symbol_table* head);

/* Function to return the symbol */
char* get_symbol(struct symbol_table* head);

/* Function to check if a symbol already exists in the symbol table */
int symbol_exists(struct symbol_table* head, const char* symbol, int line_number);

/* Function to check the legality of a symbol name */
int check_symbol_legality(const char* symbol, int line_number);

/* Function to add a new node to the symbol table */
struct symbol_table* insert_symbol(struct symbol_table* head, const char* symbol, int ic_value, int line_number);

#endif
