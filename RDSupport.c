/* Recursive Descent Support Functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "RDSupport.h"
#include "Scanner.h"
#include "IOMngr.h"

int CurToken;
int Failed = 0;

void
RDInit() {
  CurToken = yylex();
}

int
CurrentToken() {
  return CurToken;
}

bool
Match(int ExpectedToken) {
  if (CurToken == ExpectedToken) {
    CurToken = yylex();
    return true;
  }
  else {
    if (!Failed) ParseError("Token Match Failed");
    Failed = true;
    return false;
  }
}

char * callTrail = NULL;

void
pushTrail(const char * name) {
  if (callTrail) {
    callTrail = realloc(callTrail,strlen(callTrail) + strlen(name) + 2 + 1);
    strcat(callTrail,"> ");
    strcat(callTrail,name);
  }
  else {
    callTrail = strdup(name);
  }
}

void
popTrail() {
  char * p = strrchr(callTrail,'>');
  if (p) *p = '\0';
}

void
Enter(const char *Name) {
  pushTrail(Name);
//  printf("enter %s\n",callTrail);
}

bool
LeaveFail() {
  popTrail();
//  printf("leave fail %s\n",callTrail);
  return false;
}

bool
LeaveSucc() {
  popTrail();
//  printf("leave succ %s\n",callTrail);
  return true;
}

void
ParseError(char *Message) {
  char * formatStr = "CurToken: %d -- %s -- %s";
  char * buf = malloc(strlen(formatStr) + 16 + strlen(callTrail) + strlen(Message) + 1);
	sprintf(buf,formatStr,CurToken,callTrail,Message);
	PostMessage(MakeSpanFromLength(tokenStartPosition,yyleng),buf);
  free(buf);
}
