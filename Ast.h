#ifndef TINY_LANG_AST_H_INCLUDED
#define TINY_LANG_AST_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    AST_ERROR,
    AST_ID,
    AST_INT,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_LESS_THAN,
    AST_ASSIGN,
    AST_PRINT,
    AST_SEQ,
    AST_WHILE,
    AST_IF,
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
Ast *createUnaryOpAst(AstType type, Ast *lhs);
Ast *createBinaryOpAst(AstType type, Ast *lhs, Ast *rhs);
void destroyAst(Ast *ast);
void fprintAst(FILE *file, Ast *ast, int indent);
bool isUnaryOpAst(Ast *ast);
bool isBinaryOpAst(Ast *ast);

#endif // TINY_LANG_AST_H_INCLUDED
