#include <stdio.h>
#include "Compiler.h"

static void outputFasmHeader(FILE *file);
static void outputTextSection(FILE *file, Ast *ast);
static void outputIdataSection(FILE *file);

void startCompile(FILE *file, Ast *ast)
{
    outputFasmHeader(file);
    outputTextSection(file, ast);
    outputIdataSection(file);
}

static void outputFasmHeader(FILE *file)
{
    fprintf(file, "format PE64 console\n");
    fprintf(file, "entry start\n");
    fprintf(file, "\n");
    fprintf(file, "include \'win64w.inc\'\n");
    fprintf(file, "\n");
}

static void outputTextSection(FILE *file, Ast *ast)
{
    fprintf(file, "section \'.text\' code readable executable\n");
    fprintf(file, "start:\n");
    fprintf(file, "  sub rsp, 8\n");
    fprintf(file, "  stdcall main\n");
    fprintf(file, "  invoke ExitProcess, 0\n");
    fprintf(file, "\n");

    fprintf(file, "proc %s\n", ast->strVal);
    fprintf(file, "  ret\n");
    fprintf(file, "endp\n");
    fprintf(file, "\n");
}

static void outputIdataSection(FILE *file)
{
    fprintf(file, "section \'.idata\' import data readable writeable\n");
    fprintf(file, "  library kernel32, \'kernel32.dll\'\n");
    fprintf(file, "  include \'api\\kernel32.inc\'\n");
    fprintf(file, "\n");
}
