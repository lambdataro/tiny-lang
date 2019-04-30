#include <stdio.h>
#include <stdlib.h>
#include "Ast.h"
#include "Util.h"

static const char *astTypeName(AstType type);

static struct {
    AstType type;
    const char *name;
} astTypeNameTable[] = {
    {AST_ERROR,  "AST_ERROR"},
    {AST_ID,     "AST_ID"},
    {AST_INT,    "AST_INT"},
    {AST_ADD,    "AST_ADD"},
    {AST_SUB,    "AST_SUB"},
    {AST_MUL,    "AST_MUL"},
    {AST_DIV,    "AST_DIV"},
    {AST_LESS_THAN, "AST_LESS_THAN"},
    {AST_ASSIGN, "AST_ASSIGN"},
    {AST_PRINT,  "AST_PRINT"},
    {AST_SEQ,    "AST_SEQ"},
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

Ast *createUnaryOpAst(AstType type, Ast *lhs)
{
    if (lhs->type == AST_ERROR) {
        return lhs;
    }

    Ast *ast = createAst(type);
    ast->lhs = lhs;
    return ast;
}

Ast *createBinaryOpAst(AstType type, Ast *lhs, Ast *rhs)
{
    if (lhs->type == AST_ERROR) {
        destroyAst(rhs);
        return lhs;
    }

    if (rhs->type == AST_ERROR) {
        return rhs;
    }

    Ast *ast = createAst(type);
    ast->lhs = lhs;
    ast->rhs = rhs;
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
    if (ast->type == AST_ERROR ) {
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

    if (isUnaryOpAst(ast)) {
        fprintf(file, "%s:\n", astTypeName(ast->type));
        fprintAst(file, ast->lhs, indent + 1);
        return;
    }

    if (isBinaryOpAst(ast)) {
        fprintf(file, "%s:\n", astTypeName(ast->type));
        fprintAst(file, ast->lhs, indent + 1);
        fprintf(file, "\n");
        fprintAst(file, ast->rhs, indent + 1);
        return;
    }

    if (ast->type == AST_INT) {
        fprintf(file, "AST_INT(%d)", ast->intVal);
        return;
    }

    if (ast->type == AST_ID) {
        fprintf(file, "AST_ID(%s)", ast->strVal);
        return;
    }

    if (ast->type == AST_ASSIGN) {
        fprintf(file, "AST_ASSIGN(%s):\n", ast->strVal);
        fprintAst(file, ast->lhs, indent+1);
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

bool isUnaryOpAst(Ast *ast)
{
    return ast->type == AST_PRINT;
}

bool isBinaryOpAst(Ast *ast)
{
    switch (ast->type) {
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    case AST_LESS_THAN:
    case AST_SEQ:
    case AST_WHILE:
        return true;
    default:
        return false;
    }
}
