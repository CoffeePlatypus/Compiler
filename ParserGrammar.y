%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOMngr.h"
#include "Scanner.h"

//extern int yylex(); /* The next token function. */
//extern char *yytext; /* The matched token text. */
//extern int yyleng; /* The token text length. */

void yyerror(char *s);

#define YYSTYPE char *

%}

// These token definitions populate y.tab.h
// Single character tokens are their own token numbers (e.g. scanner returns
// the value ',' for the comma token)
%token INT_TOK 1
%token CHR_TOK 2
%token ASSIGN_TOK 3
%token INTLIT_TOK 4
%token IDENT_TOK 5

%%
Prog    : IDENT_TOK '{' StmtSeq '}'     {  }                                    ;
StmtSeq : Stmt ';' StmtSeq              {  }                                    ;
StmtSeq :                               {  }                                    ;

Stmt    : Decl                          {  }                                    ;
Stmt    : Assign                        {  }                                    ;

Decl    : Type IDList                   {  }                                    ;

Type    : INT_TOK                       {  }                                    ;
Type    : CHR_TOK                       {  }                                    ;

IDList   : IDENT_TOK MLst               {  }                                    ;
MLst    : ',' IDList                    {  }                                    ;
MLst    :                               {  }                                    ;

Assign  : LHS ASSIGN_TOK Expr           { printf("%s =\n", $1); }               ;
LHS     : IDENT_TOK                     { $$ = strdup(yytext); }                ;

Expr    :  Expr AddOp Term              { printf("%s ", $2); }                  ;
Expr    :  Term                         {  }                                    ;

Term    :  Term MultOp Factor           { printf("%s ", $2); }                  ;
Term    :  Factor                       {  }                                    ;

Factor  :  '(' Expr ')'                 {  }                                    ;
Factor  :  '-' Factor                   { printf("-"); }                        ;
Factor  :  INTLIT_TOK                   { printf("%s ", yytext); }              ;
Factor  :  IDENT_TOK                    { printf("%s ", yytext); }              ;
AddOp   :  '-'                          { $$ = strdup(yytext); }                ;
AddOp   :  '+'                          { $$ = strdup(yytext); }                ;
MultOp  :  '*'                          { $$ = strdup(yytext); }                ;
MultOp  :  '/'                          { $$ = strdup(yytext); }                ;

%%

void
yyerror(char *s) {
 // Called when parser encounters error. Write newline to stdout in case partial
 // postfix expression has been generated. Format error message and post to
 // current line. IOMngr will then echo error line along with error message.
 char msg[256];
 printf("\n");
 snprintf(msg,255,"error: \"%s\" yytext: \"%s\"",s,yytext);
 PostMessage(MakeSpanFromLength(tokenStartPosition,yyleng),msg);
}
