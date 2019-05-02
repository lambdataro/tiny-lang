#include "Eval.h"
#include "SymbolTable.h"
#include "StringBuffer.h"

static Value *eval(SymbolTable *table, Ast *ast);
static Value *calcUnary(SymbolTable *table, AstType type, Value *lhs);
static Value *calcBinary(SymbolTable *table, AstType type, Value *lhs, Value *rhs);
static Value *calcAdd(SymbolTable *table, Value *lhs, Value *rhs);
static Value *calcSub(SymbolTable *table, Value *lhs, Value *rhs);
static Value *calcMul(SymbolTable *table, Value *lhs, Value *rhs);
static Value *calcDiv(SymbolTable *table, Value *lhs, Value *rhs);
static Value *calcLessThan(SymbolTable *table, Value *lhs, Value *rhs);
static Value *calcSeq(SymbolTable *table, Value *lhs, Value *rhs);
static Value *evalId(SymbolTable *table, Ast *ast);
static Value *evalAssign(SymbolTable *table, Ast *ast);
static Value *evalBinaryOp(SymbolTable *table, Ast *ast);
static Value *evalUnaryOp(SymbolTable *table, Ast *ast);
static Value *evalWhile(SymbolTable *table, Ast *ast);
static Value *evalIf(SymbolTable *table, Ast *ast);
static Value *calcPair(SymbolTable *table, Value *lhs, Value *rhs);

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
        return createIntValue(table->pool, ast->intVal);
    case AST_STR:
        return createStrValue(table->pool, ast->strVal);
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
        return createErrorValue(table->pool, "runtime error");
    }
}

static Value *evalId(SymbolTable *table, Ast *ast)
{
    Value *value = findVariable(table, ast->strVal);
    if (!value) {
        StringBuffer *buf = createStringBuffer();
        stringBufferAddString(buf, "undefined valueble: ");
        stringBufferAddString(buf, ast->strVal);
        Value *value = createErrorValue(table->pool, stringBufferToString(buf));
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
    destroyValue(table->pool, value);
    return createVoidValue(table->pool);
}

static Value *evalUnaryOp(SymbolTable *table, Ast *ast)
{
    Value *lhs = eval(table, ast->lhs);
    if (isErrorValue(lhs)) {
        return lhs;
    }
    return calcUnary(table, ast->type, lhs);
}

static Value *evalBinaryOp(SymbolTable *table, Ast *ast)
{
    Value *lhs = eval(table, ast->lhs);
    if (isErrorValue(lhs)) {
        return lhs;
    }
    Value *rhs = eval(table, ast->rhs);
    if (isErrorValue(rhs)) {
        destroyValue(table->pool, lhs);
        return rhs;
    }
    return calcBinary(table, ast->type, lhs, rhs);
}

static Value *calcUnary(SymbolTable *table, AstType type, Value *lhs)
{
    if (type != AST_PRINT) {
        destroyValue(table->pool, lhs);
        return createErrorValue(table->pool, "unknown unary operator");
    }
    fprintValue(stdout, lhs);
    printf("\n");
    return lhs;
}

static Value *calcBinary(SymbolTable *table, AstType type, Value *lhs, Value *rhs)
{
    switch (type) {
    case AST_ADD:
        return calcAdd(table, lhs, rhs);
    case AST_SUB:
        return calcSub(table, lhs, rhs);
    case AST_MUL:
        return calcMul(table, lhs, rhs);
    case AST_DIV:
        return calcDiv(table, lhs, rhs);
    case AST_LESS_THAN:
        return calcLessThan(table, lhs, rhs);
    case AST_SEQ:
        return calcSeq(table, lhs, rhs);
    case AST_PAIR:
        return calcPair(table, lhs, rhs);
    default:
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return createErrorValue(table->pool, "unknown binary operator");
    }
}

static Value *calcAdd(SymbolTable *table, Value *lhs, Value *rhs)
{
    if (lhs->type == VALUE_INT && rhs->type == VALUE_INT) {
        Value *result = createIntValue(table->pool, lhs->intVal+rhs->intVal);
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return result;
    }
    destroyValue(table->pool, lhs);
    destroyValue(table->pool, rhs);
    return createErrorValue(table->pool, "type error: add");
}

static Value *calcSub(SymbolTable *table, Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return createErrorValue(table->pool, "type error: sub");
    }
    Value *result = createIntValue(table->pool, lhs->intVal-rhs->intVal);
    destroyValue(table->pool, lhs);
    destroyValue(table->pool, rhs);
    return result;
}

static Value *calcMul(SymbolTable *table, Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return createErrorValue(table->pool, "type error: mul");
    }
    Value *result = createIntValue(table->pool, lhs->intVal*rhs->intVal);
    destroyValue(table->pool, lhs);
    destroyValue(table->pool, rhs);
    return result;
}

static Value *calcDiv(SymbolTable *table, Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return createErrorValue(table->pool, "type error: div");
    }
    if (rhs->intVal == 0) {
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return createErrorValue(table->pool, "divide by zero");
    }
    Value *result = createIntValue(table->pool, lhs->intVal/rhs->intVal);
    destroyValue(table->pool, lhs);
    destroyValue(table->pool, rhs);
    return result;
}

static Value *calcLessThan(SymbolTable *table, Value *lhs, Value *rhs)
{
    if (lhs->type != VALUE_INT || rhs->type != VALUE_INT) {
        destroyValue(table->pool, lhs);
        destroyValue(table->pool, rhs);
        return createErrorValue(table->pool, "type error: less than");
    }
    Value *result = createIntValue(table->pool, lhs->intVal < rhs->intVal);
    destroyValue(table->pool, lhs);
    destroyValue(table->pool, rhs);
    return result;
}

static Value *calcSeq(SymbolTable *table, Value *lhs, Value *rhs)
{
    destroyValue(table->pool, lhs);
    return rhs;
}

static Value *calcPair(SymbolTable *table, Value *lhs, Value *rhs)
{
    Value *value = createPairValue(table->pool, lhs, rhs);
    destroyValue(table->pool, lhs);
    destroyValue(table->pool, rhs);
    return value;
}

static Value *evalWhile(SymbolTable *table, Ast *ast)
{
    while (true) {
        Value *cond = eval(table, ast->lhs);
        if (isErrorValue(cond)) {
            return cond;
        }
        if (cond->type != VALUE_INT) {
            return createErrorValue(table->pool, "invalid condition");
        }
        if (cond->intVal == 0) {
            destroyValue(table->pool, cond);
            return createVoidValue(table->pool);
        }
        destroyValue(table->pool, cond);
        Value *value = eval(table, ast->rhs);
        if (isErrorValue(value)) {
            return value;
        }
        destroyValue(table->pool, value);
    }
}

static Value *evalIf(SymbolTable *table, Ast *ast)
{
    Value *cond = eval(table, ast->lhs);
    if (isErrorValue(cond)) {
        return cond;
    }
    if (cond->type != VALUE_INT) {
        return createErrorValue(table->pool, "invalid condition");
    }
    if (cond->intVal == 0) {
        destroyValue(table->pool, cond);
        return createVoidValue(table->pool);
    }
    destroyValue(table->pool, cond);
    Value *value = eval(table, ast->rhs);
    if (isErrorValue(value)) {
        return value;
    }
    destroyValue(table->pool, value);
    return createVoidValue(table->pool);
}
