#ifndef TINY_LANG_VALUE_H_INCLUDED
#define TINY_LANG_VALUE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct MemoryPool MemoryPool;

typedef enum {
    VALUE_ERROR,
    VALUE_VOID,
    VALUE_INT,
    VALUE_STR,
    VALUE_PAIR
} ValueType;

typedef struct Value {
    ValueType type;
    union {
        int intVal;
        char *strVal;
        struct {
            struct Value *fstValue;
            struct Value *sndValue;
        } pairVal;
    };
} Value;

Value *createVoidValue(MemoryPool *pool);
Value *createIntValue(MemoryPool *pool, int intVal);
Value *createStrValue(MemoryPool *pool, const char *strVal);
Value *createErrorValue(MemoryPool *pool, const char *strVal);
Value *createPairValue(MemoryPool *pool, Value *fstValue, Value *sndValue);
void destroyValue(MemoryPool *pool, Value *value);
Value *createValueCopy(Value *value);
void fprintValue(FILE *file, Value *value);
bool isErrorValue(Value *value);

#endif // TINY_LANG_VALUE_H_INCLUDED
