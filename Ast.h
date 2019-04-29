#ifndef TINY_LANG_AST_H_INCLUDED
#define TINY_LANG_AST_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    AST_ERROR,
    AST_INT,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV
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
Ast *createUnaryOpAst(AstType type, Ast *lhs);
Ast *createBinaryOpAst(AstType type, Ast *lhs, Ast *rhs);
void destroyAst(Ast *ast);
void fprintAst(FILE *file, Ast *ast, int indent);
bool isBinaryOpAst(Ast *ast);

#endif // TINY_LANG_AST_H_INCLUDED
