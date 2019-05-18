#include "Parser.h"
#include "Util.h"

#define CONSUME(type)

static Ast *parseDeclProc(LexingState *state);
static Ast *consumeToken(LexingState *state, TokenType type);

Ast *startParse(LexingState *state)
{
    nextToken(state);
    Ast *ast = parseDeclProc(state);

    if (state->token->type != TOKEN_EOF) {
        destroyAst(ast);
        ast = createErrorAst("syntax error");
    }

    return ast;
}

static Ast *parseDeclProc(LexingState *state)
{
    Ast *result;

    result = consumeToken(state, TOKEN_KWD_PROC);
    if (result) return result;

    if (state->token->type != TOKEN_ID) {
        return createErrorAst("syntax error");
    }
    Ast *ast = createAst(AST_MODULE_PROC);
    ast->strVal = allocAndCopyString(state->token->strVal);
    nextToken(state);

    result = consumeToken(state, TOKEN_LEFT_PAREN);
    if (result) return result;

    result = consumeToken(state, TOKEN_RIGHT_PAREN);
    if (result) return result;

    result = consumeToken(state, TOKEN_LEFT_BRACE);
    if (result) return result;

    result = consumeToken(state, TOKEN_RIGHT_BRACE);
    if (result) return result;

    return ast;
}

static Ast *consumeToken(LexingState *state, TokenType type)
{
    if (state->token->type != type) {
        return createErrorAst("syntax error");
    }
    nextToken(state);
    return NULL;
}
