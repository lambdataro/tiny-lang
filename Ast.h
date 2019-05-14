#ifndef TINY_LANG_AST_H_INCLUDED
#define TINY_LANG_AST_H_INCLUDED

#include <stdio.h>

typedef enum {
    AST_ERROR,
    AST_STMT_PRINT,
    AST_EXPR_STR,
    END_OF_AST_TYPE_LIST
} AstType;

typedef struct Ast {
    AstType type;
    union {
        int intVal;
        char *strVal;
    };
    struct Ast *lhs, *rhs;
} Ast;

Ast *createAst(AstType type);
Ast *createErrorAst(const char *message);
void destroyAst(Ast *ast);
void fprintAst(FILE *file, Ast *ast, int indent);

#endif // TINY_LANG_AST_H_INCLUDED
