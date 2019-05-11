#include <stdio.h>
#include <stdlib.h>

void outputHelloWorld(void);

int main(void)
{
    printf("TinyLang\n");
    fflush(stdout);

    outputHelloWorld();
    system("gcc -o prog build.c");
    system("prog");

    return EXIT_SUCCESS;
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
