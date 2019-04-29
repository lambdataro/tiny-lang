#include "Eval.h"

static Value *eval(Ast *ast);
static Value *calc_binary(AstType type, Value *lhs, Value *rhs);
static Value *calc_add(Value *lhs, Value *rhs);
static Value *calc_sub(Value *lhs, Value *rhs);
static Value *calc_mul(Value *lhs, Value *rhs);
static Value *calc_div(Value *lhs, Value *rhs);

Value *startEval(Ast *ast)
{
    return eval(ast);
}

static Value *eval(Ast *ast)
{
    if (ast->type == AST_INT) {
        return createIntValue(ast->intVal);
    }

    if (isBinaryOpAst(ast)) {
        int lhs = eval(ast->lhs);
        if (isErrorValue(lhs)) {
            return lhs;
        }
        Value *rhs = eval(ast->rhs);
        if (isErrorValue(rhs)) {
            destroyValue(lhs);
            return rhs;
        }
        return calc_binary(ast->type, lhs, rhs);
    }
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
    default:
        destroyValue(lhs);
        destroyValue(rhs);
        return NULL;
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
