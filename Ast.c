#include "Ast.h"
#include "Util.h"

static const char *astTypeName(AstType type);

static struct {
    AstType type;
    const char *name;
} astTypeNameTable[] = {
    {AST_ERROR,       "AST_ERROR"},
    {AST_MODULE_PROC, "AST_MODULE_PROC"},
    {END_OF_AST_TYPE_LIST, NULL}
};

Ast *createAst(AstType type)
{
    Ast *ast = allocAndCheck(sizeof(Ast));
    ast->type = type;
    return ast;
}

Ast *createErrorAst(const char *message)
{
    Ast *ast = createAst(AST_ERROR);
    ast->strVal = allocAndCopyString(message);
    return ast;
}

void destroyAst(Ast *ast)
{
    if (ast->type == AST_ERROR) {
        free(ast->strVal);
    }
    free(ast);
}

void fprintAst(FILE *file, Ast *ast, int indent)
{
    fprintIndent(file, indent);

    if (ast->type == AST_MODULE_PROC) {
        fprintf(file, "AST_MODULE_PROC(%s)", ast->strVal);
        return;
    }

    fprintf(file, "%s", astTypeName(ast->type));
}

static const char *astTypeName(AstType type)
{
    for (int i = 0; astTypeNameTable[i].name; i++) {
        if (astTypeNameTable[i].type == type) {
            return astTypeNameTable[i].name;
        }
    }
    return "(unknown ast)";
}
