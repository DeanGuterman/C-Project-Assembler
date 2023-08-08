#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "utils.h"


struct symbol_table;

struct symbol_table* get_next_symbol(struct symbol_table* head);
struct symbol_table* insert_symbol(struct symbol_table* head, const char* symbol, int ic_value, int line_number);
int symbol_exists(struct symbol_table* head, const char* symbol, int line_number);
int check_symbol_legality(const char* symbol, int line_number);
char* get_symbol(struct symbol_table* head);

#endif
