#include <stdbool.h>

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

  switch (CurrentToken()) {
  case INT_TOK:
  case CHR_TOK:
	case IDENT_TOK:
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

  return LEAVE_SUCC;
}

bool Decl()
// <Decl>    :== <Type> <IDLst>              INT CHR
{
}

bool Type()
// <Type>    :== INT
// <Type>    :== CHR
{
}

bool IDLst()
// <IDLst>   :== Ident <MLst>
{
}

bool MLst()
// <MLst>    :== , <IDLst>
// <MLst>    :==
{
}

bool Assign()
// <Assign>		:==	<Ident> := <Expr>
{
}

bool Expr()
// <Expr>		:==	<Term> <MExpr>
{
}

bool MExpr()
// <MExpr>		:==	<AddOp> <Term> <MExpr>
// <MExpr>		:==
{
}

bool Term()
//<Term>		:==	<Factor> <MTerm>
{
}

bool MTerm()
//<MTerm>		:==	<MultOp> <Factor> <MTerm>
//<MTerm>		:==
{
}

bool Factor()
// <Factor>	:==	( <Expr> )
// <Factor>	:==	- <Factor>
// <Factor>	:==	<IntLit>
// <Factor>	:==	<FloatLit>
// <Factor>	:==	<Ident>
{
}

bool AddOp()
// <AddOp>		:==	-
// <AddOp>		:==	+
{
}

bool MultOp()
// <MultOp>	:==	*
// <MultOp>	:==	/
{
}
