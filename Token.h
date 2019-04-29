#ifndef TINY_LANG_TOKEN_H_INCLUDED
#define TINY_LANG_TOKEN_H_INCLUDED

#include <stdio.h>

typedef enum {
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    END_OF_TOKEN_TYPE_LIST
} TokenType;

typedef struct {
    TokenType type;
    union {
        int intVal;
    };
} Token;

Token *createToken(TokenType type);
void destroyToken(Token *token);
void fprintToken(FILE *file, Token *token);

#endif //TINY_LANG_TOKEN_H_INCLUDED
