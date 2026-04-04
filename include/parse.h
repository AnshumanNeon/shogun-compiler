#ifndef PARSE_H
#include "ast.h"

typedef struct BINOPSMAP {
  char key;
  int precedence;
} BinOpsMap;

BinOpsMap precedenceTable[10];
int precedenceTableLen;

int getTokenPrecedence(int token);
void* parseNumExpr(FILE* fp);
void* parseParenExpr(FILE* fp);
void* parseIdentifierExpr(FILE* fp);
void* parseExpr(FILE* fp);
void* parse(FILE* fp);
void* parseBinOpsRHS(FILE* fp, int exprPrecedence, void* lhs);

#define PARSE_H
#endif
