#include "Eval.h"
#include "SymbolTable.h"
#include "StringBuffer.h"

static Value *eval(SymbolTable *table, Ast *ast);
static Value *calc_unary(AstType type, Value *lhs);
static Value *calc_binary(AstType type, Value *lhs, Value *rhs);
static Value *calc_add(Value *lhs, Value *rhs);
static Value *calc_sub(Value *lhs, Value *rhs);
static Value *calc_mul(Value *lhs, Value *rhs);
static Value *calc_div(Value *lhs, Value *rhs);
static Value *calc_seq(Value *lhs, Value *rhs);
static Value *evalId(SymbolTable *table, Ast *ast);
static Value *evalAssign(SymbolTable *table, Ast *ast);
static Value *evalBinaryOp(SymbolTable *table, Ast *ast);
static Value *evalUnaryOp(SymbolTable *table, Ast *ast);

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
    case AST_ASSIGN:
        return evalAssign(table, ast);
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
    return value;
}

static Value *evalUnaryOp(SymbolTable *table, Ast *ast)
{
    Value *lhs = eval(table, ast->lhs);
    if (isErrorValue(lhs)) {
        return lhs;
    }
    return calc_unary(ast->type, lhs);
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
    return calc_binary(ast->type, lhs, rhs);
}

static Value *calc_unary(AstType type, Value *lhs)
{
    if (type != AST_PRINT) {
        destroyValue(lhs);
        return createErrorValue("unknown unary operator");
    }
    fprintValue(stdout, lhs);
    printf("\n");
    return lhs;
}

static Value *calc_binary(AstType type, Value *lhs, Value *rhs)
{
    switch (type) {
    case AST_ADD:
        return calc_add(lhs, rhs);
    case AST_SUB:
        return calc_sub(lhs, rhs);
    case AST_MUL:
        return calc_mul(lhs, rhs);
    case AST_DIV:
        return calc_div(lhs, rhs);
    case AST_SEQ:
        return calc_seq(lhs, rhs);
    default:
        destroyValue(lhs);
        destroyValue(rhs);
        return createErrorValue("unknown binary operator");
    }
}

static Value *calc_add(Value *lhs, Value *rhs)
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

static Value *calc_sub(Value *lhs, Value *rhs)
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

static Value *calc_mul(Value *lhs, Value *rhs)
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

static Value *calc_div(Value *lhs, Value *rhs)
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

static Value *calc_seq(Value *lhs, Value *rhs)
{
    destroyValue(lhs);
    return rhs;
}
