#include "Eval.h"
#include "SymbolTable.h"
#include "StringBuffer.h"

static Value *eval(SymbolTable *table, Ast *ast);
static Value *calcUnary(AstType type, Value *lhs);
static Value *calcBinary(AstType type, Value *lhs, Value *rhs);
static Value *calcAdd(Value *lhs, Value *rhs);
static Value *calcSub(Value *lhs, Value *rhs);
static Value *calcMul(Value *lhs, Value *rhs);
static Value *calcDiv(Value *lhs, Value *rhs);
static Value *calcLessThan(Value *lhs, Value *rhs);
static Value *calcSeq(Value *lhs, Value *rhs);
static Value *evalId(SymbolTable *table, Ast *ast);
static Value *evalAssign(SymbolTable *table, Ast *ast);
static Value *evalBinaryOp(SymbolTable *table, Ast *ast);
static Value *evalUnaryOp(SymbolTable *table, Ast *ast);
static Value *evalWhile(SymbolTable *table, Ast *ast);
static Value *evalIf(SymbolTable *table, Ast *ast);

Value *startEval(SymbolTable *table, Ast *ast)
{
    return eval(table, ast);
}

static Value *eval(SymbolTable *table, Ast *ast)
{
    switch (ast->type) {
    case AST_ID:
        return evalId(table, ast);
    case AST_INT:
        return createIntValue(ast->intVal);
    case AST_STR:
        return createStrValue(ast->strVal);
    case AST_ASSIGN:
        return evalAssign(table, ast);
    case AST_WHILE:
        return evalWhile(table, ast);
    case AST_IF:
        return evalIf(table, ast);
    default:
        if (isUnaryOpAst(ast)) {
            return evalUnaryOp(table, ast);
        }
        if (isBinaryOpAst(ast)) {
            return evalBinaryOp(table, ast);
        }
        return createErrorValue("runtime error");
    }
}

static Value *evalId(SymbolTable *table, Ast *ast)
{
    Value *value = findVariable(table, ast->strVal);
    if (!value) {
        StringBuffer *buf = createStringBuffer();
        stringBufferAddString(buf, "undefined valueble: ");
        stringBufferAddString(buf, ast->strVal);
        Value *value = createErrorValue(stringBufferToString(buf));
        destroyStringBuffer(buf);
        return value;
    }
    return value;
}

static Value *evalAssign(SymbolTable *table, Ast *ast)
{
    Value *value = eval(table, ast->lhs);
    if (isErrorValue(value)) {
        return value;
    }
    addVariable(table, ast->strVal, value);
    destroyValue(value);
    return createVoidValue();
}

static Value *evalUnaryOp(SymbolTable *table, Ast *ast)
{
    Value *lhs = eval(table, ast->lhs);
    if (isErrorValue(lhs)) {
        return lhs;
    }
    return calcUnary(ast->type, lhs);
}

static Value *evalBinaryOp(SymbolTable *table, Ast *ast)
{
    Value *lhs = eval(table, ast->lhs);
    if (isErrorValue(lhs)) {
        return lhs;
    }
    Value *rhs = eval(table, ast->rhs);
    if (isErrorValue(rhs)) {
        destroyValue(lhs);
        return rhs;
    }
    return calcBinary(ast->type, lhs, rhs);
}

static Value *calcUnary(AstType type, Value *lhs)
{
    if (type != AST_PRINT) {
        destroyValue(lhs);
        return createErrorValue("unknown unary operator");
    }
    fprintValue(stdout, lhs);
    printf("\n");
    return lhs;
}

static Value *calcBinary(AstType type, Value *lhs, Value *rhs)
{
    switch (type) {
    case AST_ADD:
        return calcAdd(lhs, rhs);
    case AST_SUB:
        return calcSub(lhs, rhs);
    case AST_MUL:
        return calcMul(lhs, rhs);
    case AST_DIV:
        return calcDiv(lhs, rhs);
    case AST_LESS_THAN:
        return calcLessThan(lhs, rhs);
    case AST_SEQ:
        return calcSeq(lhs, rhs);
    default:
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("unknown binary operator");
    }
}

static Value *calcAdd(Value *lhs, Value *rhs)
{
    if (lhs->type == VALUE_INT && rhs->type == VALUE_INT) {
        Value *result = createIntValue(lhs->intVal + rhs->intVal);
        destroyValue(lhs);
        destroyValue(rhs);
        return result;
    }
    destroyValue(lhs);
    destroyValue(rhs);
    return createErrorValue("type error: add");
}

static Value *calcSub(Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("type error: sub");
    }
    Value *result = createIntValue(lhs->intVal - rhs->intVal);
    destroyValue(lhs);
    destroyValue(rhs);
    return result;
}

static Value *calcMul(Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("type error: mul");
    }
    Value *result = createIntValue(lhs->intVal * rhs->intVal);
    destroyValue(lhs);
    destroyValue(rhs);
    return result;
}

static Value *calcDiv(Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("type error: div");
    }
    if (rhs->intVal == 0) {
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("divide by zero");
    }
    Value *result = createIntValue(lhs->intVal / rhs->intVal);
    destroyValue(lhs);
    destroyValue(rhs);
    return result;
}

static Value *calcLessThan(Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("type error: less than");
    }
    Value *result = createIntValue(lhs->intVal < rhs->intVal);
    destroyValue(lhs);
    destroyValue(rhs);
    return result;
}

static Value *calcSeq(Value *lhs, Value *rhs)
{
    destroyValue(lhs);
    return rhs;
}

static Value *evalWhile(SymbolTable *table, Ast *ast)
{
    while (true) {
        Value *cond = eval(table, ast->lhs);
        if (isErrorValue(cond)) {
            return cond;
        }
        if (cond->type != VALUE_INT) {
            return createErrorValue("invalid condition");
        }
        if (cond->intVal == 0) {
            destroyValue(cond);
            return createVoidValue();
        }
        destroyValue(cond);
        Value *value = eval(table, ast->rhs);
        if (isErrorValue(value)) {
            return value;
        }
        destroyValue(value);
    }
}

static Value *evalIf(SymbolTable *table, Ast *ast)
{
    Value *cond = eval(table, ast->lhs);
    if (isErrorValue(cond)) {
        return cond;
    }
    if (cond->type != VALUE_INT) {
        return createErrorValue("invalid condition");
    }
    if (cond->intVal == 0) {
        destroyValue(cond);
        return createVoidValue();
    }
    destroyValue(cond);
    Value *value = eval(table, ast->rhs);
    if (isErrorValue(value)) {
        return value;
    }
    destroyValue(value);
    return createVoidValue();
}
