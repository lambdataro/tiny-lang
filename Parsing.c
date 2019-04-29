#include <stdbool.h>
#include "Parsing.h"
#include "Util.h"

static Ast *parseExpr(LexingState *state);
static Ast *parseSeqExpr(LexingState *state);
static Ast *parseAddExpr(LexingState *state);
static Ast *parseMulExpr(LexingState *state);
static Ast *parseSimpleExpr(LexingState *state);

Ast *startParse(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseExpr(state);

    if (state->token->type != TOKEN_EOF) {
        destroyAst(ast);
        ast = createErrorAst("syntax error");
    }

    return ast;
}

static Ast *parseExpr(LexingState *state)
{
    return parseSeqExpr(state);
}

static Ast *parseSeqExpr(LexingState *state)
{
    Ast *lhs = parseAddExpr(state);
    if (state->token->type == TOKEN_SEMICOLON) {
        nextToken(state);
        Ast *rhs = parseSeqExpr(state);
        lhs = createBinaryOpAst(AST_SEQ, lhs, rhs);
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
    if (state->token->type == TOKEN_INT) {
        Ast *ast = createAst(AST_INT);
        ast->intVal = state->token->intVal;
        nextToken(state);
        return ast;
    }

    if (state->token->type == TOKEN_LEFT_PAREN) {
        nextToken(state);
        Ast *ast = parseExpr(state);
        if (state->token->type != TOKEN_RIGHT_PAREN) {
            destroyAst(ast);
            return createErrorAst("unclosed expression");
        }
        nextToken(state);
        return ast;
    }

    if (state->token->type == TOKEN_KWD_PRINT) {
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

    return createErrorAst("syntax error");
}
