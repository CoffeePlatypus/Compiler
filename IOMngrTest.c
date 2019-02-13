#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

#include "IOMngr.h"

int
main(int argc, char * argv[]) {

  OpenSource("IOMngrInput");

  char c;
  int cnt = 0;
  bool needHeader = true;
  printf("offset source\n");
  while ((c = GetSourceChar()) != EOF) {
    if (needHeader) {
      printf("%5d: ",cnt);
      needHeader = false;
    }
    putc(c,stdout);
    cnt++;
    if (c == '\n' || c == '\r') needHeader = true;
  }
  printf("\n");

  PostMessage(MakeSpan(0,3),"This");
  PostMessage(MakeSpan(13,21),"test text");
  // discards at end of list
  PostMessage(MakeSpan(13,21),"1st discard");
  PostMessage(MakeSpan(13,21),"2nd discard");
  PostMessage(MakeSpan(13,16),"3rd discard");
  PostMessage(MakeSpan(16,21),"4th discard");
  PostMessage(MakeSpan(16,24),"5th discard");
  PostMessage(MakeSpan(10,16),"6th discard");
  PostMessage(MakeSpan(10,24),"7th discard");
  PostMessage(MakeSpan(15,17),"8th discard");
  PostMessage(MakeSpan(26,31),"little");
  // discards not at end of list
  PostMessage(MakeSpan(13,21),"1st discard");
  PostMessage(MakeSpan(13,21),"2nd discard");
  PostMessage(MakeSpan(13,16),"3rd discard");
  PostMessage(MakeSpan(16,21),"4th discard");
  PostMessage(MakeSpan(16,24),"5th discard");
  PostMessage(MakeSpan(10,16),"6th discard");
  PostMessage(MakeSpan(10,24),"7th discard");
  PostMessage(MakeSpan(15,17),"8th discard");
  PostMessage(MakeSpan(33,37),"more");
  PostMessage(MakeSpan(43,45),"3 4");
  PostMessage(MakeSpan(47,47),"5");
  PostMessage(MakeSpan(51,55),"7 8 9");
  PostMessage(MakeSpan(70,99),"First ... fifth");
  PostMessage(MakeSpan(105,112),"are done");
  PostMessage(MakeEOFSpan(),"end of source - 1");
  PostMessage(MakeEOFSpan(),"end of source - 2");

  CloseSource();
}
