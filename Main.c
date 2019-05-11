#include <stdio.h>
#include <stdlib.h>
#include "Stream.h"
#include "Lexing.h"

void outputHelloWorld(void);

int main(int argc, char *argv[])
{
    if (argc == 2) {
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

    // TODO: 処理を追加

    destroyLexingState(state);
    destroyStream(stream);
}

void outputHelloWorld(void)
{
    FILE *file = fopen("build.c", "w");
    if (!file) {
        fprintf(stderr, "failed to create output file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file,
        "#include <stdio.h>\n"
        "int main(void) {\n"
        "  printf(\"hello, world.\\n\");\n"
        "  return 0;\n"
        "}\n");

    fclose(file);
}
