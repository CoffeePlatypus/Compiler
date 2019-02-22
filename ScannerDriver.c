/* ScannerDriver.c

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ScanTokens.h"
#include "SymTab.h"
#include "IOMngr.h"
#include "Scanner.h"

#define MESSAGE_SIZE 256
#define ACTION_MESSAGE_SIZE 32

struct Attributes {
  struct Span span;
  int cnt;
};
enum AttrKinds { IGNORE_KIND };

// used with qsort to sort list of symbol table entries
int entryCmp(const void * A, const void * B) {
  // A is pointer to element of array which contains a pointer to a struct SymEntry
  const char * strA = GetName(*(struct SymEntry **)A);
  const char * strB = GetName(*(struct SymEntry **)B);
  return strcmp(strA,strB);
}

int main(int argc, char **argv) {
  int Token;
  char message[MESSAGE_SIZE];
  char actionMessage[ACTION_MESSAGE_SIZE];
  struct SymTab * table = NULL;

  bool ret = OpenSource("ScannerSource");
  if (!ret) {
    printf("Could not open source and listing files\n");
    exit(1);
  }

  int intSum = 0;
  float floatSum = 0;

  while ((Token = yylex()) != 0) {
    snprintf(actionMessage,ACTION_MESSAGE_SIZE," ");
    switch(Token) {
      case INIT_TOK: {
        // create a symbol table
        if (!table) table = CreateSymTab(20,"main",NULL);
      } break;
      case IDENT_TOK: {
        // place the identifier in the table (if it exists), if new then create and init
        // attributes structure, if already in table then update attributes cnt field, in
        // each case set actionMessage with one of
        // sprintf(actionMessage," -- No SymbolTable");
        // sprintf(actionMessage," -- occurrence %d",attr->cnt);
        // sprintf(actionMessage," -- new ident");
        if (!table) {
          snprintf(actionMessage,ACTION_MESSAGE_SIZE," -- No SymbolTable");
          break;
        }
        //
        // code
        //
      } break;
      case INT_TOK: {
        // code
      } break;
      case FLOAT_TOK: {
        // code
      } break;
      case DUMP_TOK: {
        printf("---------\n");
        printf("intSum = %d\n",intSum);
        printf("floatSum = %f\n",floatSum);
        // get table statistics, alloc an array to hold entry pointers
        struct Stats * stats = Statistics(table);
        struct SymEntry ** entries = malloc(stats->entryCnt * sizeof(struct SymEntry *));
        memcpy(entries,GetEntries(table,false,NULL),stats->entryCnt * sizeof(struct SymEntry *));

        // sort the entries
        qsort(entries,stats->entryCnt,sizeof(struct SymEntry *),entryCmp);

        // list the contents of the table in sorted order
        printf("\nContents of Symbol Table\n");
        printf("                    name  strt  stop   cnt \n");
        for (int i = 0; i < stats->entryCnt; i++) {
          printf("%3d %20s %5d %5d %5d\n", i,
                 GetName(entries[i]),
                 ((struct Attributes *) GetAttr(entries[i]))->span.first,
                 ((struct Attributes *) GetAttr(entries[i]))->span.last,
                 ((struct Attributes *) GetAttr(entries[i]))->cnt);
        }
        free(stats);
      } break;
    }
    sprintf(message,"Token#=%d, Length=%lu, Text=\"%s\"%*c%s",Token,yyleng,yytext,(int)(15-yyleng),' ',actionMessage);
    //fprintf(stderr,"msg: %s\n",message);
    PostMessage(MakeSpanFromLength(tokenStartPosition,yyleng),message);
  }
  PostMessage(MakeEOFSpan(),"EOF");
  CloseSource();
}
