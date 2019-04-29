#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Stream.h"
#include "Util.h"
#include "Lexing.h"

static void runFile(const char *filename);
static void runRepl(void);
static void startParse(Stream *stream);
static int parseExpr(LexingState *state);
static int parseAddExpr(LexingState *state);
static int parseMulExpr(LexingState *state);
static int parseSimpleExpr(LexingState *state);

int main(int argc, char *argv[])
{
    if (argc == 2) {
        runFile(argv[1]);
        return EXIT_SUCCESS;
    }

    if (argc == 1) {
        printf("TinyLang\n");
        runRepl();
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Usage: TinyLang [filename]\n");
    return EXIT_FAILURE;
}

static void runFile(const char *filename)
{
    Stream *stream = createFileStream(filename);
    if (!stream) {
        fprintf(stderr, "failed to open file: \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }
    startParse(stream);
    destroyStream(stream);
}

static void runRepl(void)
{
    for (;;) {
        printf("> ");
        fflush(stdout);
        char *str = readConsoleUntilEnterTwice();
        if (!str) {
            free(str);
            break;
        }
        Stream *stream = createStringStream(str);
        free(str);
        startParse(stream);
        destroyStream(stream);
    }
}

static void startParse(Stream *stream)
{
    LexingState *state = createLexingState(stream);
    nextToken(state);
    int result = parseExpr(state);
    if (state->token->type != TOKEN_EOF) {
        printf("syntax error\n");
    }
    printf("%d\n", result);
    destroyLexingState(state);
}

static int parseExpr(LexingState *state)
{
    return parseAddExpr(state);
}

static int parseAddExpr(LexingState *state)
{
    int lhs = parseMulExpr(state);
    for (;;) {
        if (state->token->type == TOKEN_PLUS) {
            nextToken(state);
            lhs += parseMulExpr(state);
            continue;
        }
        if (state->token->type == TOKEN_MINUS) {
            nextToken(state);
            lhs -= parseMulExpr(state);
            continue;
        }
        break;
    }
    return lhs;
}

static int parseMulExpr(LexingState *state)
{
    int lhs = parseSimpleExpr(state);
    for (;;) {
        if (state->token->type == TOKEN_ASTERISK) {
            nextToken(state);
            lhs *= parseSimpleExpr(state);
            continue;
        }
        if (state->token->type == TOKEN_SLASH) {
            nextToken(state);
            lhs /= parseSimpleExpr(state);
            continue;
        }
        break;
    }
    return lhs;
}

static int parseSimpleExpr(LexingState *state)
{
    if (state->token->type == TOKEN_INT) {
        int value = state->token->intVal;
        nextToken(state);
        return value;
    }
    if (state->token->type == TOKEN_LEFT_PAREN) {
        nextToken(state);
        int value = parseExpr(state);
        if (state->token->type != TOKEN_RIGHT_PAREN) {
            printf("syntax error\n");
            return 0;
        }
        nextToken(state);
        return value;
    }
    printf("syntax error\n");
    return 0;
}
