#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    FILE *file;
    int ch;
} LexingState;

static void runFile(const char *filename);
static void runConsole(void);
static void nextChar(LexingState *state);
static void skipSpace(LexingState *state);
static int expr(LexingState *state);
static int term(LexingState *state);
static int factor(LexingState *state);
static int readNextNum(LexingState *state);

int main(int argc, char *argv[])
{
    printf("TinyLang\n");

    if (argc == 2) {
        runFile(argv[1]);
        return EXIT_SUCCESS;
    }

    if (argc == 1) {
        runConsole();
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Usage: TinyLang [filename]\n");
    return EXIT_FAILURE;
}

static void runFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "failed to open file: \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }

    LexingState state;
    state.file = file;
    nextChar(&state);

    int result = expr(&state);
    printf("%d\n", result);

    fclose(file);
}

static void runConsole(void)
{
    LexingState state;
    state.file = stdin;
    nextChar(&state);

    int result = expr(&state);
    printf("%d\n", result);
}

static void nextChar(LexingState *state)
{
    state->ch = fgetc(state->file);
}

static void skipSpace(LexingState *state)
{
    while (isspace(state->ch)) nextChar(state);
}

static int expr(LexingState *state)
{
    int lhs = term(state);
    for (;;) {
        skipSpace(state);
        if (state->ch == '+') {
            nextChar(state);
            lhs += term(state);
            continue;
        }
        if (state->ch == '-') {
            nextChar(state);
            lhs -= term(state);
            continue;
        }
        break;
    }
    return lhs;
}

static int term(LexingState *state)
{
    int lhs = factor(state);
    for (;;) {
        skipSpace(state);
        if (state->ch == '*') {
            nextChar(state);
            lhs *= factor(state);
            continue;
        }
        if (state->ch == '/') {
            nextChar(state);
            lhs /= factor(state);
            continue;
        }
        break;
    }
    return lhs;
}

static int factor(LexingState *state)
{
    skipSpace(state);
    if (isdigit(state->ch)) {
        return readNextNum(state);
    }
    if (state->ch == '(') {
        nextChar(state);
        int result = expr(state);
        skipSpace(state);
        if (state->ch != ')') {
            printf("syntax error\n");
            exit(EXIT_FAILURE);
        }
        nextChar(state);
        return result;
    }
    printf("syntax error\n");
    exit(EXIT_FAILURE);
}

static int readNextNum(LexingState *state)
{
    int num = 0;
    while (isdigit(state->ch)) {
        num = num * 10 + (state->ch - '0');
        nextChar(state);
    }
    return num;
}
