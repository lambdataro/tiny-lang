#include <stdio.h>
#include <stdlib.h>
#include "Stream.h"
#include "Lexing.h"
#include "Parsing.h"

static void compileFile(const char *filename);

int main(int argc, char *argv[])
{
    if (argc == 2) {
        compileFile(argv[1]);
        return EXIT_SUCCESS;
    }

    if (argc == 1) {
        compileFile("..\\tests\\prog01.tl");
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Usage: TinyLang [filename]\n");
    return EXIT_FAILURE;
}

static void compileFile(const char *filename)
{
    Stream *stream = createFileStream(filename);
    if (!stream) {
        fprintf(stderr, "failed to open file: \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }

    LexingState *state = createLexingState(stream);
    Ast *ast = startParse(state);
    fprintAst(stdout, ast, 0);
    destroyAst(ast);
    destroyLexingState(state);
    destroyStream(stream);
}
