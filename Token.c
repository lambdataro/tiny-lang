#include <stdlib.h>
#include "Token.h"
#include "Util.h"

static void fprintSimpleToken(FILE *file, Token *token);

static struct {
    TokenType type;
    const char *name;
} simpleTokenNameList[] = {
    {TOKEN_PLUS, "TOKEN_PLUS"},
    {TOKEN_MINUS, "TOKEN_MINUS"},
    {TOKEN_ASTERISK, "TOKEN_ASTERISK"},
    {TOKEN_SLASH, "TOKEN_SLASH"},
    {TOKEN_LEFT_PAREN, "TOKEN_LEF_PAREN"},
    {TOKEN_RIGHT_PAREN, "TOKEN_RIGHT_PAREN"},
    {TOKEN_EOF, "TOKEN_EOF"},
    {END_OF_TOKEN_TYPE_LIST, NULL}
};

Token *createToken(TokenType type)
{
    Token *token = allocAndCheck(sizeof(Token));
    token->type = type;
    return token;
}

void destroyToken(Token *token)
{
    free(token);
}

void fprintToken(FILE *file, Token *token)
{
    if (token->type == TOKEN_INT) {
        fprintf(file, "TOKEN_INT(%d)", token->intVal);
        return;
    }
    fprintSimpleToken(file, token);
}

static void fprintSimpleToken(FILE *file, Token *token)
{
    for (int i = 0; simpleTokenNameList[i].name; i++) {
        if (simpleTokenNameList[i].type == token->type) {
            fprintf(file, "%s", simpleTokenNameList[i].name);
            return;
        }
    }
    fprintf(file, "(unknown token)");
}
