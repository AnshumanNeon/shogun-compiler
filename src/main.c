#include <stdio.h>

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

  char c = '\n';
  while(c) {
    c = fgetc(fp);
    if(c == '\0') break;
    printf("%c", c);

    if(feof(fp)) break;
  }

  printf("\n");
  fclose(fp);
}
