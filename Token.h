#ifndef TINY_LANG_TOKEN_H_INCLUDED
#define TINY_LANG_TOKEN_H_INCLUDED

#include <stdio.h>

typedef enum {
    TOKEN_ID,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_KWD_PROC,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    END_OF_TOKEN_TYPE_LIST
} TokenType;

typedef struct {
    TokenType type;
    union {
        int intVal;
        char *strVal;
    };
} Token;

Token *createToken(TokenType type);
void destroyToken(Token *token);

#endif // TINY_LANG_TOKEN_H_INCLUDED
