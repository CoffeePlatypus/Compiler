/* Main of Parser */
/* ParseDriver.c
   Author:      Julia Froegel
   Created:     03/26/2019
   Resources:
     - Talked to David about the grammer and he helped me realize two of my rules were pointless
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Scanner.h"
#include "IOMngr.h"
#include "Grammar.h"

int
main(int argc, char * argv[])
{
  if (argc != 2) {
    fprintf(stderr,"usage: Parse filename\n");
    return 1;
  }

  if (!OpenSource(argv[1])) {
    fprintf(stderr,"Source File Not Found.\n");
    return 1;
  }

  if (yyparse() == 0) {
    PostMessage(MakeEOFSpan(),"Parse Successful");
  }
  else {
    PostMessage(MakeEOFSpan(),"Parse Unsuccessful");
  }

  CloseSource();

  return 0;
}

/* Updated Grammer
- todo rewrite expr and term to use left reccurstion
- change assign procudtion to provide opportuity to capture the left hand side identifier

                                           Select Set
<Prog>    :==  Ident { <StmtSeq> }         Ident {
<StmtSeq> :==  <Stmt> ; <StmtSeq>          INT CHR Ident    // given
<StmtSeq> :==                              }                // given
<Stmt>    :== <Decl>                       [INT | CHR] <var list>;
<Stmt>    :== <Assign>                     Ident := <Expr>
<Decl>    :== <Type> <IDLst>               INT CHR          // given
<Type>    :== INT
<Type>    :== CHR
<IDLst>   :== Ident <MLst>
<MLst>    :== , <IDLst>
<MLst>    :==
<Assign>  :==  Ident := <Expr>
<Expr>    :==  <Expr> <AddOp> <Term>         IntLit Ident ( -
<Expr>    :==  <AddOp> <Term>                +
<Expr>    :==  <Term>                        IntLit Ident (
<Term>    :==  <Term> <MultOp> <Factor>       IntLit Ident (
<Term>    :==  <MultOp> <Factor>              *
<Term>    :==  <Factor>
<Factor>  :==  ( <Expr> )
<Factor>  :==  - <Factor>
<Factor>  :==  IntLit
<Factor>  :==  Ident
<AddOp>   :==  -
<AddOp>   :==  +
<MultOp>  :==  *
<MultOp>  :==  /
*/
