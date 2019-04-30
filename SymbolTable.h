#ifndef TINY_LANG_SYMBOLTABLE_H_INCLUDED
#define TINY_LANG_SYMBOLTABLE_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>
#include "Value.h"

typedef struct HashEntry {
    char *key;
    Value *value;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    size_t size;
    HashEntry **table;
} HashTable;

typedef struct {
    HashTable *env;
} SymbolTable;

SymbolTable *createSymbolTable(void);
void destroySymbolTable(SymbolTable *table);
void addVariable(SymbolTable *table, const char *key, Value *value);
Value *findVariable(SymbolTable *table, const char *key);
void fprintSymbolTable(FILE *file, SymbolTable *table);

#endif // TINY_LANG_SYMBOLTABLE_H_INCLUDED
