#include <stdlib.h>
#include "Token.h"
#include "Util.h"

Token *createToken(TokenType type)
{
    Token *token = allocAndCheck(sizeof(Token));
    token->type = type;
    return token;
}

void destroyToken(Token *token)
{
    if (token->type == TOKEN_ID) {
        free(token->strVal);
    }
    free(token);
}
