#ifndef PARSE_H
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct BINOPSMAP {
  char key;
  int precedence;
} BinOpsMap;

BinOpsMap precedenceTable[10];
int precedenceTableLen;

void initPrecedence();
int getTokenPrecedence(int token);
void* parseNumExpr(FILE* fp);
void* parseParenExpr(FILE* fp);
void* parseIdentifierExpr(FILE* fp);
void* parseExpr(FILE* fp);
void* parse(FILE* fp);
void* parseBinOpsRHS(FILE* fp, int exprPrecedence, void* lhs);
void* parsePrototype(FILE* fp);
void* parseDefinition(FILE* fp);
void* parseTopLevelExpr(FILE* fp);

// just for testing
void loop(FILE* fp);

#define PARSE_H
#endif
