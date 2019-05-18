#include <stdio.h>
#include <stdlib.h>
#include "Stream.h"
#include "Compiler.h"
#include "Util.h"
#include "Lexer.h"

static void compileFile(const char *filename);
static Stream *openSourceFile(const char *filename);
void closeSourceFile(Stream *stream);
static void generateCode(void);
static void runCode(void);

int main(int argc, char *argv[])
{
    if (argc == 2) {
        compileFile(argv[1]);
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Usage: TinyLang [filename]\n");
    return EXIT_FAILURE;
}

static void compileFile(const char *filename)
{
    Stream *stream = openSourceFile(filename);
    LexingState *lexingState = createLexingState(stream);
    destroyLexingState(lexingState);
    closeSourceFile(stream);
    generateCode();
    runCode();
}

static Stream *openSourceFile(const char *filename)
{
    Stream *stream = createFileStream(filename);
    if (!stream) {
        fprintf(stderr, "failed to open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    return stream;
}

void closeSourceFile(Stream *stream)
{
    destroyStream(stream);
}

static void generateCode(void)
{
    const char *filename = "build.asm";
    FILE *outFile = fopen(filename, "w");
    if (!outFile) {
        fprintf(stderr, "failed to create output file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    startCompile(outFile);
    fclose(outFile);
}

static void runCode(void)
{
    runCommand("fasm build.asm");
    runCommand("build.exe");
}
