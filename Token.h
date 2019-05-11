#ifndef TINY_LANG_TOKEN_H_INCLUDED
#define TINY_LANG_TOKEN_H_INCLUDED

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_ID,
    TOKEN_LIT_INT,
    TOKEN_LIT_STR,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_KWD_PROC,
    TOKEN_KWD_PRINT,
    TOKEN_EOF,
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
