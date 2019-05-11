#include "Parsing.h"
#include "Util.h"

static Ast *parseDef(LexingState *state);
static Ast *parseDefProc(LexingState *state);

static Ast *parseStmt(LexingState *state);
static Ast *parseStmtPrint(LexingState *state);

static Ast *parseExpr(LexingState *state);

Ast *startParse(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseDef(state);

    if (state->token->type != TOKEN_EOF) {
        destroyAst(ast);
        ast = createErrorAst("syntax error");
    }

    return ast;
}

static Ast *parseDef(LexingState *state)
{
    switch (state->token->type) {
    case TOKEN_KWD_PROC:
        return parseDefProc(state);
    default:
        return createErrorAst("syntax error");
    }
}

static Ast *parseDefProc(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_ID) {
        return createErrorAst("proc name expected");
    }
    Ast *ast = createAst(AST_DEF_PROC);
    ast->strVal = allocAndCopyString(state->token->strVal);

    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        destroyAst(ast);
        return createErrorAst("left paren expected");
    }

    nextToken(state);
    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(ast);
        return createErrorAst("right paren expected");
    }

    nextToken(state);
    if (state->token->type != TOKEN_LEFT_BRACE) {
        destroyAst(ast);
        return createErrorAst("left brace expected");
    }

    nextToken(state);
    Ast *body = parseStmt(state);
    if (ast->type == AST_ERROR) {
        destroyAst(ast);
        return body;
    }
    ast->lhs = body;

    if (state->token->type != TOKEN_RIGHT_BRACE) {
        destroyAst(ast);
        return createErrorAst("right brace expected");
    }

    nextToken(state);
    return ast;
}

static Ast *parseStmt(LexingState *state)
{
    switch (state->token->type) {
    case TOKEN_KWD_PRINT:
        return parseStmtPrint(state);
    default:
        return createErrorAst("syntax error");
    }
}

static Ast *parseStmtPrint(LexingState *state)
{
    nextToken(state);
    if (state->token->type != TOKEN_LEFT_PAREN) {
        return createErrorAst("left paren expected");
    }

    nextToken(state);
    Ast *lhs = parseExpr(state);
    if (lhs->type == AST_ERROR) {
        return lhs;
    }

    if (state->token->type != TOKEN_RIGHT_PAREN) {
        destroyAst(lhs);
        return createErrorAst("right paren expected");
    }

    nextToken(state);

    Ast *ast = createAst(AST_STMT_PRINT);
    ast->lhs = lhs;
    return ast;
}

static Ast *parseExpr(LexingState *state)
{
    if (state->token->type == TOKEN_LIT_INT) {
        Ast *ast = createAst(AST_EXPR_INT);
        ast->intVal = state->token->intVal;
        nextToken(state);
        return ast;
    }

    return createErrorAst("syntax error");
}
