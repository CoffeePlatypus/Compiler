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
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;
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
%type <ExprResult> Expr
%type <ExprResult> Oprnd
%type <CondResult> Cond
%type <Text> CondOp

/* List of token name, corresponding numbers will be generated */
/* y.tab.h will be generated from these */
/* %token symbolic-name text-used-in-rules */
%token IDENT_TOK
%token INT_TOK    	   "int"
%token CHR_TOK    	   "chr"
%token BOOL_TOK        "bool"
%token VOID_TOK        "void"
%token IF_TOK          "if"
%token ELSE_TOK        "else"
%token WHILE_TOK       "while"
%token INTLIT_TOK
%token CHRLIT_TOK
%token BOOLLIT_TOK
%token ARROW_TOK       "->"
%token DBLCOLON_TOK    "::"
%token PUT_TOK         "put"
%token GET_TOK         "get"
%token GTEQ_TOK        ">="
%token LTEQ_TOK        "<="
%token EQ_TOK          "=="
%token NTEQ_TOK        "##"

/* operator associativity and precedence in increasing order */
%left '+' '-'
%left '*' '/'
%left UMINUS

%%
// Step 1
Module        : DeclList                                    {  };

DeclList      : Decl DeclList                               {  };
DeclList      : Decl                                        {  };

Decl          : IdList ':' BaseType                         { ProcDecl($1,$3,0); };
Decl          : IdList "::" Literal                         { ProcDecl($1,$3->baseType,$3->value); };
Decl          : IdList ':' FuncDecl                         { ProcDeclFunc($1,$3); };
Decl          : IdList "::" CodeBlock                       { ProcFuncBody($1,$3); };

IdList        : IdItem ',' IdList                           { $$ = AppendIdList($1,$3); };
IdList        : IdItem                                      { $$ = $1; };

IdItem        : Id                                          { $$ = ProcName($1,MAKE_SPAN(@1)); };

Id            : IDENT_TOK                                   { @$ = @1; $$ = strdup(yytext); };

BaseType      : "int"                                       { @$ = @1; $$ = IntBaseType;  };
BaseType      : "chr"                                       { @$ = @1; $$ = ChrBaseType;  };
BaseType      : "bool"                                      { @$ = @1; $$ = BoolBaseType; };
BaseType      : "void"                                      { @$ = @1; $$ = VoidBaseType; };

Literal       : INTLIT_TOK                                  { @$ = @1; $$ = MakeLiteralDesc(yytext,IntBaseType);  };
Literal       : CHRLIT_TOK                                  { @$ = @1; $$ = MakeLiteralDesc(yytext,ChrBaseType);  };
Literal       : BOOLLIT_TOK                                 { @$ = @1; $$ = MakeLiteralDesc(yytext,BoolBaseType); };

FuncDecl      : '(' ')' "->" BaseType                       { $$ = VoidBaseType; };

CodeBlock     : '{' StmtSeq '}'                             { $$ = $2; printf("codeblock\n");};

StmtSeq       : Stmt StmtSeq                                { $$ = AppendSeq($1,$2); };
StmtSeq       :                                             { $$ = NULL; };

Stmt          : "put" '(' Expr ')'                          { $$ = Put($3);                };
Stmt          : AssignStmt                                  { $$ = $1;                     };
//Stmt          : "if" Cond CodeBlock                         { $$ = ProcIf($2, $3);       };
Stmt          : "if" Cond CodeBlock "else" CodeBlock        { $$ = ProcIfElse($2, $3, $5); };
Stmt          : "while" Cond CodeBlock                      { $$ = ProcWhile($2, $3);      };

Cond          : Expr CondOp Expr                            { $$ = ProcCond($1, $2, $3);   };

CondOp        : '>'                                         { $$ = strdup("ble");   };
CondOp        : '<'                                         { $$ = strdup("bge");   };
CondOp        : ">="                                        { $$ = strdup("bl" );   };
CondOp        : "<="                                        { $$ = strdup("bg" );   };
CondOp        : "=="                                        { $$ = strdup("bne");   };
CondOp        : "##"                                        { $$ = strdup("beq");   };

AssignStmt    : Id '=' Expr                                 { $$ = ProcAssign($1, $3); };

Expr    :  Expr '+' Expr                { $$ = ProcOp($1, $3, 0);     } ;
Expr    :  Expr '-' Expr                { $$ = ProcOp($1, $3, 1);     } ;
Expr    :  Expr '*' Expr                { $$ = ProcOp($1, $3, 2);     } ;
Expr    :  Expr '/' Expr                { $$ = ProcOp($1, $3, 3);     } ;
Expr    :  '(' Expr ')'                 { $$ = $2;                    } ;
Expr    :  '-' Expr       %prec UMINUS  { $$ = ProcUmin($2);          } ;
Expr    :  GET_TOK '(' "int" ')'        { $$ = GetInt();              } ;
Expr    :  Oprnd                        { $$ = $1;                    } ;
Oprnd   :  INTLIT_TOK                   { $$ = ProcLit(strdup(yytext), IntBaseType);       } ;
Oprnd   :  Id                           { $$ = ProcLoadVar($1);                            } ;
Oprnd   :  CHRLIT_TOK                   { $$ = ProcLit(strdup(yytext), ChrBaseType);       } ;
Oprnd   :  BOOLLIT_TOK                  { $$ = ProcLit(strdup(yytext), BoolBaseType);      } ;


%%

void
yyerror( char *s)     {
  char * buf = malloc(strlen(s) + strlen(yytext) + 20 + 1);
  sprintf(buf,"ERROR: \"%s\" token: \"%s\"",s,yytext);
  PostMessage(LastTokenSpan(),buf);
  free(buf);
}
