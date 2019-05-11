#ifndef TINY_LANG_LEXING_H_INCLUDED
#define TINY_LANG_LEXING_H_INCLUDED

#include "Stream.h"
#include "Token.h"

typedef struct {
    Stream *stream;
    int ch;
    Token *token;
} LexingState;

LexingState *createLexingState(Stream *stream);
void destroyLexingState(LexingState *state);
Token *nextToken(LexingState *state);

#endif //TINY_LANG_LEXING_H_INCLUDED
