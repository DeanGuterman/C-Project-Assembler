
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol_table {
    char symbol[100];
    int value;
    struct symbol_table* next;
} symbol_table;

void add(symbol_table** head, const char* symbol, int value);
int checkIfExists(symbol_table* head, const char* symbol);

#endif /* SYMBOL_TABLE_H */