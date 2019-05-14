#include "Compile.h"

static void outputAsmHeader(FILE *file);
static void outputTextSection(FILE *file);
static void outputDataSection(FILE *file, Ast *ast);
static void outputIdataSection(FILE *file);

void startCompile(FILE *file, Ast *ast)
{
    outputAsmHeader(file);
    outputTextSection(file);
    outputDataSection(file, ast);
    outputIdataSection(file);
}

static void outputAsmHeader(FILE *file)
{
    fprintf(file, "format PE64 console\n");
    fprintf(file, "entry start\n\n");
    fprintf(file, "include 'win64w.inc'\n");
    fprintf(file, "\n");
}

static void outputTextSection(FILE *file)
{
    fprintf(file, "section '.text' code readable executable\n");
    fprintf(file, "start:\n");
    fprintf(file, "  sub rsp, 8\n");
    fprintf(file, "  invoke GetStdHandle, STD_OUTPUT_HANDLE\n");
    fprintf(file, "  invoke WriteFile, rax, msg, msg.len, output, 0\n");
    fprintf(file, "  invoke ExitProcess, 0\n");
    fprintf(file, "\n");
}

static void outputDataSection(FILE *file, Ast *ast)
{
    fprintf(file, "section '.data' data readable writeable\n");
    fprintf(file, "  output dd ?\n");
    fprintf(file, "  msg db \'%s\'\n", ast->lhs->strVal);
    fprintf(file, "  .len = $ - msg\n");
    fprintf(file, "\n");
}

static void outputIdataSection(FILE *file)
{
    fprintf(file, "section '.idata' import data readable writeable\n");
    fprintf(file, "  library kernel32, 'kernel32.dll'\n");
    fprintf(file, "  include 'api\\kernel32.inc'\n");
}
