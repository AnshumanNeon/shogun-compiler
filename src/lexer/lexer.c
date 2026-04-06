#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

enum Token matchToken() {
  if(0 == strcmp(identifierStr, "str")) return keyword;
  if(0 == strcmp(identifierStr, "char")) return keyword;
  if(0 == strcmp(identifierStr, "ptr")) return keyword;
  if(0 == strcmp(identifierStr, "bool")) return keyword;
  if(0 == strcmp(identifierStr, "true")) return keyword;
  if(0 == strcmp(identifierStr, "false")) return keyword;
  if(0 == strcmp(identifierStr, "arr")) return keyword;
  if(0 == strcmp(identifierStr, "tup")) return keyword;
  if(0 == strcmp(identifierStr, "return")) return keyword;
  if(0 == strcmp(identifierStr, "if")) return keyword;
  if(0 == strcmp(identifierStr, "else")) return keyword;
  if(0 == strcmp(identifierStr, "while")) return keyword;
  if(0 == strcmp(identifierStr, "for")) return keyword;
  else return identifier;
}

int isNum() {
  if(0 == strcmp(identifierStr, "int")) return 1;
  if(0 == strcmp(identifierStr, "int8")) return 1;
  if(0 == strcmp(identifierStr, "int16")) return 1;
  if(0 == strcmp(identifierStr, "int32")) return 1;
  if(0 == strcmp(identifierStr, "int64")) return 1;
  if(0 == strcmp(identifierStr, "int128")) return 1;
  if(0 == strcmp(identifierStr, "float")) return 1;
  if(0 == strcmp(identifierStr, "float16")) return 1;
  if(0 == strcmp(identifierStr, "float32")) return 1;
  if(0 == strcmp(identifierStr, "float64")) return 1;
  if(0 == strcmp(identifierStr, "float128")) return 1;

  return 0;
}

void nextWord(FILE* fp) {
  identifierLen = 0;
  strcpy(identifierStr, " ");

  while(isspace(lastChar = fgetc(fp)));
      
  identifierStr[identifierLen] = lastChar;
  identifierLen++;

  while(isalnum(lastChar = fgetc(fp))) {
    identifierStr[identifierLen] = lastChar;
    identifierLen++;
  }
}

enum Token checkNumbers(FILE* fp) {
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

  return invalid;
}

enum Token checkIdentifier(FILE* fp) {
  if(isalpha(lastChar)) {
    identifierStr[identifierLen] = lastChar;
    identifierLen++;

    while(isalpha(lastChar = fgetc(fp))) {
      identifierStr[identifierLen] = lastChar;
      identifierLen++;
    }

    if(0 == strcmp(identifierStr, "var") || (0 == strcmp(identifierStr, "const"))) {
      nextWord(fp);

      if(isNum()) {
	nextWord(fp);
	nextWord(fp);

	if(0 != strcmp(identifierStr, "=")) {
	  return invalid;
	}
	
	nextWord(fp);
	checkNumbers(fp);
      }
    }
    
    if(0 == strcmp(identifierStr, "fn")) {
      identifierLen = 0;
      strcpy(identifierStr, " ");
      return fn;
    }

    return matchToken();
  }

  return invalid;
}

enum Token checkComments(FILE* fp) {
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

  return invalid;
}

enum Token checkParens(FILE* fp) {
  if(lastChar == '(') return '(';
  if(lastChar == ')') return ')';
  return invalid;
}

enum Token getToken(FILE* fp) {
  lastChar = ' ';

  while(isspace(lastChar)) {
    lastChar = fgetc(fp);
  }

  int ret = invalid;

  ret = checkParens(fp);
  if(ret != invalid) return ret;
  
  ret = checkIdentifier(fp);
  if(ret != invalid) return ret;

  ret = checkNumbers(fp);
  if(ret != invalid) return ret;

  ret = checkComments(fp);
  if(ret != invalid) return ret;
  
  if(lastChar == EOF) {
    return eof;
  }

  char thisChar = ret;
  ret = lastChar;
  return thisChar;
}
