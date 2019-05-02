#include <stdlib.h>
#include "Value.h"
#include "Util.h"

static Value *createValue(ValueType type);

static Value *createValue(ValueType type)
{
    Value *value = allocAndCheck(sizeof(Value));
    value->type = type;
    return value;
}

Value *createVoidValue(void)
{
    return createValue(VALUE_VOID);
}

Value *createIntValue(int intVal)
{
    Value *value = createValue(VALUE_INT);
    value->intVal = intVal;
    return value;
}

Value *createErrorValue(const char *strVal)
{
    Value *value = createValue(VALUE_ERROR);
    value->strVal = allocAndCopyString(strVal);
    return value;
}

void destroyValue(Value *value)
{
    if (value->type == VALUE_ERROR) {
        free(value->strVal);
    }
    free(value);
}

Value *createValueCopy(Value *srcValue)
{
    Value *dstValue = allocAndCheck(sizeof(Value));
    dstValue->type = srcValue->type;
    switch (srcValue->type) {
    case VALUE_VOID:
        break;
    case VALUE_INT:
        dstValue->intVal = srcValue->intVal;
        break;
    case VALUE_ERROR:
        dstValue->strVal = allocAndCopyString(srcValue->strVal);
        break;
    default:
        fprintf(stderr, "failed to copy value\n");
        exit(EXIT_FAILURE);
    }
    return dstValue;
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
    default:
        fprintf(file, "(unknown value)");
        return;
    }
}

bool isErrorValue(Value *value)
{
    return value->type == VALUE_ERROR;
}
