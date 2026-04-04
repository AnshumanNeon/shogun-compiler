#include "parse.h"
#include <ctype.h>
#include <stdlib.h>

void initPrecedence() {
  precedenceTable[0] = (BinOpsMap){ .key = '<', .precedence = 10 };
  precedenceTable[2] = (BinOpsMap){ .key = '+', .precedence = 20 };
  precedenceTable[3] = (BinOpsMap){ .key = '-', .precedence = 30 };
  precedenceTable[1] = (BinOpsMap){ .key = '*', .precedence = 40 };
}

int getTokenPrecedence(int token) {
  if(!isascii(token)) return -1;

  int p = -10;
  for(int i = 0; i < precedenceTableLen; i++) {
    if(precedenceTable[i].key == token) {
      p = precedenceTable[i].precedence;
    }
  }

  if(p <= 0) return -1;
  return p;
}

void* parseNumExpr(FILE* fp) {
  NumAST* n = (NumAST*)malloc(sizeof(NumAST));
  n->value = numberVal;
  getNextToken(fp);
  return (void*)n;
}

void* parseParenExpr(FILE* fp) {
  getNextToken(fp);
  void* V = parseExpr(fp);
  if(!V) return NULL;

  if(currentToken != ')') {
    LOG_ERROR("expected ')'");
    return NULL;
  }
  
  getNextToken(fp);
  return (void*)V;
}

void* parseIdentifierExpr(FILE* fp) {
  getNextToken(fp);

  if(currentToken != '(') {
    VarExprAST* v = (VarExprAST*)malloc(sizeof(NumAST));
    v->name = identifierStr;
    return v;
  }

  getNextToken(fp);
  CallExprAST* c = (CallExprAST*)malloc(sizeof(CallExprAST));
  c->callee = identifierStr;
  if(currentToken != '(') {
    while(1) {
      c->args[c->numArgs] = parseExpr(fp);
      c->numArgs++;
      
      if(currentToken == ')') break;
      if(currentToken != ',') LOG_ERROR("Expected ')' or ','");
      getNextToken(fp);
    }
  }

  getNextToken(fp);
  return (void*)c;
}

void* parse(FILE* fp) {
  switch (currentToken) {
  case identifier: return parseIdentifierExpr(fp);
  case number: return parseNumExpr(fp);
  case '(': return parseParenExpr(fp);
  }

  LOG_ERROR("unknown token");
  return NULL;
}

void* parseExpr(FILE* fp) {
  void* lhs = parse(fp);
  if(!lhs) return NULL;

  return parseBinOpsRHS(fp, 0, lhs);
}

void* parseBinOpsRHS(FILE* fp, int exprPrecedence, void* lhs) {
  while(1) {
    int p = getTokenPrecedence(currentToken);
    if(p < exprPrecedence) return lhs;

    int binOp = currentToken;
    getNextToken(fp);

    void* rhs = parse(fp);
    if(!rhs) return NULL;

    int nextp = getTokenPrecedence(currentToken);
    if(p < nextp) {
      rhs = parseBinOpsRHS(fp, p + 1, rhs);
      if(!rhs) return NULL;
    }

    BinExprAST* x = (BinExprAST*)malloc(sizeof(BinExprAST));
    x->op = binOp;
    x->LHS = lhs;
    x->RHS = rhs;

    lhs = x;
  }
}
