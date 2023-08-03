
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol_table {
    char symbol[100];
    int value;
    struct symbol_table* prev;
} symbol_table;

symbol_table* add(symbol_table* prev, const char* symbol);
int check_if_exists(symbol_table* prev, const char* symbol);

#endif /* SYMBOL_TABLE_H */