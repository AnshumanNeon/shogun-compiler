#ifndef AST_H
#include "lexer.h"
#include <stdio.h>

typedef struct NUMBER_AST {
  long double value;
} NumAST;

typedef struct VARIABLE_EXPR_AST {
  char* name;
} VarExprAST;

typedef struct BINARY_EXPR_AST {
  char op;
  void* LHS;
  void* RHS;
} BinExprAST;

typedef struct CALL_EXPR_AST {
  char* callee;
  void** args; // a list of arguments, each arg is an expression
  unsigned int numArgs;
} CallExprAST;

typedef struct PROTOTYPE_AST {
  char* name;
  char** args; // list of arguments, each arg is a char* string
} ProtAST;

typedef struct FUNC_AST {
  ProtAST* prot;
  void* body;
} FuncAST;

int currentToken;

int getNextToken(FILE* fp) {
  currentToken = getToken(fp);
  return currentToken;
}

#define LOG_ERROR(str) fprintf(stderr, "error: %s\n", str)

#define AST_H
#endif
