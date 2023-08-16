#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

typedef struct symbol_table symbol_table; /* Forward declaration */

/* Function to return the next symbol in the symbol table */
symbol_table* get_next_symbol(symbol_table* node);

/* Function to return a symbol */
char* get_symbol(symbol_table* node);

/* Function to get a symbol's value */
int get_symbol_value(symbol_table* node);

/* Function to set a symbol's value */
int set_symbol_value(symbol_table* node, int value);

/* Function to search a symbol in the symbol table */
symbol_table* search_symbol(symbol_table* head, const char* symbol);

/* Function to set a symbol's external or entry value */
void set_symbol_external_or_entry(symbol_table* node, int extern_or_entry);

/* Function to get a symbol's external or entry value */
int get_symbol_external_or_entry(symbol_table* node);

/* Function to set a symbol's type */
void set_symbol_type(symbol_table* node, int type);

/* Function to get a symbol's type */
int get_symbol_type(symbol_table* node);

/* Function to set a symbol's pre-defined entry value */
void set_symbol_pre_defined_entry(symbol_table* node, int pre_defined_entry);

/* Function to get a symbol's pre-defined entry value */
int get_symbol_pre_defined_entry(symbol_table* node);

/* Function to check if a symbol already exists in the symbol table */
int symbol_exists(symbol_table* head, const char* symbol, int line_number);

/* Function to check symbol legality */
int check_symbol_legality(const char* symbol, int line_number);

/* Function to add a new node to the symbol table */
symbol_table* insert_symbol(symbol_table* head, const char* symbol, int value, int line_number);

#endif
