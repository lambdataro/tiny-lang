#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Lexer.h"
#include "Util.h"
#include "StringBuffer.h"

static bool isIdStartChar(int ch);
static bool isIdChar(int ch);
static void nextChar(LexingState *state);
static void skipSpace(LexingState *state);
static Token *readIdToken(LexingState *state);
static char *readIdString(LexingState *state);
static Token *readSymbolToken(LexingState *state);

static struct {
    const char ch;
    TokenType type;
} symbolTokenTable[] = {
    {'(',  TOKEN_LEFT_PAREN},
    {')',  TOKEN_RIGHT_PAREN},
    {'{',  TOKEN_LEFT_BRACE},
    {'}',  TOKEN_RIGHT_BRACE},
    {'\0', END_OF_TOKEN_TYPE_LIST}
};

static struct {
    const char *str;
    TokenType type;
} keywordTable[] = {
    {"proc", TOKEN_KWD_PROC},
    {NULL,   END_OF_TOKEN_TYPE_LIST}
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

    if (isIdStartChar(state->ch)) {
        return state->token = readIdToken(state);
    }

    return state->token = readSymbolToken(state);
}

static bool isIdStartChar(int ch)
{
    return isalpha(ch) || ch == '_';
}

static bool isIdChar(int ch)
{
    return isalnum(ch) || ch == '_';
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

static Token *readIdToken(LexingState *state)
{
    char *str = readIdString(state);
    for (int i = 0; keywordTable[i].str; i++) {
        if (strcmp(str, keywordTable[i].str) == 0) {
            Token *token = createToken(keywordTable[i].type);
            free(str);
            return token;
        }
    }
    Token *token = createToken(TOKEN_ID);
    token->strVal = str;
    return token;
}

static char *readIdString(LexingState *state)
{
    StringBuffer *buffer = createStringBuffer();
    while (isIdChar(state->ch)) {
        stringBufferAddChar(buffer, state->ch);
        nextChar(state);
    }
    char *str = stringBufferToString(buffer);
    destroyStringBuffer(buffer);
    return str;
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
