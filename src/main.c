#include <stdio.h>
#include "parse.h"

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

  int x = 0;

  while((x = getToken(fp)) && x != eof) {
    printf("%c -- %d\n", x, x);
  }
  
  fclose(fp);
}
