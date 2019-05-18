#ifndef TINY_LANG_AST_H_INCLUDED
#define TINY_LANG_AST_H_INCLUDED

#include <stdio.h>

typedef enum {
    AST_ERROR,
    AST_MODULE_PROC,
    END_OF_AST_TYPE_LIST
} AstType;

typedef struct {
    AstType type;
    union {
        char *strVal;
    };
} Ast;

Ast *createAst(AstType type);
Ast *createErrorAst(const char *message);
void destroyAst(Ast *ast);
void fprintAst(FILE *file, Ast *ast, int indent);

#endif // TINY_LANG_AST_H_INCLUDED
