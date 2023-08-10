#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "utils.h"

struct symbol_table; /* Forward declaration */

/* Function to return the next symbol in the symbol table */
struct symbol_table* get_next_symbol(struct symbol_table* node);

/* Function to return the symbol */
char* get_symbol(struct symbol_table* node);

/* Function to search a symbol in the symbol table */
struct symbol_table* search_symbol(struct symbol_table* head, const char* symbol);

void set_symbol_pre_defined_entry(struct symbol_table* node, int pre_defined_entry);

/* Function to set a symbol's external or entry value */
void set_symbol_external_or_entry(struct symbol_table* node, int extern_or_entry);

/* Function to get a symbol's external or entry value */
int get_symbol_external_or_entry(struct symbol_table* node);

/* Function to set a symbol's type */
void set_symbol_type(struct symbol_table* node, int type);

/* Function to set a symbol's is_extern_or_entry value */
void set_symbol_is_extern_or_entry(struct symbol_table* node, int is_extern_or_entry);

/* Function to search and delete a symbol in the symbol table */
struct symbol_table* delete_symbol(struct symbol_table* head, const char* symbol);

/* Function to check if a symbol already exists in the symbol table */
int symbol_exists(struct symbol_table* head, const char* symbol, int line_number);

/* Function to check the legality of a symbol */
int check_symbol_legality(const char* symbol, int line_number);

/* Function to add a new node to the symbol table */
struct symbol_table* insert_symbol(struct symbol_table* head, const char* symbol, int value, int line_number);


#endif
