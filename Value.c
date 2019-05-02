#include <stdlib.h>
#include "Value.h"
#include "Util.h"
#include "Memory.h"

static Value *createValue(MemoryPool *pool, ValueType type);

static Value *createValue(MemoryPool *pool, ValueType type)
{
    Value *value = allocValue(pool);
    value->type = type;
    return value;
}

Value *createVoidValue(MemoryPool *pool)
{
    return createValue(pool, VALUE_VOID);
}

Value *createIntValue(MemoryPool *pool, int intVal)
{
    Value *value = createValue(pool, VALUE_INT);
    value->intVal = intVal;
    return value;
}

Value *createStrValue(MemoryPool *pool, const char *strVal)
{
    Value *value = createValue(pool, VALUE_STR);
    value->strVal = allocAndCopyString(strVal);
    return value;
}

Value *createErrorValue(MemoryPool *pool, const char *strVal)
{
    Value *value = createValue(pool, VALUE_ERROR);
    value->strVal = allocAndCopyString(strVal);
    return value;
}

void destroyValue(MemoryPool *pool, Value *value)
{
    freeValue(pool, value);
}

Value *createValueCopy(Value *srcValue)
{
    incrRefCount(srcValue);
    return srcValue;
}

void fprintValue(FILE *file, Value *value)
{
    if (!value) {
        fprintf(file, "(null value)");
        return;
    }
    switch (value->type) {
    case VALUE_ERROR:
        fprintf(file, "Error: %s", value->strVal);
        return;
    case VALUE_VOID:
        fprintf(file, "(void)");
        return;
    case VALUE_INT:
        fprintf(file, "%d", value->intVal);
        return;
    case VALUE_STR:
        fprintf(file, "%s", value->strVal);
        return;
    default:
        fprintf(file, "(unknown value)");
        return;
    }
}

bool isErrorValue(Value *value)
{
    return value->type == VALUE_ERROR;
}
