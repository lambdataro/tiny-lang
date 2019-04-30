#ifndef TINY_LANG_EVAL_H_INCLUDED
#define TINY_LANG_EVAL_H_INCLUDED

#include "Ast.h"
#include "Value.h"
#include "SymbolTable.h"

Value *startEval(SymbolTable *table, Ast *ast);

#endif // TINY_LANG_EVAL_H_INCLUDED
