#include <stdio.h>
#include <stdlib.h>
#include "Ast.h"
#include "Util.h"

static const char *astTypeName(AstType type);

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

    fprintf(file, "%s", astTypeName(ast->type));
}

static const char *astTypeName(AstType type)
{
    switch (type) {
    case AST_ERROR:
        return "AST_ERROR";
    case AST_INT:
        return "AST_INT";
    case AST_ADD:
        return "AST_ADD";
    case AST_SUB:
        return "AST_SUB";
    case AST_MUL:
        return "AST_MUL";
    case AST_DIV:
        return "AST_DIV";
    default:
        return "(unknown ast)";
    }
}

bool isBinaryOpAst(Ast *ast)
{
    switch (ast->type) {
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
        return true;
    default:
        return false;
    }
}
