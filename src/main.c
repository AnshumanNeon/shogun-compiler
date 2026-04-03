#include <stdio.h>
#include "lexer/lexer.h"

int main(int argc, char** argv) {
  if(argc < 2) {
    printf("Hello, World!\n");
    return 0;
  }

  FILE* fp = fopen(argv[1], "r");
  if(!fp) {
    printf("error in opening the file");
    return -1;
  }

  int x = getToken(fp);

  printf("%c -- %d\n", x, x);
  fclose(fp);
}
