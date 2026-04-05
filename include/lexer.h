#include <stdio.h>

enum Token {
  eof, identifier, number, keyword,
  fn,

  invalid
};

char identifierStr[44];
int identifierLen;
long double numberVal;

enum Token getToken(FILE* fp);
