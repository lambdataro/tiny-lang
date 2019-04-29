#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Stream.h"
#include "Util.h"

static void runFile(const char *filename);
static void runRepl(void);
static void skipSpace(Stream *stream);
static int expr(Stream *stream);
static int term(Stream *stream);
static int factor(Stream *stream);
static int readNextNum(Stream *stream);

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
    int result = expr(stream);
    printf("%d\n", result);
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

        int result = expr(stream);
        printf("%d\n\n", result);

        destroyStream(stream);
    }
}

static void skipSpace(Stream *stream)
{
    while (isspace(stream->ch)) nextChar(stream);
}

static int expr(Stream *stream)
{
    int lhs = term(stream);
    for (;;) {
        skipSpace(stream);
        if (stream->ch == '+') {
            nextChar(stream);
            lhs += term(stream);
            continue;
        }
        if (stream->ch == '-') {
            nextChar(stream);
            lhs -= term(stream);
            continue;
        }
        break;
    }
    return lhs;
}

static int term(Stream *stream)
{
    int lhs = factor(stream);
    for (;;) {
        skipSpace(stream);
        if (stream->ch == '*') {
            nextChar(stream);
            lhs *= factor(stream);
            continue;
        }
        if (stream->ch == '/') {
            nextChar(stream);
            lhs /= factor(stream);
            continue;
        }
        break;
    }
    return lhs;
}

static int factor(Stream *stream)
{
    skipSpace(stream);
    if (isdigit(stream->ch)) {
        return readNextNum(stream);
    }
    if (stream->ch == '(') {
        nextChar(stream);
        int result = expr(stream);
        skipSpace(stream);
        if (stream->ch != ')') {
            printf("syntax error\n");
            exit(EXIT_FAILURE);
        }
        nextChar(stream);
        return result;
    }
    printf("syntax error\n");
    exit(EXIT_FAILURE);
}

static int readNextNum(Stream *stream)
{
    int num = 0;
    while (isdigit(stream->ch)) {
        num = num * 10 + (stream->ch - '0');
        nextChar(stream);
    }
    return num;
}
