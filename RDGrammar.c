#include <stdbool.h>
#include <stdio.h>

#include "IOMngr.h"
#include "RDGrammar.h"
#include "RDTokens.h"

bool Prog()
// <Prog>		  :==	Ident { <StmtSeq> }
{
  ENTER;
  if (!Match(IDENT_TOK)) return LEAVE_FAIL;
  if (!Match(LBRACE_TOK)) return LEAVE_FAIL;
  if (!StmtSeq()) return LEAVE_FAIL;
  if (!Match(RBRACE_TOK)) return LEAVE_FAIL;

  return LEAVE_SUCC;
}

bool StmtSeq()
// <StmtSeq>  :==	<Stmt> ; <StmtSeq>
// <StmtSeq>	:==
{
  ENTER;
  printf("StmtSeq %d\n",CurrentToken());
  switch (CurrentToken()) {
     case INT_TOK:
          // assign
     case CHR_TOK:
          // assign
	case IDENT_TOK:
          // Declare
          if(! Stmt()) return LEAVE_FAIL;
          if(! Match(SEMI_TOK)) return LEAVE_FAIL;
          if(! StmtSeq()) return LEAVE_FAIL;
	    break;
	case RBRACE_TOK:
          // empty statement
	    break;
	default:
          ParseError("No option in switch");
	     return LEAVE_FAIL;
		break;
	}

	return LEAVE_SUCC;
}

bool Stmt()
// <Stmt>    :== <Decl>
// <Stmt>    :== <Assign>
{
  ENTER;
  printf("stmt %d\n",CurrentToken());
  switch (CurrentToken()) {
     case INT_TOK:
          printf("stmtass %d\n",CurrentToken());
     case CHR_TOK:
          printf("stmtass %d\n",CurrentToken());
          if(! Decl()) return LEAVE_FAIL;
          break;
	case IDENT_TOK:
          if(!Assign()) return LEAVE_FAIL;
	    break;
	default:
          ParseError("No option in switch");
	     return LEAVE_FAIL;
		break;
	}
printf("good stmt\n" );
  return LEAVE_SUCC;
}

bool Decl()
// <Decl>    :== <Type> <IDLst>              INT CHR
{
     // printf("dec %d\n",CurrentToken());
     if(!Type()) return LEAVE_FAIL;
     // printf("dec id %d\n",CurrentToken());
     if(!IDLst()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool Type()
// <Type>    :== INT
// <Type>    :== CHR
{
     switch (CurrentToken()) {
        case INT_TOK:
             Match(INT_TOK);
             break;
        case CHR_TOK:
             Match(CHR_TOK);
             break;
   	default:
             ParseError("No option in switch");
   	     return LEAVE_FAIL;
   		break;
   	}
     return LEAVE_SUCC;
}

bool IDLst()
// <IDLst>   :== Ident <MLst>
{
     printf("idlst %d\n",CurrentToken());
     if(!Match(IDENT_TOK)) return LEAVE_FAIL;
     if(!MLst()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool MLst()
// <MLst>    :== , <IDLst>
// <MLst>    :==
{
     printf("comma? %d ",CurrentToken());
     switch (CurrentToken()) {
          case COMMA_TOK:
               printf("yah\n");
               Match(COMMA_TOK);
               if(! IDLst()) return LEAVE_FAIL;
          case SEMI_TOK:
               printf("nah\n");
               break;
          default:
               ParseError("No option in switch");
               return LEAVE_FAIL;
               break;
   	}
     return LEAVE_SUCC;

}

bool Assign()
// <Assign>		:==	<Ident> := <Expr>
{
     printf("assign %d\n",CurrentToken());
     if(!Match(IDENT_TOK)) return LEAVE_FAIL;
     if(!Match(ASSIGN_TOK)) return LEAVE_FAIL;
     if(!Expr()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool Expr()
// <Expr>		:==	<Term> <MExpr>
{
     if(!Term()) return LEAVE_FAIL;
     if(!MExpr()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool MExpr()
// <MExpr>		:==	<AddOp> <Term> <MExpr>
// <MExpr>		:==
{
     // might not work - what about neg sign
     printf("adders %d\n",CurrentToken());
     if(CurrentToken() == MINUS_TOK || CurrentToken() == PLUS_TOK) {
          if(!AddOp()) return LEAVE_FAIL;
          if(!Term()) return LEAVE_FAIL;
          if(!MExpr()) return LEAVE_FAIL;
     }
     return LEAVE_SUCC;
}

bool Term()
//<Term>		:==	<Factor> <MTerm>
{
     printf("terms %d\n",CurrentToken());
     if(! Factor()) return LEAVE_FAIL;
     if(! MTerm()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool MTerm()
//<MTerm>		:==	<MultOp> <Factor> <MTerm>
//<MTerm>		:==
{
     printf("multy %d\n",CurrentToken());
     if(CurrentToken() == TIMES_TOK || CurrentToken() == DIV_TOK) {
          printf("malt\n");
          if(! MultOp()) return LEAVE_FAIL;
          if(! Factor()) return LEAVE_FAIL;
          if(! MTerm()) return LEAVE_FAIL;
          printf("out\n");
     }
     return LEAVE_SUCC;
}

bool Factor()
// <Factor>	:==	( <Expr> )
// <Factor>	:==	- <Factor>
// <Factor>	:==	<IntLit>
// <Factor>	:==	<FloatLit>
// <Factor>	:==	<Ident>
{
     printf("Factor %d\n",CurrentToken());
     switch (CurrentToken()) {
          case LPAREN_TOK:
               printf("(\n");
               Match(LPAREN_TOK);
               if(!Expr()) return LEAVE_FAIL;
               printf(")?\n");
               if(!Match(RPAREN_TOK)) return LEAVE_FAIL;
               break;
          case MINUS_TOK:
               Match(MINUS_TOK);
               if(!Factor()) return LEAVE_FAIL;
               break;
          case INTLIT_TOK:
               Match(INTLIT_TOK);
               break;
          case FLOATLIT_TOK:
               Match(FLOATLIT_TOK);
               break;
          case IDENT_TOK :
               Match(IDENT_TOK);
               break;
          default:
               ParseError("No option in switch");
               return LEAVE_FAIL;
               break;
   	}
     return LEAVE_SUCC;
}

bool AddOp()
// <AddOp>		:==	-
// <AddOp>		:==	+
{
     switch (CurrentToken()) {
        case MINUS_TOK:
             Match(MINUS_TOK);
             break;
        case PLUS_TOK:
             Match(PLUS_TOK);
             break;
   	default:
             ParseError("No option in switch");
   	     return LEAVE_FAIL;
   		break;
   	}
     return LEAVE_SUCC;
}

bool MultOp()
// <MultOp>	:==	*
// <MultOp>	:==	/
{
     switch (CurrentToken()) {
        case TIMES_TOK:
             Match(TIMES_TOK);
             break;
        case DIV_TOK:
             Match(DIV_TOK);
             break;
   	default:
             ParseError("No option in switch");
   	     return LEAVE_FAIL;
   		break;
   	}
     return LEAVE_SUCC;
}
