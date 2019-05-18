#ifndef TINY_LANG_LEXER_H_INCLUDED
#define TINY_LANG_LEXER_H_INCLUDED

#include "Stream.h"
#include "Token.h"

typedef struct {
    Stream *stream;
    int ch;         // 先読み文字
    Token *token;   // 先読みトークン
} LexingState;

LexingState *createLexingState(Stream *stream);
void destroyLexingState(LexingState *state);
Token *nextToken(LexingState *state);

#endif // TINY_LANG_LEXER_H_INCLUDED
