#include <stdio.h>

enum Token {
  eof, identifier, number, keyword
};

static char* identifierStr;
static long double numberVal;

enum Token getToken(FILE* fp);
