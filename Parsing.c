#include "Parsing.h"
#include "Util.h"

static Ast *parseStmt(LexingState *state);
static Ast *parseStmtPrint(LexingState *state);

static Ast *parseExpr(LexingState *state);

Ast *startParse(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseStmt(state);

    if (state->token->type != TOKEN_EOF) {
        destroyAst(ast);
        ast = createErrorAst("syntax error");
    }

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
    if (state->token->type == TOKEN_LIT_STR) {
        Ast *ast = createAst(AST_EXPR_STR);
        ast->strVal = allocAndCopyString(state->token->strVal);
        nextToken(state);
        return ast;
    }

    return createErrorAst("syntax error");
}
