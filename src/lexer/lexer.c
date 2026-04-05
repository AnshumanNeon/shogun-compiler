#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

enum Token matchToken() {
  if(0 == strcmp(identifierStr, "int")) return keyword;
  if(0 == strcmp(identifierStr, "int8")) return keyword;
  if(0 == strcmp(identifierStr, "int16")) return keyword;
  if(0 == strcmp(identifierStr, "int32")) return keyword;
  if(0 == strcmp(identifierStr, "int64")) return keyword;
  if(0 == strcmp(identifierStr, "int128")) return keyword;
  if(0 == strcmp(identifierStr, "float")) return keyword;
  if(0 == strcmp(identifierStr, "float16")) return keyword;
  if(0 == strcmp(identifierStr, "float32")) return keyword;
  if(0 == strcmp(identifierStr, "float64")) return keyword;
  if(0 == strcmp(identifierStr, "float128")) return keyword;
  if(0 == strcmp(identifierStr, "str")) return keyword;
  if(0 == strcmp(identifierStr, "char")) return keyword;
  if(0 == strcmp(identifierStr, "ptr")) return keyword;
  if(0 == strcmp(identifierStr, "bool")) return keyword;
  if(0 == strcmp(identifierStr, "true")) return keyword;
  if(0 == strcmp(identifierStr, "false")) return keyword;
  if(0 == strcmp(identifierStr, "arr")) return keyword;
  if(0 == strcmp(identifierStr, "tup")) return keyword;
  if(0 == strcmp(identifierStr, "fn")) return keyword;
  if(0 == strcmp(identifierStr, "return")) return keyword;
  if(0 == strcmp(identifierStr, "if")) return keyword;
  if(0 == strcmp(identifierStr, "else")) return keyword;
  if(0 == strcmp(identifierStr, "while")) return keyword;
  if(0 == strcmp(identifierStr, "for")) return keyword;
  else return identifier;
}

enum Token getToken(FILE* fp) {
  char lastChar = ' ';
  while(isspace(lastChar)) {
    lastChar = fgetc(fp);
  }

  if(isalpha(lastChar)) {
    identifierStr[identifierLen] = lastChar;
    identifierLen++;

    while(isalnum(lastChar = fgetc(fp))) {
      identifierStr[identifierLen] = lastChar;
      identifierLen++;
    }

    if(0 == strcmp(identifierStr, "var") || (0 == strcmp(identifierStr, "const"))) {
      identifierLen = 0;
      strcpy(identifierStr, " ");

      while(isspace(lastChar)) {
	lastChar = fgetc(fp);
      }
      identifierStr[identifierLen] = lastChar;
      identifierLen++;

      while(isalnum(lastChar = fgetc(fp))) {
	identifierStr[identifierLen] = lastChar;
	identifierLen++;
      }

      printf("%s\n", identifierStr);
    }
    
    if(0 == strcmp(identifierStr, "fn")) return fn;

    return matchToken();
  }

  // check for numbers
  // TODO: handle errors such as 127.3.1.0
  if(isdigit(lastChar) || lastChar == '.') {
    char* num = (char*)malloc(128);
    int n = 0;

    do {
      num[n] = lastChar;
      n++;
      lastChar = fgetc(fp);
    } while(isdigit(lastChar) || lastChar == '.');

    numberVal = strtold(num, 0);
    free(num);
    return number;
  }

  // check for comments
  if(lastChar == '/') {
    lastChar = fgetc(fp);

    if(lastChar == '/') {
      // comment until EOF
      do {
	lastChar = fgetc(fp);
      } while(lastChar != EOF && lastChar != '\n' && lastChar != '\r');

      if(lastChar != EOF) return getToken(fp);
    }
  }

  if(lastChar == EOF) {
    return eof;
  }

  char thisChar = lastChar;
  lastChar = fgetc(fp);
  return thisChar;
}
