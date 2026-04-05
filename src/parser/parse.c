#include "parse.h"
#include "ast.h"
#include <string.h>

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
      strcpy(c->args[c->numArgs], parseExpr(fp));
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

void* parsePrototype(FILE* fp) {
  if(currentToken != identifier) LOG_ERROR("expected function name in prototype");

  ProtAST* p = (ProtAST*)malloc(sizeof(ProtAST));
  p->args = (char**)malloc(sizeof(char*)*128);
  p->name = identifierStr;

  getNextToken(fp);
  if(currentToken != '(') LOG_ERROR("expected '(' in prototype");

  while(getNextToken(fp) == identifier) {
    strcpy(p->args[p->argLen], identifierStr);
    p->argLen++;
  }

  if(currentToken != ')') LOG_ERROR("expected ')' in prototype");
  getNextToken(fp);

  return (void*)p;
}

void* parseDefinition(FILE* fp) {
  getNextToken(fp);
  void* p = parsePrototype(fp);
  if(!p) return NULL;

  void* e = parseExpr(fp);
  if(e) {
    FuncAST* f = (FuncAST*)malloc(sizeof(FuncAST));
    f->prot = (ProtAST*)p;
    f->body = e;
    return (void*)f;
  }

  return NULL;
}

void* parseTopLevelExpr(FILE* fp) {
  void* e = parseExpr(fp);
  if(e) {
    ProtAST* p = (ProtAST*)malloc(sizeof(ProtAST));
    p->name = "__anon_expr";
    FuncAST* f = (FuncAST*)malloc(sizeof(FuncAST));
    f->prot = (ProtAST*)p;
    f->body = e;
    return (void*)f;
  }

  return NULL;
}

void handleTPE(FILE* fp) {
  void* x = parseTopLevelExpr(fp);
  if(x) {
    fprintf(stderr, "top\n");
    free(((FuncAST*)x)->prot);
    free(x);
  } else {
    getNextToken(fp);
  }
}

void handleDef(FILE* fp) {
  void* x = parseDefinition(fp);
  if(x) {
    fprintf(stderr, "function\n");
    free(((FuncAST*)x)->prot->args);
    free(((FuncAST*)x)->prot);
    free(x);
  } else {
    getNextToken(fp);
  }
}

// just for testing
void loop(FILE* fp) {
  getNextToken(fp);

  while(1) {
    switch(currentToken) {
    case eof: return;
    case ';': {
      getNextToken(fp);
      break;
    }
    case fn: {
      handleDef(fp);
      break;
    }
    default: {
      handleTPE(fp);
    }
    }
  }
}
