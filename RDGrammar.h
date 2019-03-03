/* RDGrammar.h
     Contains the grammar parsing routines for the grammar

                                           Select Set
<Prog>    :==  Ident { <StmtSeq> }
<StmtSeq> :==  <Stmt> ; <StmtSeq>          INT CHR Ident
<StmtSeq> :==                              }
<Stmt>    :== <Decl>
<Stmt>    :== <Assign>
<Decl>    :== <Type> <IDLst>               INT CHR
<Type>    :== INT
<Type>    :== CHR
<IDLst>   :== Ident <MLst>
<MLst>    :== , <IDLst>
<MLst>    :==
<Assign>  :==  Ident := <Expr>
<Expr>    :==  <Term> <MExpr>
<MExpr>   :==  <AddOp> <Term> <MExpr>      - +
<MExpr>   :==                              ; )
<Term>    :==  <Factor> <MTerm>
<MTerm>   :==  <MultOp> <Factor> <MTerm>   * /
<MTerm>   :==                              - + ; )
<Factor>  :==  ( <Expr> )
<Factor>  :==  - <Factor>
<Factor>  :==  IntLit
<Factor>  :==  FloatLit
<Factor>  :==  Ident
<AddOp>   :==  -
<AddOp>   :==  +
<MultOp>  :==  *
<MultOp>  :==  /
*/

#include "RDSupport.h"

/* Nonterminal Functions */
bool Prog();
bool StmtSeq();
bool Stmt();
bool Decl();
bool Type();
bool IDLst();
bool MLst();
bool Assign();
bool Expr();
bool MExpr();
bool Term();
bool MTerm();
bool Factor();
bool AddOp();
bool MultOp();
