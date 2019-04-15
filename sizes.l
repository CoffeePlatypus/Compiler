%{
#include <stdio.h>
#include <stdlib.h>
%}

%%

%%

int main() {
  FILE * fp = popen("lex -t sizes.l | grep extern | grep yyleng | awk '{ print $2 }' ","r");
  if (fp == NULL) {
    printf("failed to open pipe\n");
    exit(1);
  }
  char s[32];
  fscanf(fp,"%s",s);
  printf("yyleng:    %s\n",s);
  printf("int:       %lu\n",sizeof(int));
  printf("int *:     %lu\n",sizeof(int *));
  printf("size_t:    %lu\n",sizeof(size_t));
  printf("yy_size_t: %lu\n",sizeof(yy_size_t));
}
