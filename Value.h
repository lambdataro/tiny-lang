#ifndef TINY_LANG_VALUE_H_INCLUDED
#define TINY_LANG_VALUE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    VALUE_ERROR,
    VALUE_VOID,
    VALUE_INT
} ValueType;

typedef struct {
    ValueType type;
    union {
        int intVal;
        char *strVal;
    };
} Value;

Value *createVoidValue(void);
Value *createIntValue(int intVal);
Value *createErrorValue(const char *strVal);
void destroyValue(Value *value);
Value *createValueCopy(Value *value);
void fprintValue(FILE *file, Value *value);
bool isErrorValue(Value *value);

#endif // TINY_LANG_VALUE_H_INCLUDED
