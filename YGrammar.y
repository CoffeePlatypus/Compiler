%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "IOMngr.h"
  #include "Scanner.h"
  #include "YSemantics.h"

  void yyerror(char *s);

  #define MAKE_SPAN(loc) MakeSpan(loc.first_column,loc.last_column)
%}

%locations

/* Union structure of data that can be attached to non-terminals   */
/* type union-field-name                                           */
%union     {
  int Integer;
  char * Text;
  enum BaseTypes BaseType;
  struct IdList * IdList;
  struct LiteralDesc * LiteralDesc;
  struct InstrSeq * InstrSeq;
}

/* Type declaration for data attached to non-terminals. Allows     */
/* $# notation to be correctly type casted for function arguments. */
/* %type <union-field-name> non-terminal                           */
%type <Text> Id
%type <IdList> IdItem
%type <IdList> IdList
%type <BaseType> BaseType
%type <LiteralDesc> Literal
%type <BaseType> FuncDecl
%type <InstrSeq> CodeBlock
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <InstrSeq> AssignStmt

/* List of token name, corresponding numbers will be generated */
/* y.tab.h will be generated from these */
/* %token symbolic-name text-used-in-rules */
%token IDENT_TOK
%token INT_TOK    	   "int"
%token CHR_TOK    	   "chr"
%token BOOL_TOK        "bool"
%token VOID_TOK        "void"
%token INTLIT_TOK
%token CHRLIT_TOK
%token BOOLLIT_TOK
%token ARROW_TOK       "->"
%token DBLCOLON_TOK    "::"

/* operator associativity and precedence in increasing order */

%%
// Step 1
Module        : DeclList                                    { printf("why"); };

DeclList      : Decl DeclList                               { printf("here?");};
DeclList      : Decl                                        { printf("woiej?");};

Decl          : IdList ':' BaseType                         { printf("Base type\n"); ProcDecl($1,$3,0); };
Decl          : IdList "::" Literal                         { printf("Literal\n"); ProcDecl($1,$3->baseType,$3->value); };
Decl          : IdList ':' FuncDecl                         { printf("No\n"); ProcDeclFunc($1,$3); };
Decl          : IdList "::" CodeBlock                       { printf("No\n"); ProcFuncBody($1,$3); };

IdList        : IdItem ',' IdList                           { $$ = AppendIdList($1,$3); printf(",\n");};
IdList        : IdItem                                      { $$ = $1; printf("___\n");};

IdItem        : Id                                          { $$ = ProcName($1,MAKE_SPAN(@1)); printf("Id\n");}

Id            : IDENT_TOK                                   { @$ = @1; $$ = strdup(yytext); };

BaseType      : "int"                                       { printf("here1?");@$ = @1; $$ = IntBaseType; };
BaseType      : "chr"                                       { printf("here2?");@$ = @1; $$ = ChrBaseType; };
BaseType      : "bool"                                      { printf("her3?");@$ = @1; $$ = BoolBaseType; };
BaseType      : "void"                                      { printf("her4?");@$ = @1; $$ = VoidBaseType; };

Literal       : INTLIT_TOK                                  { printf("int\n");  @$ = @1; $$ = MakeLiteralDesc(yytext,IntBaseType); };
Literal       : CHRLIT_TOK                                  { printf("chr\n"); @$ = @1; $$ = MakeLiteralDesc(yytext,ChrBaseType); };
Literal       : BOOLLIT_TOK                                 { printf("bool\n");@$ = @1; $$ = MakeLiteralDesc(yytext,BoolBaseType); };

FuncDecl      : '(' ')' "->" BaseType                       {printf("herdssd1?"); $$ = VoidBaseType; };

CodeBlock     : '{' StmtSeq '}'                             { printf("her1dfsfsd?");$$ = $2; };

StmtSeq       : Stmt StmtSeq                                { printf("herfsdfsd1?");$$ = AppendSeq($1,$2); };
StmtSeq       :                                             { printf("her1sdfsdf?");$$ = NULL; };

Stmt :                                                       {printf("her1werewrwe?");};

%%

void
yyerror( char *s)     {
  char * buf = malloc(strlen(s) + strlen(yytext) + 20 + 1);
  sprintf(buf,"ERROR: \"%s\" token: \"%s\"",s,yytext);
  PostMessage(LastTokenSpan(),buf);
  free(buf);
}
