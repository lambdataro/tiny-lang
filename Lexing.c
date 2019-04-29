#include <stdlib.h>
#include <ctype.h>
#include "Lexing.h"
#include "Util.h"

static void nextChar(LexingState *state);
static void skipSpace(LexingState *state);
static Token *readIntToken(LexingState *state);
static Token *readSymbolToken(LexingState *state);

static struct {
    const char ch;
    TokenType type;
} symbolTokenTable[] = {
    {'+', TOKEN_PLUS},
    {'-', TOKEN_MINUS},
    {'*', TOKEN_ASTERISK},
    {'/', TOKEN_SLASH},
    {'(', TOKEN_LEFT_PAREN},
    {')', TOKEN_RIGHT_PAREN},
    {'\0', END_OF_TOKEN_TYPE_LIST}
};

LexingState *createLexingState(Stream *stream)
{
    LexingState *state = allocAndCheck(sizeof(LexingState));
    state->stream = stream;
    state->ch = getChar(state->stream);
    state->token = NULL;
    return state;
}

void destroyLexingState(LexingState *state)
{
    if (state->token) {
        destroyToken(state->token);
    }
    free(state);
}

Token *nextToken(LexingState *state)
{
    if (state->token) {
        destroyToken(state->token);
    }
    skipSpace(state);

    if (state->ch == EOF) {
        return state->token = createToken(TOKEN_EOF);
    }

    if (isdigit(state->ch)) {
        return state->token = readIntToken(state);
    }

    return state->token = readSymbolToken(state);
}

static void nextChar(LexingState *state)
{
    state->ch = getChar(state->stream);
}

static void skipSpace(LexingState *state)
{
    while (isspace(state->ch)) {
        nextChar(state);
    }
}

static Token *readIntToken(LexingState *state)
{
    int value = 0;
    while (isdigit(state->ch)) {
        value = value * 10 + (state->ch - '0');
        nextChar(state);
    }
    Token *token = createToken(TOKEN_INT);
    token->intVal = value;
    return token;
}

static Token *readSymbolToken(LexingState *state)
{
    for (int i = 0; symbolTokenTable[i].ch; i++) {
        if (symbolTokenTable[i].ch == state->ch) {
            Token *token = createToken(symbolTokenTable[i].type);
            nextChar(state);
            return token;
        }
    }
    return createToken(TOKEN_UNKNOWN);
}
