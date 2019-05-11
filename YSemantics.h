/* Semantics.h
 The action and supporting routines for performing semantics processing.
 */

#include "SymTab.h"
#include "IOMngr.h"
#include "YCodeGen.h"
#include "YStructs.h"

// Supporting Routines

void InitSemantics();
void FinishSemantics();

// Semantics Actions

// Step 1
void                    ProcDecl(struct IdList * idList, enum BaseTypes baseType, int initialValue);
void                    ProcDeclFunc(struct IdList * idList, enum BaseTypes type);
void                    ProcFuncBody(struct IdList * idItem, struct InstrSeq * codeBlock);
struct IdList *         AppendIdList(struct IdList * item, struct IdList * list);
struct IdList *         ProcName(char * tokenText, struct Span span);

struct InstrSeq * ProcAssign(char * id, struct ExprResult * res);
struct InstrSeq * Put(struct ExprResult * res);
struct InstrSeq * PutString(char * str);

struct ExprResult * ProcOp(struct ExprResult * oprnd1, struct ExprResult * oprnd2, int opNum);
struct ExprResult * ProcUmin(struct ExprResult * oprnd1);
struct ExprResult * ProcLit(char * val, enum BaseTypes type);
struct ExprResult * ProcLoadVar(char * id);
struct ExprResult * GetInt();

struct CondResult * ProcCond(struct ExprResult * x, char * op, struct ExprResult * y);

struct InstrSeq * ProcIf(struct CondResult * res, struct InstrSeq * ins);
struct InstrSeq * ProcIfElse(struct CondResult * res, struct InstrSeq * ins, struct InstrSeq * ins2);
struct InstrSeq * ProcWhile(struct CondResult * cond, struct InstrSeq * ins);
struct InstrSeq * ProcFuncCall(char * id);
struct InstrSeq * ProcFor(struct InstrSeq * a, struct CondResult * cond, struct InstrSeq * prog, struct InstrSeq * body);
struct InstrSeq * ProcTuri(char * id, struct CondResult * cond, struct ExprResult * exp1, struct ExprResult * exp2 );
struct InstrSeq * ProcLoop(struct InstrSeq * seq);
struct InstrSeq * ProcBreak();
