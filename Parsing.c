#include <stdbool.h>
#include "Parsing.h"
#include "Util.h"

static Ast *parseExpr(LexingState *state);
static Ast *parseStmt(LexingState *state);
static Ast *parseStmtList(LexingState *state);
static Ast *parseCmpExpr(LexingState *state);
static Ast *parseAddExpr(LexingState *state);
static Ast *parseMulExpr(LexingState *state);
static Ast *parseSimpleExpr(LexingState *state);
static Ast *parseSimpleExprInt(LexingState *state);
static Ast *parseSimpleExprStr(LexingState *state);
static Ast *parseSimpleExprId(LexingState *state);
static Ast *parseSimpleExprParens(LexingState *state);
static Ast *parseAssign(LexingState *state);
static Ast *parsePrint(LexingState *state);
static Ast *parseStmtBlock(LexingState *state);
static Ast *parseWhile(LexingState *state);
static Ast *parseIf(LexingState *state);
static Ast *parseSimpleExprPair(LexingState *state);
static Ast *parseSimpleExprToString(LexingState *state);
static Ast *parseSimpleExprFst(LexingState *state);
static Ast *parseSimpleExprSnd(LexingState *state);

Ast *startParse(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseStmtList(state);

    if (state->token->type != TOKEN_EOF) {
        destroyAst(ast);
        ast = createErrorAst("syntax error");
    }

    return ast;
}

static Ast *parseStmtList(LexingState *state)
{
    Ast *lhs = parseStmt(state);
    if (state->token->type == TOKEN_SEMICOLON) {
        nextToken(state);
        Ast *rhs = parseStmtList(state);
        lhs = createBinaryOpAst(AST_SEQ, lhs, rhs);
    }
    return lhs;
}

static Ast *parseStmt(LexingState *state)
{
    switch (state->token->type) {
    case TOKEN_KWD_PRINT:
        return parsePrint(state);
    case TOKEN_ID:
        return parseAssign(state);
    case TOKEN_LEFT_BRACE:
        return parseStmtBlock(state);
    case TOKEN_KWD_WHILE:
        return parseWhile(state);
    case TOKEN_KWD_IF:
        return parseIf(state);
    default:
        return createErrorAst("syntax error");
    }
}

static Ast *parseAssign(LexingState *state)
{
    Ast *ast = createAst(AST_ASSIGN);
    ast->strVal = allocAndCopyString(state->token->strVal);
    nextToken(state);
    if (state->token->type != TOKEN_EQUAL) {
        destroyAst(ast);
        return createErrorAst("equal expected");
    }
    nextToken(state);
    ast->lhs = parseExpr(state);
    return ast;
}

static Ast *parsePrint(LexingState *state)
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

static Ast *parseStmtBlock(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseStmtList(state);
    if (state->token->type != TOKEN_RIGHT_BRACE) {
        destroyAst(ast);
        return createErrorAst("unclosed block");
    }
    nextToken(state);
    return ast;
}

static Ast *parseWhile(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren required");
    }
    nextToken(state);
    Ast *lhs = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(lhs);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    Ast *rhs = parseStmt(state);
    return createBinaryOpAst(AST_WHILE, lhs, rhs);
}

static Ast *parseIf(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren required");
    }
    nextToken(state);
    Ast *lhs = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(lhs);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    Ast *rhs = parseStmt(state);
    return createBinaryOpAst(AST_IF, lhs, rhs);
}

static Ast *parseExpr(LexingState *state)
{
    return parseCmpExpr(state);
}

static Ast *parseCmpExpr(LexingState *state)
{
    Ast *lhs = parseAddExpr(state);
    while (true) {
        if (state->token->type == TOKEN_LEFT_ANGLE) {
            nextToken(state);
            Ast *rhs = parseAddExpr(state);
            lhs = createBinaryOpAst(AST_LESS_THAN, lhs, rhs);
            continue;
        }
        break;
    }
    return lhs;
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
    case TOKEN_STR:
        return parseSimpleExprStr(state);
    case TOKEN_ID:
        return parseSimpleExprId(state);
    case TOKEN_LEFT_PAREN:
        return parseSimpleExprParens(state);
    case TOKEN_LEFT_BRACKET:
        return parseSimpleExprPair(state);
    case TOKEN_KWD_STR:
        return parseSimpleExprToString(state);
    case TOKEN_KWD_FST:
        return parseSimpleExprFst(state);
    case TOKEN_KWD_SND:
        return parseSimpleExprSnd(state);
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

static Ast *parseSimpleExprStr(LexingState *state)
{
    Ast *ast = createAst(AST_STR);
    ast->strVal = state->token->strVal;
    nextToken(state);
    return ast;
}

static Ast *parseSimpleExprId(LexingState *state)
{
    Ast *ast = createAst(AST_ID);
    ast->strVal = allocAndCopyString(state->token->strVal);
    nextToken(state);
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

static Ast *parseSimpleExprPair(LexingState *state)
{
    nextToken(state);
    Ast *fstExpr = parseExpr(state);
    if (state->token->type != TOKEN_COMMA) {
        destroyAst(fstExpr);
        return createErrorAst("comma expected");
    }
    nextToken(state);
    Ast *sndExpr = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_BRACKET) {
        destroyAst(fstExpr);
        destroyAst(sndExpr);
        return createErrorAst("unclosed bracket");
    }
    nextToken(state);
    return createBinaryOpAst(AST_PAIR, fstExpr, sndExpr);
}

static Ast *parseSimpleExprToString(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren required");
    }
    nextToken(state);
    Ast *lhs = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(lhs);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    return createUnaryOpAst(AST_TO_STRING, lhs);
}

static Ast *parseSimpleExprFst(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren required");
    }
    nextToken(state);
    Ast *lhs = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(lhs);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    return createUnaryOpAst(AST_FST, lhs);
}

static Ast *parseSimpleExprSnd(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren required");
    }
    nextToken(state);
    Ast *lhs = parseExpr(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(lhs);
        return createErrorAst("unclosed expression");
    }
    nextToken(state);
    return createUnaryOpAst(AST_SND, lhs);
}
