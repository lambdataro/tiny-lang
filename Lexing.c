#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Lexing.h"
#include "Util.h"
#include "StringBuffer.h"

static bool isIdStartChar(int ch);
static bool isIdChar(int ch);
static void nextChar(LexingState *state);
static void skipSpace(LexingState *state);
static Token *readIdToken(LexingState *state);
static char *readIdString(LexingState *state);
static Token *readStrToken(LexingState *state);
static char escapeChar(char ch);
static Token *readIntToken(LexingState *state);
static Token *readSymbolToken(LexingState *state);

static struct {
    const char ch;
    TokenType type;
} symbolTokenTable[] = {
    {'(',  TOKEN_LEFT_PAREN},
    {')',  TOKEN_RIGHT_PAREN},
    {'\0', END_OF_TOKEN_TYPE_LIST}
};

static struct {
    const char *str;
    TokenType type;
} keywordTable[] = {
    {"print", TOKEN_KWD_PRINT},
    {NULL,    END_OF_TOKEN_TYPE_LIST}
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

    if (isIdStartChar(state->ch)) {
        return state->token = readIdToken(state);
    }

    if (state->ch == '\"') {
        return state->token = readStrToken(state);
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

static Token *readIntToken(LexingState *state)
{
    int value = 0;
    while (isdigit(state->ch)) {
        value = value*10+(state->ch-'0');
        nextChar(state);
    }
    Token *token = createToken(TOKEN_LIT_INT);
    token->intVal = value;
    return token;
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

static Token *readStrToken(LexingState *state)
{
    nextChar(state);
    StringBuffer *buffer = createStringBuffer();
    while (state->ch != '\"' && state->ch != EOF) {
        if (state->ch == '\\') {
            nextChar(state);
            if (state->ch == EOF) {
                break;
            }
            state->ch = escapeChar(state->ch);
        }
        stringBufferAddChar(buffer, state->ch);
        nextChar(state);
    }
    if (state->ch == '\"') {
        nextChar(state);
    }
    Token *token = createToken(TOKEN_LIT_STR);
    token->strVal = stringBufferToString(buffer);
    destroyStringBuffer(buffer);
    return token;
}

static char escapeChar(char ch)
{
    switch (ch) {
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    default:
        return ch;
    }
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
