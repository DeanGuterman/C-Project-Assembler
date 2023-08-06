
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#define MAX_SYMBOL_LENGTH 30

typedef struct symbol_table {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    int value;
    struct symbol_table* next;
} symbol_table;

symbol_table* insert_symbol(symbol_table* head, const char* symbol, int ic_value);
int symbol_exists(symbol_table* head, const char* symbol);

#endif /* SYMBOL_TABLE_H */