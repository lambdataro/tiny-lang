#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "Stream.h"
#include "Util.h"
#include "Lexing.h"
#include "Parsing.h"
#include "Eval.h"

static void runFile(const char *filename);
static void runRepl(void);
static void parseAndRunCode(Stream *stream, SymbolTable *table);

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
    SymbolTable *table = createSymbolTable();
    parseAndRunCode(stream, table);
    destroyStream(stream);
    destroySymbolTable(table);
}

static void runRepl(void)
{
    SymbolTable *table = createSymbolTable();
    while (true) {
        printf("\n");
        statistics(table->pool);
        printf("> ");
        fflush(stdout);
        char *str = readConsoleUntilEnterTwice();
        if (!str) {
            free(str);
            break;
        }
        Stream *stream = createStringStream(str);
        free(str);
        parseAndRunCode(stream, table);
        destroyStream(stream);
    }
    destroySymbolTable(table);
}

static void parseAndRunCode(Stream *stream, SymbolTable *table)
{
    LexingState *state = createLexingState(stream);
    Ast *ast = startParse(state);
    fprintAst(stdout, ast, 0);
    fprintf(stdout, "\n\n");

    if (ast->type != AST_ERROR) {
        Value *value = startEval(table, ast);
        destroyValue(table->pool, value);
    }

    destroyAst(ast);
    destroyLexingState(state);
}
