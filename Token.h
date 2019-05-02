#ifndef TINY_LANG_TOKEN_H_INCLUDED
#define TINY_LANG_TOKEN_H_INCLUDED

#include <stdio.h>

typedef enum {
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_STR,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_EQUAL,
    TOKEN_LEFT_ANGLE,
    TOKEN_KWD_PRINT,
    TOKEN_KWD_IF,
    TOKEN_KWD_WHILE,
    TOKEN_KWD_STR,
    TOKEN_KWD_FST,
    TOKEN_KWD_SND,
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

#endif //TINY_LANG_TOKEN_H_INCLUDED
