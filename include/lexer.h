#include <stdio.h>

enum Token {
  eof, identifier, number, keyword,
  fn,

  invalid
};

char identifierStr[44];
char lastChar;
int identifierLen;
long double numberVal;

enum Token getToken(FILE* fp);
void nextWord(FILE* fp);
