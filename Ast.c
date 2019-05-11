#include <stdlib.h>
#include "Ast.h"
#include "Util.h"

static const char *astTypeName(AstType type);

static struct {
    AstType type;
    const char *name;
} astTypeNameTable[] = {
    {AST_ERROR,      "AST_ERROR"},
    {AST_DEF_PROC,   "AST_DEF_PROC"},
    {AST_STMT_PRINT, "AST_STMT_PRINT"},
    {AST_EXPR_INT,   "AST_EXPR_INT"},
    {END_OF_AST_TYPE_LIST, NULL}
};

Ast *createAst(AstType type)
{
    Ast *ast = allocAndCheck(sizeof(Ast));
    ast->type = type;
    ast->lhs = NULL;
    ast->rhs = NULL;
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
    if (ast->lhs) {
        free(ast->lhs);
    }
    if (ast->rhs) {
        free(ast->rhs);
    }
    if (ast->type == AST_ERROR || ast->type == AST_DEF_PROC) {
        free(ast->strVal);
    }
    free(ast);
}

void fprintAst(FILE *file, Ast *ast, int indent)
{
    fprintIndent(file, indent);

    if (ast->type == AST_ERROR) {
        fprintf(file, "Error: %s", ast->strVal);
        return;
    }

    if (ast->type == AST_DEF_PROC) {
        fprintf(file, "AST_DEF_PROC(%s):\n", ast->strVal);
        fprintAst(file, ast->lhs, indent+1);
        return;
    }

    if (ast->type == AST_STMT_PRINT) {
        fprintf(file, "AST_STMT_PRINT:\n");
        fprintAst(file, ast->lhs, indent+1);
        return;
    }

    if (ast->type == AST_EXPR_INT) {
        fprintf(file, "AST_INT(%d)", ast->intVal);
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
