#include <stdio.h>
#include <stdlib.h>
#include "Stream.h"

static void compileFile(const char *filename);

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
    Stream *stream = createFileStream(filename);
    if (!stream) {
        fprintf(stderr, "failed to open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    destroyStream(stream);
}
