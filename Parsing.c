#include <stdbool.h>
#include "Parsing.h"
#include "Util.h"

static Ast *parseExpr(LexingState *state);
static Ast *parseSeqExpr(LexingState *state);
static Ast *parseAddExpr(LexingState *state);
static Ast *parseMulExpr(LexingState *state);
static Ast *parseSimpleExpr(LexingState *state);
static Ast *parseSimpleExprInt(LexingState *state);
static Ast *parseSimpleExprId(LexingState *state);
static Ast *parseSimpleExprParens(LexingState *state);
static Ast *parseSimpleExprPrint(LexingState *state);

Ast *startParse(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseSeqExpr(state);

    if (state->token->type != TOKEN_EOF) {
        destroyAst(ast);
        ast = createErrorAst("syntax error");
    }

    return ast;
}

static Ast *parseSeqExpr(LexingState *state)
{
    Ast *lhs = parseExpr(state);
    if (state->token->type == TOKEN_SEMICOLON) {
        nextToken(state);
        Ast *rhs = parseSeqExpr(state);
        lhs = createBinaryOpAst(AST_SEQ, lhs, rhs);
    }
    return lhs;
}

static Ast *parseExpr(LexingState *state)
{
    return parseAddExpr(state);
}

static Ast *parseAddExpr(LexingState *state)
{
    Ast *lhs = parseMulExpr(state);
    while (true) {
        if (state->token->type == TOKEN_PLUS) {
            nextToken(state);
            Ast *rhs = parseMulExpr(state);
            lhs = createBinaryOpAst(AST_ADD, lhs, rhs);
            continue;
        }
        if (state->token->type == TOKEN_MINUS) {
            nextToken(state);
            Ast *rhs = parseMulExpr(state);
            lhs = createBinaryOpAst(AST_SUB, lhs, rhs);
            continue;
        }
        break;
    }
    return lhs;
}

static Ast *parseMulExpr(LexingState *state)
{
    Ast *lhs = parseSimpleExpr(state);
    while (true) {
        if (state->token->type == TOKEN_ASTERISK) {
            nextToken(state);
            Ast *rhs = parseSimpleExpr(state);
            lhs = createBinaryOpAst(AST_MUL, lhs, rhs);
            continue;
        }
        if (state->token->type == TOKEN_SLASH) {
            nextToken(state);
            Ast *rhs = parseSimpleExpr(state);
            lhs = createBinaryOpAst(AST_DIV, lhs, rhs);
            continue;
        }
        break;
    }
    return lhs;
}

static Ast *parseSimpleExpr(LexingState *state)
{
    switch (state->token->type) {
    case TOKEN_INT:
        return parseSimpleExprInt(state);
    case TOKEN_ID:
        return parseSimpleExprId(state);
    case TOKEN_LEFT_PAREN:
        return parseSimpleExprParens(state);
    case TOKEN_KWD_PRINT:
        return parseSimpleExprPrint(state);
    default:
        return createErrorAst("syntax error");
    }
}

static Ast *parseSimpleExprInt(LexingState *state)
{
    Ast *ast = createAst(AST_INT);
    ast->intVal = state->token->intVal;
    nextToken(state);
    return ast;
}

static Ast *parseSimpleExprId(LexingState *state)
{
    Ast *ast = createAst(AST_ID);
    ast->strVal = allocAndCopyString(state->token->strVal);
    nextToken(state);
    if (state->token->type == TOKEN_EQUAL) {
        nextToken(state);
        ast->type = AST_ASSIGN;
        ast->lhs = parseExpr(state);
    }
    return ast;
}

static Ast *parseSimpleExprParens(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(ast);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    return ast;
}

static Ast *parseSimpleExprPrint(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren required");
    }
    nextToken(state);
    Ast *ast = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(ast);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    return createUnaryOpAst(AST_PRINT, ast);
}
