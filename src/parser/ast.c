#include "ast.h"
#include <stdlib.h>

void* parseNumExpr(FILE* fp) {
  NumAST* n = (NumAST*)malloc(sizeof(NumAST));
  n->value = numberVal;
  getNextToken(fp);
  return (void*)n;
}

void* parseParenExpr(FILE* fp) {
  getNextToken(fp);
  void* V = parseExpr();
  if(!V) return NULL;

  if(currentToken != ')') return LOG_ERROR("expected ')'");
  getNextToken(fp);
  return (void*)V;
}

void* parseIdentifierExpr(FILE* fp) {
  getNextToken(fp);

  if(currentToken != '(') {
    VarExprASt* v = (VarExprAST*)malloc(sizeof(NumAST));
    v->name = identifierStr;
    return v;
  }

  getNextToken(fp);
  CallExprAST* c = (CallExprAST*)malloc(sizeof(CallExprAST));
  c->name = identifierStr;
  if(currentToken != '(') {
    while(1) {
      c->args[c->numArgs] = parseExpr();
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
  default: LOG_ERROR("unknown token");
  }
}
