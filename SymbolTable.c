#include <stdlib.h>
#include <string.h>
#include "Util.h"
#include "SymbolTable.h"

#define HASH_TABLE_SIZE 3

SymbolTable *createSymbolTable(void)
{
    HashTable *env = allocAndCheck(sizeof(HashTable));
    env->size = HASH_TABLE_SIZE;
    env->table = allocAndCheck(sizeof(HashEntry **)*env->size);
    for (size_t i = 0; i < env->size; i++) {
        env->table[i] = NULL;
    }
    SymbolTable *table = allocAndCheck(sizeof(SymbolTable));
    table->env = env;
    return table;
}

void destroySymbolTable(SymbolTable *table)
{
    HashTable *env = table->env;
    for (size_t i = 0; i < env->size; i++) {
        if (env->table[i]) {
            HashEntry *entry = env->table[i];
            while (entry) {
                HashEntry *p = entry->next;
                free(entry->key);
                destroyValue(entry->value);
                free(entry);
                entry = p;
            }
        }
    }
    free(env);
    free(table);
}

void addVariable(SymbolTable *table, const char *key, Value *value)
{
    size_t index = hash(key)%table->env->size;
    HashEntry *entry = table->env->table[index];
    while (entry) {
        if (strcmp(key, entry->key) == 0) {
            destroyValue(entry->value);
            entry->value = createValueCopy(value);
            return;
        }
        entry = entry->next;
    }
    entry = allocAndCheck(sizeof(HashEntry));
    entry->key = allocAndCopyString(key);
    entry->value = createValueCopy(value);
    entry->next = table->env->table[index];
    table->env->table[index] = entry;
}

Value *findVariable(SymbolTable *table, const char *key)
{
    size_t index = hash(key)%table->env->size;
    HashEntry *entry = table->env->table[index];
    while (entry) {
        if (strcmp(key, entry->key) == 0) {
            return createValueCopy(entry->value);
        }
        entry = entry->next;
    }
    return NULL;
}

void fprintSymbolTable(FILE *file, SymbolTable *table)
{
    fprintf(file, "env:\n");
    for (size_t index = 0; index < table->env->size; index++) {
        fprintf(file, "  slot %d\n", (int) index);
        HashEntry *entry = table->env->table[index];
        while (entry) {
            fprintf(file, "    %s: ", entry->key);
            fprintValue(file, entry->value);
            fprintf(file, "\n");
            entry = entry->next;
        }
    }
    fprintf(file, "\n");
}
