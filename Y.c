/* Main of Y Language Compiler
   Base Driver
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#include "Scanner.h"
#include "IOMngr.h"
#include "Grammar.h"
#include "YSemantics.h"

int
main(int argc, char * argv[]) {
  if (argc != 2) {
    fprintf(stderr,"usage: Y [-l] base-filename\n");
    return 1;
  }

  char * srcName = malloc(strlen(argv[optind]) + 4 + 1);
  strcpy(srcName,argv[optind]);
  strcat(srcName,".src");

  char * asmName = malloc(strlen(argv[optind]) + 4 + 1);
  strcpy(asmName,argv[optind]);
  strcat(asmName,".asm");

  if (!OpenSource(srcName)) {
    fprintf(stderr,"Source File Not Found.\n");
    exit(1);
  }

  InitSemantics();
  InitCodeGen(asmName);

  int ret = yyparse();
  FinishSemantics();
  //CloseSource();
  fprintf(stdout, (ret == 0) ? "Parse Successful\n" : "Parse Unsuccessful\n");

  exit(0);
}
