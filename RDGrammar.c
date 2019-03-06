#include <stdbool.h>
#include <stdio.h>

#include "IOMngr.h"
#include "RDGrammar.h"
#include "RDTokens.h"

bool rd_debug = 0;

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
  if (rd_debug) printf("StmtSeq %d\n",CurrentToken());
    switch (CurrentToken()) {
      case INT_TOK:
      case CHR_TOK:
      case IDENT_TOK:
        if(! Stmt()) return LEAVE_FAIL;
        if(! Match(SEMI_TOK)) return LEAVE_FAIL;
        if(! StmtSeq()) return LEAVE_FAIL;
        break;
      case RBRACE_TOK:
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
  if (rd_debug) printf("stmt %d\n",CurrentToken());
  switch (CurrentToken()) {
     case INT_TOK:
     case CHR_TOK:
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
if (rd_debug) printf("good stmt\n" );
  return LEAVE_SUCC;
}

bool Decl()
// <Decl>    :== <Type> <IDLst>              INT CHR
{
     ENTER;
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
     ENTER;
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
     ENTER;
     if (rd_debug) printf("idlst %d\n",CurrentToken());
     if(!Match(IDENT_TOK)) return LEAVE_FAIL;
     if(!MLst()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool MLst()
// <MLst>    :== , <IDLst>
// <MLst>    :==
{
     ENTER;
     if (rd_debug) printf("comma? %d \n",CurrentToken());
     switch (CurrentToken()) {
          case COMMA_TOK:
               Match(COMMA_TOK);
               if(! IDLst()) return LEAVE_FAIL;
          case SEMI_TOK:
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
     ENTER;
     if (rd_debug) printf("assign %d\n",CurrentToken());
     if(!Match(IDENT_TOK)) return LEAVE_FAIL;
     if(!Match(ASSIGN_TOK)) return LEAVE_FAIL;
     if(!Expr()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool Expr()
// <Expr>		:==	<Term> <MExpr>
{
     ENTER;
     if(!Term()) return LEAVE_FAIL;
     if(!MExpr()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool MExpr()
// <MExpr>		:==	<AddOp> <Term> <MExpr>
// <MExpr>		:==
{
     ENTER;
     // might not work - what about neg sign
     if (rd_debug) printf("adders %d\n",CurrentToken());
     // if(CurrentToken() == MINUS_TOK || CurrentToken() == PLUS_TOK) {
     //      if(!AddOp()) return LEAVE_FAIL;
     //      if(!Term()) return LEAVE_FAIL;
     //      if(!MExpr()) return LEAVE_FAIL;
     // }
     switch (CurrentToken()) {
          case MINUS_TOK:
          case PLUS_TOK:
               if(!AddOp()) return LEAVE_FAIL;
               if(!Term()) return LEAVE_FAIL;
               if(!MExpr()) return LEAVE_FAIL;
          case SEMI_TOK:
          case RPAREN_TOK:
               break;
          default:
               ParseError("No option in switch");
               return LEAVE_FAIL;
               break;
   	}
     return LEAVE_SUCC;
}

bool Term()
//<Term>		:==	<Factor> <MTerm>
{
     ENTER;
     if (rd_debug) printf("terms %d\n",CurrentToken());
     if(! Factor()) return LEAVE_FAIL;
     if(! MTerm()) return LEAVE_FAIL;
     return LEAVE_SUCC;
}

bool MTerm()
//<MTerm>		:==	<MultOp> <Factor> <MTerm>
//<MTerm>		:==
{
     ENTER;
     if (rd_debug) printf("multy %d\n",CurrentToken());
     // if(CurrentToken() == TIMES_TOK || CurrentToken() == DIV_TOK) {
     //      if(! MultOp()) return LEAVE_FAIL;
     //      if(! Factor()) return LEAVE_FAIL;
     //      if(! MTerm()) return LEAVE_FAIL;
     // }
     switch (CurrentToken()) {
          case TIMES_TOK:
          case DIV_TOK:
               if(! MultOp()) return LEAVE_FAIL;
              if(! Factor()) return LEAVE_FAIL;
              if(! MTerm()) return LEAVE_FAIL;
          case SEMI_TOK:
          case RPAREN_TOK:
          case MINUS_TOK:
          case PLUS_TOK:
               break;
          default:
               ParseError("No option in switch");
               return LEAVE_FAIL;
               break;
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
     ENTER;
     if (rd_debug) printf("Factor %d\n",CurrentToken());
     switch (CurrentToken()) {
          case LPAREN_TOK:
               // printf("(\n");
               Match(LPAREN_TOK);
               if(!Expr()) return LEAVE_FAIL;
               // printf(")?\n");
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
     ENTER;
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
ENTER;
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
