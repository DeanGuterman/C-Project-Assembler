
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol_table {
    char symbol[100];
    int value;
    struct symbol_table* next;
} symbol_table;

symbol_table* insert_first_symbol(const char* symbol);
symbol_table* insert_symbol(symbol_table* head, const char* symbol);
int symbol_exists(symbol_table* head, const char* symbol);

#endif /* SYMBOL_TABLE_H */