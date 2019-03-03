/* Main of Recursive Descent Parser */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "Scanner.h"
#include "IOMngr.h"
#include "RDTokens.h"

#include "RDSupport.h"
#include "RDGrammar.h"

int
main(int argc, char * argv[])
{
  if (argc != 2) {
    fprintf(stderr,"usage: RecDescent filename\n");
    return 1;
  }

  char * srcName = argv[1];
  if (!OpenSource(srcName)) {
    fprintf(stderr,"failed to open %s\n",srcName);
    return 1;
  }

  // to debug token sequencee, set to true
  RDInit(false);
  if (Prog()) {
    if (CurrentToken() == EOF_TOK) {
      PostMessage(MakeEOFSpan(),"Parse Successful");
      CloseSource();
    }
    else {
      PostMessage(MakeEOFSpan(),"Parse Successful but excess input");
      CloseSource();
    }
  }
  else {
    PostMessage(MakeEOFSpan(),"Parse Unsuccessful");
    CloseSource();
  }

  return 0;
}
