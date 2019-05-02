#ifndef TINY_LANG_VALUE_H_INCLUDED
#define TINY_LANG_VALUE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct MemoryPool MemoryPool;

typedef enum {
    VALUE_ERROR,
    VALUE_VOID,
    VALUE_INT,
    VALUE_STR
} ValueType;

typedef struct {
    ValueType type;
    union {
        int intVal;
        char *strVal;
    };
} Value;

Value *createVoidValue(MemoryPool *pool);
Value *createIntValue(MemoryPool *pool, int intVal);
Value *createStrValue(MemoryPool *pool, const char *strVal);
Value *createErrorValue(MemoryPool *pool, const char *strVal);
void destroyValue(MemoryPool *pool, Value *value);
Value *createValueCopy(Value *value);
void fprintValue(FILE *file, Value *value);
bool isErrorValue(Value *value);

#endif // TINY_LANG_VALUE_H_INCLUDED
