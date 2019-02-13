/* IOMngrDriver.c

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

#include "IOMngr.h"

enum States {INIT, IDENT, NUMBR, FINAL};
enum Tokens {EOF_TOK, IDENT_TOK, NUMBR_TOK, EQUAL_TOK, SEMI_TOK, UNKWN_TOK};
enum Actions {NOP, GET, APP};
enum CharClasses {ALPHA_CLASS, DIGIT_CLASS, EQUAL_CLASS, SEMI_CLASS, SPACE_CLASS, EOF_CLASS, UNKWN_CLASS};

int letter;
int tokenLength;

struct action {
  int nextState;        // transition state
  int token;            // token number if final state
  enum Actions action;  // action: no-operation, get next char, append and get next char
};

struct action M[FINAL][UNKWN_CLASS+1] = {
//         char class -->
// state        alpha                    digit                    =                        ;                  whitespace               EOF                     UNKWN
/*INIT*/   { {IDENT,-1,APP},        {NUMBR,-1,APP},        {FINAL,EQUAL_TOK,APP}, {FINAL,SEMI_TOK,APP},  {INIT,-1,GET},         {FINAL,EOF_TOK,APP},   {FINAL,UNKWN_TOK,APP} },
/*IDENT*/  { {IDENT,-1,APP},        {FINAL,IDENT_TOK,NOP}, {FINAL,IDENT_TOK,NOP}, {FINAL,IDENT_TOK,NOP}, {FINAL,IDENT_TOK,NOP}, {FINAL,IDENT_TOK,NOP}, {FINAL,IDENT_TOK,NOP} },
/*NUMBR*/  { {FINAL,NUMBR_TOK,NOP}, {NUMBR,-1,APP},        {FINAL,NUMBR_TOK,NOP}, {FINAL,NUMBR_TOK,NOP}, {FINAL,NUMBR_TOK,NOP}, {FINAL,NUMBR_TOK,NOP}, {FINAL,NUMBR_TOK,NOP} }
};

int LetterClass(int c) {
  // could be replaced with table lookup but this better display the info
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return ALPHA_CLASS;
  if (c >= '0' && c <= '9') return DIGIT_CLASS;
  if (c == '=') return EQUAL_CLASS;
  if (c == ';') return SEMI_CLASS;
  if (c == ' ' || c == '\n' || c == '\r') return SPACE_CLASS;
  if (c == EOF) return EOF_CLASS;
  return UNKWN_CLASS;
}

int GetNextToken() {
  int token = 0;
  int state = 0;
  tokenLength = 0;
  while (state < FINAL) {
    //fprintf(stderr,"state: %d letter: %c\n",state,letter);
    int class = LetterClass(letter);
    int nstate = M[state][class].nextState;
    token = M[state][class].token;
    int action = M[state][class].action;
    switch (action){
      case NOP:
        break;
      case GET:
        letter = GetSourceChar();
        break;
      case APP:
        tokenLength++;
        letter = GetSourceChar();
        break;
    }
    state = nstate;
  }
  //fprintf(stderr,"return %d\n",token);
  return token;
}

int
main(int argc, char **argv) {
  // collect command line arguments and source file
  bool exitOnUnknown = false;
  int c;
  opterr = 0;
  while ((c = getopt(argc,argv,"u")) != -1) {
    switch (c) {
      case 'u':
        exitOnUnknown = true;
        break;
      case '?':
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        break;
    }
  }

  if (optind >= argc) {
    fprintf(stderr,"usage: IOMngrDriver [-u] filename\n");
    return 1;
  }

  if (OpenSource(argv[optind])) {
    // we delibertly bump up against the EOF three times
    int eofCnt = 0;
    letter = GetSourceChar();
    while(eofCnt < 3) {
      int token = GetNextToken();
      //fprintf(stderr,"tok: %d\n",token);
      switch (token) {
        case EOF_TOK: {
          eofCnt++;
          PostMessage(MakeEOFSpan(), "EOF found");
        } break;
        case IDENT_TOK: {
          //PostMessage(MakeSpanForToken(GetCurrentColumn()-tokenLength, tokenLength, "IDENT found");
        } break;
        case NUMBR_TOK: {
          PostMessage(MakeSpanFromLength(GetLastPosition()-tokenLength, tokenLength), "NUMBR found");
        } break;
        case EQUAL_TOK: {
          PostMessage(MakeSpanFromLength(GetLastPosition()-1, 1), "EQUAL found");
        } break;
        case SEMI_TOK: {
          PostMessage(MakeSpanFromLength(GetLastPosition()-1, 1), "SEMI found");
        } break;
        case UNKWN_TOK: {
          if (exitOnUnknown) {
            PostMessage(MakeSpanFromLength(GetLastPosition()-1, 1), "UNKWN found");
            CloseSource();
            exit(0);
          }
        } break;
      }
   }
    CloseSource();
  }
  else {
    printf("Files could not be opened.\n");
  }

  exit(0);
}
